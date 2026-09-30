/*
 * Mana Playground runtime.
 *
 * A thin JavaScript wrapper over the WebAssembly bridge in web/cpp/ManaWeb.cpp.
 * The compiler and the VM inside mana.wasm are the same ones the desktop
 * build uses; this file only moves strings in and out of the module.
 *
 * The Web Worker loads it with importScripts(); the tests require() it.
 */
(function (root, factory) {
  "use strict";
  if (typeof module === "object" && module.exports) {
    module.exports = factory();
  } else {
    root.ManaRuntime = factory();
  }
})(typeof self !== "undefined" ? self : this, function () {
  "use strict";

  // mana::TraceLevel
  var TRACE_LEVELS = ["info", "warning", "error", "debug"];

  function ManaRuntime(module) {
    this.module = module;
  }

  /*
   * Instantiates mana.wasm.
   *
   * createManaModule: the factory mana.js defines.
   * onOutput(level, text): receives print() and VM traces. level is one of
   *   TRACE_LEVELS. Text arrives in pieces, not necessarily whole lines.
   */
  ManaRuntime.create = function (createManaModule, onOutput) {
    return createManaModule({
      onOutput: function (level, text) {
        if (onOutput) onOutput(TRACE_LEVELS[level] || "info", text);
      },
    }).then(function (module) {
      return new ManaRuntime(module);
    });
  };

  ManaRuntime.prototype.withString = function (text, use) {
    var pointer = this.module.stringToNewUTF8(text);
    try {
      return use(pointer);
    } finally {
      this.module._free(pointer);
    }
  };

  // The Mana version the module was built from, e.g. "0.10.0".
  ManaRuntime.prototype.version = function () {
    return this.module.UTF8ToString(this.module._mana_web_version());
  };

  // Forgets every source, the compiled program and the VM.
  ManaRuntime.prototype.reset = function () {
    this.module._mana_web_reset();
  };

  // Adds or replaces a source file the compiler can read.
  ManaRuntime.prototype.setFile = function (filename, text) {
    var self = this;
    self.withString(filename, function (name) {
      self.withString(text, function (source) {
        self.module._mana_web_set_file(name, source);
      });
    });
  };

  /*
   * Compiles the sources added with setFile(), starting from entry.
   * Returns { success, programSize, diagnostics: [{ severity, phase,
   * filename, line, message }] }. A successful program is kept for start().
   */
  ManaRuntime.prototype.compile = function (entry) {
    var self = this;
    var json = self.withString(entry, function (name) {
      return self.module.UTF8ToString(self.module._mana_web_compile(name));
    });
    return JSON.parse(json);
  };

  // Loads the compiled program into a new VM. Returns false on failure.
  ManaRuntime.prototype.start = function () {
    return this.module._mana_web_start() !== 0;
  };

  // Runs the VM for about budgetMilliseconds. Returns true while it still runs.
  ManaRuntime.prototype.step = function (budgetMilliseconds) {
    return this.module._mana_web_step(budgetMilliseconds) !== 0;
  };

  // Discards the running VM.
  ManaRuntime.prototype.stop = function () {
    this.module._mana_web_stop();
  };

  // "main.mn:12: error: message", the way the Playground shows a diagnostic.
  ManaRuntime.formatDiagnostic = function (diagnostic) {
    var where = diagnostic.filename || "";
    if (diagnostic.line > 0) where += ":" + diagnostic.line;
    var message = String(diagnostic.message).replace(/\s+$/, "");
    return (where ? where + ": " : "") + diagnostic.severity + ": " + message;
  };

  ManaRuntime.TRACE_LEVELS = TRACE_LEVELS;

  return ManaRuntime;
});
