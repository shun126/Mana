#!/usr/bin/env node
/*
 * Tests the WebAssembly build of Mana and the Playground worker.
 *
 *   node web/tests/run-tests.js build-web/web
 *
 * The argument is the directory holding mana.js and mana.wasm. The tests use
 * the Playground's own mana-runtime.js and worker.js, so what passes here is
 * what the published page runs. worker.js is a browser worker; it runs here
 * in a worker_threads thread with importScripts() and postMessage() shimmed.
 */
"use strict";

const assert = require("assert");
const fs = require("fs");
const path = require("path");
const { Worker } = require("worker_threads");

const ROOT = path.resolve(__dirname, "..", "..");
const PLAYGROUND = path.join(ROOT, "documents", "pages", "playground");
const EXPECTED = path.join(__dirname, "expected");
const EXAMPLES = ["hello", "actor", "request"];

const moduleDirectory = path.resolve(process.argv[2] || path.join(ROOT, "build-web", "web"));
for (const name of ["mana.js", "mana.wasm"]) {
  if (!fs.existsSync(path.join(moduleDirectory, name))) {
    console.error(`error: ${path.join(moduleDirectory, name)} does not exist`);
    process.exit(2);
  }
}

const ManaRuntime = require(path.join(PLAYGROUND, "mana-runtime.js"));
const createManaModule = require(path.join(moduleDirectory, "mana.js"));

const readExample = (name) => fs.readFileSync(path.join(PLAYGROUND, "examples", name + ".mn"), "utf8");
const readExpected = (name) => fs.readFileSync(path.join(EXPECTED, name + ".txt"), "utf8").replace(/\r\n/g, "\n");

const SYNTAX_ERROR = [
  "actor Broken",
  "{",
  "    action main()",
  "    {",
  "        print(\"missing semicolon\")",
  "    }",
  "}",
  "",
].join("\n");

const UNKNOWN_ACTOR = [
  "actor Event",
  "{",
  "    action main()",
  "    {",
  "        request(1, Enemy->attack());",
  "    }",
  "}",
  "",
].join("\n");

const ENDLESS = [
  "actor Endless",
  "{",
  "    action main()",
  "    {",
  "        print(\"started\\n\");",
  "        while (true)",
  "        {",
  "        }",
  "    }",
  "}",
  "",
].join("\n");

// -- direct tests of the module ------------------------------------------

async function createRuntime() {
  const output = [];
  const runtime = await ManaRuntime.create(createManaModule, (level, text) => output.push({ level, text }));
  return { runtime, output };
}

function compileAndRun(runtime, output, files, entry = "main.mn", limitMilliseconds = 5000) {
  output.length = 0;
  runtime.reset();
  for (const [name, text] of Object.entries(files)) runtime.setFile(name, text);
  const result = runtime.compile(entry);
  if (!result.success) return { result, text: "" };
  assert.ok(runtime.start(), "the VM did not start");
  const deadline = Date.now() + limitMilliseconds;
  while (runtime.step(20)) {
    if (Date.now() > deadline) {
      runtime.stop();
      throw new Error("the program did not finish in time");
    }
  }
  const text = output.filter((o) => o.level === "info").map((o) => o.text).join("");
  return { result, text };
}

const tests = [];
const test = (name, body) => tests.push({ name, body });

test("the module reports the Mana version", async () => {
  const { runtime } = await createRuntime();
  const version = JSON.parse(fs.readFileSync(path.join(ROOT, "runner", "common", "Version.json"), "utf8"));
  assert.strictEqual(runtime.version(), `${version.MajorVersion}.${version.MinorVersion}.${version.PatchVersion}`);
});

for (const name of EXAMPLES) {
  test(`example '${name}' compiles and prints the expected output`, async () => {
    const { runtime, output } = await createRuntime();
    const { result, text } = compileAndRun(runtime, output, { "main.mn": readExample(name) });
    assert.ok(result.success, JSON.stringify(result.diagnostics));
    assert.ok(result.programSize > 0, "the program image is empty");
    assert.deepStrictEqual(result.diagnostics, []);
    assert.strictEqual(text, readExpected(name));
  });
}

test("a syntax error comes back as a diagnostic with a line number", async () => {
  const { runtime, output } = await createRuntime();
  const { result } = compileAndRun(runtime, output, { "main.mn": SYNTAX_ERROR });
  assert.strictEqual(result.success, false);
  assert.strictEqual(result.programSize, 0);
  const error = result.diagnostics.find((d) => d.severity === "error");
  assert.ok(error, JSON.stringify(result.diagnostics));
  assert.strictEqual(error.filename, "main.mn");
  assert.ok(error.line >= 5 && error.line <= 6, `line ${error.line}`);
  assert.match(ManaRuntime.formatDiagnostic(error), /^main\.mn:\d+: error: /);
});

test("a semantic error points at the offending line", async () => {
  const { runtime, output } = await createRuntime();
  const { result } = compileAndRun(runtime, output, { "main.mn": UNKNOWN_ACTOR });
  assert.strictEqual(result.success, false);
  const error = result.diagnostics.find((d) => d.severity === "error");
  assert.ok(error, JSON.stringify(result.diagnostics));
  assert.strictEqual(error.line, 5);
  assert.match(error.message, /Enemy/);
});

test("the next compile succeeds after an error", async () => {
  const { runtime, output } = await createRuntime();
  assert.strictEqual(compileAndRun(runtime, output, { "main.mn": SYNTAX_ERROR }).result.success, false);
  const { result, text } = compileAndRun(runtime, output, { "main.mn": readExample("hello") });
  assert.ok(result.success, JSON.stringify(result.diagnostics));
  assert.strictEqual(text, readExpected("hello"));
});

test("sources can include other in-memory files", async () => {
  const { runtime, output } = await createRuntime();
  const files = {
    "main.mn": [
      "include \"npc/guide.mn\";",
      "actor Event",
      "{",
      "    action main()",
      "    {",
      "        awaitCompletion(1, Guide->talk());",
      "    }",
      "}",
      "",
    ].join("\n"),
    "npc/guide.mn": [
      "include \"../common.mn\";",
      "actor Guide",
      "{",
      "    action talk()",
      "    {",
      "        print(\"Guide: %d keys\\n\", kKeys);",
      "    }",
      "}",
      "",
    ].join("\n"),
    "common.mn": "const int kKeys = 3;\n",
  };
  const { result, text } = compileAndRun(runtime, output, files);
  assert.ok(result.success, JSON.stringify(result.diagnostics));
  assert.strictEqual(text, "Guide: 3 keys\n");
});

test("a missing include is reported, not read from disk", async () => {
  const { runtime, output } = await createRuntime();
  const { result } = compileAndRun(runtime, output, { "main.mn": "include \"missing.mn\";\n" });
  assert.strictEqual(result.success, false);
  assert.ok(result.diagnostics.length > 0);
});

test("an endless loop yields back at the step budget", async () => {
  const { runtime, output } = await createRuntime();
  runtime.reset();
  runtime.setFile("main.mn", ENDLESS);
  assert.ok(runtime.compile("main.mn").success);
  assert.ok(runtime.start());
  const startedAt = Date.now();
  assert.strictEqual(runtime.step(100), true);
  assert.ok(Date.now() - startedAt < 2000, "step() did not return near its budget");
  runtime.stop();
  assert.strictEqual(runtime.step(10), false);
  assert.strictEqual(output.map((o) => o.text).join(""), "started\n");
});

// -- the Playground worker -----------------------------------------------

const WORKER_SHIM = `
"use strict";
const { parentPort, workerData } = require("worker_threads");
const fs = require("fs");
const path = require("path");
const vm = require("vm");
// An eval'd worker has CommonJS globals. Hide them so the scripts define
// their globals the way they do in a browser worker.
globalThis.module = undefined;
globalThis.exports = undefined;
globalThis.self = globalThis;
globalThis.onmessage = null;
globalThis.require = require;
// Emscripten looks for mana.wasm next to __dirname under Node.js.
globalThis.__dirname = workerData.moduleDirectory;
globalThis.postMessage = (message) => parentPort.postMessage(message);
globalThis.importScripts = (...names) => {
  for (const name of names) {
    const directory = name === "mana.js" ? workerData.moduleDirectory : workerData.playground;
    const file = path.join(directory, name);
    vm.runInThisContext(fs.readFileSync(file, "utf8"), { filename: file });
  }
};
parentPort.on("message", (data) => { if (globalThis.onmessage) globalThis.onmessage({ data }); });
importScripts("worker.js");
`;

class PlaygroundWorker {
  constructor() {
    this.messages = [];
    this.waiters = [];
    this.worker = new Worker(WORKER_SHIM, {
      eval: true,
      workerData: { moduleDirectory, playground: PLAYGROUND },
    });
    this.worker.on("message", (message) => {
      this.messages.push(message);
      this.waiters = this.waiters.filter((w) => !w(message));
    });
    this.worker.on("error", (error) => {
      this.error = error;
    });
  }

  waitFor(predicate, timeoutMilliseconds = 10000) {
    const found = this.messages.find(predicate);
    if (found) return Promise.resolve(found);
    return new Promise((resolve, reject) => {
      const timer = setTimeout(() => reject(new Error("timed out waiting for the worker" + (this.error ? ": " + this.error : ""))), timeoutMilliseconds);
      this.waiters.push((message) => {
        if (!predicate(message)) return false;
        clearTimeout(timer);
        resolve(message);
        return true;
      });
    });
  }

  run(id, source) {
    this.worker.postMessage({ type: "run", id, entry: "main.mn", files: { "main.mn": source } });
  }

  output(id) {
    return this.messages
      .filter((m) => m.type === "output" && m.id === id)
      .flatMap((m) => m.chunks)
      .filter((c) => c.level === "info")
      .map((c) => c.text)
      .join("");
  }

  terminate() {
    return this.worker.terminate();
  }
}

test("the worker runs the Request example", async () => {
  const worker = new PlaygroundWorker();
  try {
    const ready = await worker.waitFor((m) => m.type === "ready");
    assert.ok(ready.version);
    worker.run(1, readExample("request"));
    const done = await worker.waitFor((m) => m.type === "done" && m.id === 1);
    assert.strictEqual(done.status, "finished");
    assert.strictEqual(worker.output(1), readExpected("request"));
  } finally {
    await worker.terminate();
  }
});

test("the worker reports compile errors", async () => {
  const worker = new PlaygroundWorker();
  try {
    await worker.waitFor((m) => m.type === "ready");
    worker.run(1, SYNTAX_ERROR);
    const compiled = await worker.waitFor((m) => m.type === "compiled" && m.id === 1);
    assert.strictEqual(compiled.result.success, false);
    assert.ok(compiled.result.diagnostics.some((d) => d.severity === "error" && d.line > 0));
    const done = await worker.waitFor((m) => m.type === "done" && m.id === 1);
    assert.strictEqual(done.status, "compile-error");
  } finally {
    await worker.terminate();
  }
});

test("terminating a worker stops an endless script, and a new worker runs again", async () => {
  const stuck = new PlaygroundWorker();
  await stuck.waitFor((m) => m.type === "ready");
  stuck.run(1, ENDLESS);
  await stuck.waitFor((m) => m.type === "output" && m.id === 1);
  await new Promise((resolve) => setTimeout(resolve, 300));
  assert.ok(!stuck.messages.some((m) => m.type === "done"), "the endless script finished");
  assert.strictEqual(stuck.output(1), "started\n");
  const exitCode = await stuck.terminate();
  assert.strictEqual(exitCode, 1, "terminate() did not stop the worker");

  const fresh = new PlaygroundWorker();
  try {
    await fresh.waitFor((m) => m.type === "ready");
    fresh.run(2, readExample("hello"));
    const done = await fresh.waitFor((m) => m.type === "done" && m.id === 2);
    assert.strictEqual(done.status, "finished");
    assert.strictEqual(fresh.output(2), readExpected("hello"));
  } finally {
    await fresh.terminate();
  }
});

// -------------------------------------------------------------------------

(async () => {
  let failures = 0;
  for (const { name, body } of tests) {
    try {
      await body();
      console.log(`ok   - ${name}`);
    } catch (error) {
      failures += 1;
      console.log(`FAIL - ${name}`);
      console.log(String((error && error.stack) || error).replace(/^/gm, "       "));
    }
  }
  console.log(`\n${tests.length - failures} passed, ${failures} failed`);
  process.exit(failures === 0 ? 0 : 1);
})();
