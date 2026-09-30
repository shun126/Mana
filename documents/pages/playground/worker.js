/*
 * Mana Playground worker.
 *
 * Compiles and runs Mana off the UI thread, so a script that never ends
 * cannot freeze the page. The page stops such a script by terminating this
 * worker and starting a new one.
 *
 * Messages from the page:
 *   { type: "run", id, entry, files: { filename: source } }
 * Messages to the page:
 *   { type: "ready", version }
 *   { type: "load-error", message }
 *   { type: "compiled", id, result }
 *   { type: "output", id, chunks: [{ level, text }] }
 *   { type: "done", id, status: "finished" | "compile-error" | "error", milliseconds }
 */
"use strict";

importScripts("mana.js", "mana-runtime.js");

// How long one VM step runs before buffered output is sent.
var STEP_MILLISECONDS = 20;
// Output is sent at least this often while a script prints.
var FLUSH_MILLISECONDS = 50;
// Output beyond this is dropped so a chatty loop cannot flood the page.
var MAX_OUTPUT_CHARACTERS = 200000;

var currentId = null;
var pending = [];
var pendingCharacters = 0;
var sentCharacters = 0;
var lastFlush = 0;

function flush() {
  if (pending.length === 0) return;
  postMessage({ type: "output", id: currentId, chunks: pending });
  pending = [];
  pendingCharacters = 0;
  lastFlush = performance.now();
}

function onOutput(level, text) {
  if (sentCharacters >= MAX_OUTPUT_CHARACTERS) return;
  sentCharacters += text.length;
  if (sentCharacters >= MAX_OUTPUT_CHARACTERS) {
    text += "\n[output truncated]\n";
  }
  var last = pending[pending.length - 1];
  if (last && last.level === level) last.text += text;
  else pending.push({ level: level, text: text });
  pendingCharacters += text.length;
  if (pendingCharacters > 65536 || performance.now() - lastFlush > FLUSH_MILLISECONDS) {
    flush();
  }
}

var runtimeReady = ManaRuntime.create(createManaModule, onOutput).then(
  function (runtime) {
    postMessage({ type: "ready", version: runtime.version() });
    return runtime;
  },
  function (error) {
    postMessage({ type: "load-error", message: String((error && error.message) || error) });
    throw error;
  }
);

function run(runtime, message) {
  currentId = message.id;
  pending = [];
  pendingCharacters = 0;
  sentCharacters = 0;
  lastFlush = performance.now();
  var startedAt = performance.now();

  function done(status) {
    flush();
    postMessage({
      type: "done",
      id: message.id,
      status: status,
      milliseconds: performance.now() - startedAt,
    });
  }

  runtime.reset();
  Object.keys(message.files).forEach(function (filename) {
    runtime.setFile(filename, message.files[filename]);
  });
  var result = runtime.compile(message.entry);
  postMessage({ type: "compiled", id: message.id, result: result });
  if (!result.success) {
    done("compile-error");
    return;
  }

  if (!runtime.start()) {
    done("error");
    return;
  }
  while (runtime.step(STEP_MILLISECONDS)) {
    flush();
  }
  done("finished");
}

onmessage = function (event) {
  var message = event.data;
  if (message.type !== "run") return;
  runtimeReady.then(function (runtime) {
    run(runtime, message);
  });
};
