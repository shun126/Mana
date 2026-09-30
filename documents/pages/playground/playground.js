/*
 * Mana Playground page.
 *
 * The page never runs Mana itself. It sends the editor's contents to
 * worker.js, which compiles and runs them with the WebAssembly build of the
 * Mana compiler and VM, and shows what comes back. A script that runs too
 * long is stopped by terminating the worker; a fresh one replaces it.
 */
(function () {
  "use strict";

  var EXAMPLES = ["hello", "actor", "request"];
  var DEFAULT_EXAMPLE = "hello";
  var ENTRY = "main.mn";
  // Longer than any example needs, short enough that a runaway loop ends soon.
  var TIME_LIMIT_MILLISECONDS = 5000;

  var elements = {
    run: document.getElementById("run"),
    stop: document.getElementById("stop"),
    reset: document.getElementById("reset"),
    source: document.getElementById("source"),
    gutter: document.getElementById("gutter"),
    status: document.getElementById("status"),
    diagnostics: document.getElementById("diagnostics"),
    output: document.getElementById("output"),
    examples: Array.prototype.slice.call(document.querySelectorAll("[data-example]")),
  };

  var worker = null;
  var workerReady = false;
  var version = "";
  var running = false;
  var runId = 0;
  var timeLimit = null;
  var currentExample = DEFAULT_EXAMPLE;
  var exampleCache = {};

  // -- status and output --------------------------------------------------

  function setStatus(text, state) {
    elements.status.textContent = text;
    if (state) elements.status.setAttribute("data-state", state);
    else elements.status.removeAttribute("data-state");
  }

  function updateButtons() {
    elements.run.disabled = !workerReady || running;
    elements.stop.disabled = !running;
  }

  function clearOutput() {
    elements.output.textContent = "";
    elements.diagnostics.textContent = "";
    elements.diagnostics.hidden = true;
  }

  // Appends [{ text, className }] in one go. Plain text joins the text node
  // before it, so a script that prints every frame adds few DOM nodes.
  function appendOutput(pieces) {
    var output = elements.output;
    var atBottom = output.scrollHeight - output.scrollTop - output.clientHeight < 24;
    pieces.forEach(function (piece) {
      var last = output.lastChild;
      if (!piece.className && last && last.nodeType === Node.TEXT_NODE) {
        last.appendData(piece.text);
      } else if (piece.className) {
        var span = document.createElement("span");
        span.className = piece.className;
        span.textContent = piece.text;
        output.appendChild(span);
      } else {
        output.appendChild(document.createTextNode(piece.text));
      }
    });
    if (atBottom) output.scrollTop = output.scrollHeight;
  }

  function appendNote(text) {
    var last = elements.output.textContent;
    var separator = last && last.charAt(last.length - 1) !== "\n" ? "\n" : "";
    appendOutput([{ text: separator + text + "\n", className: "note" }]);
  }

  function showDiagnostics(diagnostics) {
    var list = elements.diagnostics;
    list.textContent = "";
    diagnostics.forEach(function (diagnostic) {
      var item = document.createElement("li");
      item.className = "severity-" + diagnostic.severity;
      var button = document.createElement("button");
      button.type = "button";
      button.textContent = ManaRuntime.formatDiagnostic(diagnostic);
      if (diagnostic.line > 0 && diagnostic.filename === ENTRY) {
        button.title = "Go to line " + diagnostic.line;
        button.addEventListener("click", function () {
          selectLine(diagnostic.line);
        });
      }
      item.appendChild(button);
      list.appendChild(item);
    });
    list.hidden = diagnostics.length === 0;
  }

  // -- worker ---------------------------------------------------------------

  function startWorker() {
    workerReady = false;
    updateButtons();
    try {
      worker = new Worker("worker.js");
    } catch (error) {
      setStatus("This browser cannot start a Web Worker.", "error");
      return;
    }
    worker.onmessage = onWorkerMessage;
    worker.onerror = function (event) {
      event.preventDefault();
      if (running) finishRun();
      workerReady = false;
      updateButtons();
      setStatus("Could not load the Mana WebAssembly module.", "error");
    };
  }

  function onWorkerMessage(event) {
    var message = event.data;
    switch (message.type) {
      case "ready":
        workerReady = true;
        version = message.version;
        if (!running) setStatus("Ready · Mana " + version);
        updateButtons();
        break;

      case "load-error":
        setStatus("Could not load the Mana WebAssembly module: " + message.message, "error");
        break;

      case "compiled":
        if (message.id !== runId) return;
        showDiagnostics(message.result.diagnostics);
        if (message.result.success) setStatus("Running…");
        break;

      case "output":
        if (message.id !== runId) return;
        appendOutput(message.chunks.map(function (chunk) {
          return { text: chunk.text, className: chunk.level === "info" ? null : "trace-" + chunk.level };
        }));
        break;

      case "done":
        if (message.id !== runId) return;
        finishRun();
        if (message.status === "finished") {
          setStatus("Finished in " + Math.max(1, Math.round(message.milliseconds)) + " ms");
        } else if (message.status === "compile-error") {
          setStatus("Compile failed", "error");
        } else {
          setStatus("The program could not start", "error");
        }
        break;
    }
  }

  function finishRun() {
    running = false;
    clearTimeout(timeLimit);
    timeLimit = null;
    updateButtons();
  }

  function run() {
    if (!workerReady || running) return;
    clearOutput();
    running = true;
    runId += 1;
    updateButtons();
    setStatus("Compiling…");
    var files = {};
    files[ENTRY] = elements.source.value;
    worker.postMessage({ type: "run", id: runId, entry: ENTRY, files: files });
    timeLimit = setTimeout(function () {
      stop("Stopped: the script ran for more than " + TIME_LIMIT_MILLISECONDS / 1000 + " seconds.");
    }, TIME_LIMIT_MILLISECONDS);
  }

  // Terminates the worker, whatever it is doing, and starts a new one.
  function stop(reason) {
    if (!running) return;
    finishRun();
    runId += 1;
    worker.terminate();
    appendNote(reason || "Stopped.");
    setStatus("Stopped · restarting…");
    startWorker();
  }

  // -- editor ---------------------------------------------------------------

  function updateGutter() {
    var lines = elements.source.value.split("\n").length;
    var numbers = [];
    for (var i = 1; i <= lines; i += 1) numbers.push(i);
    elements.gutter.textContent = numbers.join("\n");
    elements.gutter.scrollTop = elements.source.scrollTop;
  }

  function setSource(text) {
    elements.source.value = text;
    elements.source.scrollTop = 0;
    elements.source.scrollLeft = 0;
    updateGutter();
  }

  function selectLine(line) {
    var lines = elements.source.value.split("\n");
    var start = 0;
    for (var i = 0; i < line - 1 && i < lines.length; i += 1) start += lines[i].length + 1;
    var end = start + (lines[line - 1] || "").length;
    var textarea = elements.source;
    textarea.focus();
    textarea.setSelectionRange(start, end);
    var lineHeight = parseFloat(getComputedStyle(textarea).lineHeight) || 22;
    textarea.scrollTop = Math.max(0, (line - 3) * lineHeight);
    updateGutter();
  }

  function insertText(text) {
    var textarea = elements.source;
    // execCommand keeps the browser's undo history; setRangeText is the fallback.
    if (!document.execCommand || !document.execCommand("insertText", false, text)) {
      textarea.setRangeText(text, textarea.selectionStart, textarea.selectionEnd, "end");
      updateGutter();
    }
  }

  // Tab indents. Escape first lets the next Tab leave the editor as usual.
  var tabMovesFocus = false;
  elements.source.addEventListener("keydown", function (event) {
    if (event.key === "Enter" && (event.ctrlKey || event.metaKey)) {
      event.preventDefault();
      run();
      return;
    }
    if (event.key === "Escape") {
      tabMovesFocus = true;
      return;
    }
    if (event.key === "Tab" && !tabMovesFocus && !event.shiftKey && !event.altKey && !event.ctrlKey && !event.metaKey) {
      event.preventDefault();
      insertText("    ");
    }
    tabMovesFocus = false;
  });
  elements.source.addEventListener("input", updateGutter);
  elements.source.addEventListener("scroll", function () {
    elements.gutter.scrollTop = elements.source.scrollTop;
  });

  // -- examples -------------------------------------------------------------

  function markExample(name) {
    elements.examples.forEach(function (button) {
      button.setAttribute("aria-pressed", String(button.getAttribute("data-example") === name));
    });
  }

  function loadExample(name) {
    currentExample = name;
    markExample(name);
    if (exampleCache[name] !== undefined) {
      setSource(exampleCache[name]);
      return Promise.resolve();
    }
    return fetch("examples/" + name + ".mn")
      .then(function (response) {
        if (!response.ok) throw new Error(response.status + " " + response.statusText);
        return response.text();
      })
      .then(function (text) {
        exampleCache[name] = text;
        if (currentExample === name) setSource(text);
      })
      .catch(function (error) {
        setStatus("Could not load the " + name + " example: " + error.message, "error");
      });
  }

  elements.examples.forEach(function (button) {
    button.addEventListener("click", function () {
      loadExample(button.getAttribute("data-example"));
    });
  });

  // -- buttons --------------------------------------------------------------

  elements.run.addEventListener("click", run);
  elements.stop.addEventListener("click", function () {
    stop();
  });
  elements.reset.addEventListener("click", function () {
    stop();
    clearOutput();
    loadExample(currentExample);
    if (workerReady) setStatus("Ready · Mana " + version);
  });

  var requested = (location.hash || "").replace(/^#/, "");
  loadExample(EXAMPLES.indexOf(requested) >= 0 ? requested : DEFAULT_EXAMPLE);
  startWorker();
})();
