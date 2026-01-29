# Mana Script Coding Convention v0.1

## 0. Purpose

This convention aims to:

* Clearly define **actor boundaries** (state vs responsibility)
* Make **message flow via `request`** easy to follow
* Reduce **misinterpretation during reviews and maintenance**

This is a **project-level convention**, not a language restriction.

---

## 1. Formatting

### 1.1 Indentation & Line Length

* Indentation: 4 spaces or tabs
* Tabs are forbidden
* Line length guideline: **120 characters**
  * Break lines when exceeded

### 1.2 Braces

* **Allman style**

```mn
actor Foo
{
    action bar
    {
    }
}
```

---

## 2. Files & Structure

### 2.1 File Naming

* Extension: `.mn`
* File name: `PascalCase`
  Example: `EnemyAI.mn`

### 2.2 namespace

* **Not mandatory, but recommended**
* Strongly recommended for:

  * Large projects
  * Libraries / plugins
  * Long-term maintenance

```mn
namespace Game::AI;
```

### 2.3 using

* If used, place all `using` declarations:

  * At the top of the file
  * Immediately after `namespace`
* Avoid overuse to keep dependencies explicit

---

## 3. Naming Rules (Critical)

## 3.1 actor / action / variables

### actor Names

* `PascalCase`
* Use nouns that clearly describe responsibility
  Examples: `EnemyController`, `BattleService`

### action Names

* `lowerCamelCase`
* **Verb-first naming is recommended**
* **The convention does not define or restrict `onXxx` naming**

Recommended examples:

* `initialize`, `main`, `think`, `update`
* `attack`, `acquireTarget`

### Local Variables

* `lowerCamelCase`
  Example: `targetId`, `retryCount`

### actor State (Fields)

* `mLowerCamelCase`
  Example: `mState`, `mTargetId`

---

## 3.2 Constants

### Naming

* **All constants must use the `k` prefix**
* Format: `kPascalCase`
* `const` declaration is mandatory

```mn
const int kMaxRetry = 3;
const int kThinkPriority = 50;
```

### Semantic Definition

> `kXxx` represents **immutable rules, constraints, thresholds, or default values**
> They are **not actor state** and must never be modified.

---

## 3.3 Global Variables (Mutable)

### Policy

* **Prohibited by default**
* Allowed only as explicit exceptions

### Naming

* **`g` prefix is mandatory**
* Format: `gLowerCamelCase`

```mn
int  gFrameCounter;
bool gIsDebugMode;
```

### Required Constraints

When a `gXxx` variable is used:

* The **owner** (actor / action responsible for writing) must be documented
* Writes should be limited to as few locations as possible (ideally one)
* Must not be used for core gameplay logic
  (debugging, diagnostics, statistics only)

```mn
// Owned by DebugController::update
bool gIsDebugDrawEnabled;
```

---

## 4. request (Message Sending)

## 4.1 First Argument (Fixed Meaning)

> In `request(priority, Target->action)`,
> **the first argument always represents priority**

* It does **not** represent delay, frame count, or time

```mn
request(100, Enemy->think);
request(10,  Enemy->idle);
```

---

## 4.2 Priority Values

* Avoid magic numbers
* **Prefer constants (`kXxx`)**

```mn
const int kThinkPriority = 50;
request(kThinkPriority, Enemy->think);
```

---

## 4.3 Formatting (Long Calls)

```mn
request(
    kThinkPriority,
    Enemy->think
);
```

---

## 5. action Granularity (Design Rule)

* **One action = one intent**
* `main` should act as an orchestrator

  * Avoid embedding complex logic directly
* If request chains grow long:

  * Split into multiple actions
  * Use phased execution

---

## 6. Comments

* Comments should primarily explain **“why”**, not “what”
* For actors, short comments describing:

  * Responsibility
  * Meaning of held state
  * Key outgoing requests
    are strongly recommended
