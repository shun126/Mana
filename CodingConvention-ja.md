# Mana スクリプト コーディング規約 v0.1

## 0. 目的

* **actor 境界**（状態と責務）を明確にする
* **request の流れ**（誰が誰に何を依頼したか）を追いやすくする
* レビュー時に **意図の誤読**を減らす

これは**プロジェクトレベルの規約**であり、言語の制約ではありません。

---

## 1. フォーマット

### 1.1 インデント・改行

* インデント：4スペースまたはタブ
* タブ禁止
* 1行の目安：120文字（超える場合は適切に改行）

### 1.2 ブレース

* **Allman スタイル**

```mn
actor Foo
{
    action bar
    {
    }
}
```

---

## 2. ファイルと構造

### 2.1 ファイル命名

* 拡張子：`.mn`
* ファイル名：`PascalCase`（例：`EnemyAI.mn`）

### 2.2 namespace

* **必須ではない（推奨）**
* 大規模化・ライブラリ化・プラグイン化を見据える場合は特に推奨

```mn
namespace Game::AI;
```

### 2.3 using

* 使用する場合は、**ファイル先頭（namespace の直後）に集約**
* 乱用しない（参照関係が読めなくなる）

---

## 3. 命名規則（最重要）

## 3.1 actor / action / 変数

### actor 名

* `PascalCase`
* 役割が分かる名詞（例：`EnemyController`, `BattleService`）

### action 名

* `lowerCamelCase`
* **動詞から始める名前を推奨**
* **`onXxx` という命名には触れない**（規約として言及しない）

推奨例：

* `initialize`, `main`, `think`, `update`, `attack`, `acquireTarget`

### ローカル変数

* `lowerCamelCase`（例：`targetId`, `retryCount`）

### actor 状態（フィールド）

* `mLowerCamelCase`（例：`mState`, `mTargetId`）

---

## 3.2 定数（Constant）

### 命名

* **定数は必ず `k` 接頭辞**
* 形式：`kPascalCase`
* `const` 宣言を必須

```mn
const int kMaxRetry = 3;
const int kThinkPriority = 50;
```

### 意味定義

> `kXxx` は **不変のルール・制約・閾値・デフォルト値**を表す
> （actor の状態ではない・書き換えない）

---

## 3.3 グローバル変数（可変）

### 方針

* **原則禁止**
* どうしても必要な場合のみ例外として許可し、命名で明示する

### 命名

* **`g` 接頭辞**
* 形式：`gLowerCamelCase`

```mn
int  gFrameCounter;
bool gIsDebugMode;
```

### 制約（セット運用）

* `gXxx` には **所有者（書き込み責務を持つ actor/action）をコメントで明記**
* 書き込み箇所を極力 1 箇所に限定（分散させない）
* gameplay の中核ロジックでは使用しない（デバッグ・統計・一時的制御用途に限定）

```mn
// Owned by DebugController::update
bool gIsDebugDrawEnabled;
```

---

## 4. request（メッセージ送信）

## 4.1 第1引数の意味（確定）

> `request(priority, Target->action)` の第1引数は **常に優先度（priority）**

* 遅延時間・フレーム数・秒数ではない

```mn
request(100, Enemy->think);
request(10,  Enemy->idle);
```

## 4.2 優先度の表現

* マジックナンバーを避け、**`kXxx` 定数化を推奨**

```mn
const int kThinkPriority = 50;
request(kThinkPriority, Enemy->think);
```

## 4.3 整形（長い場合）

```mn
request(
    kThinkPriority,
    Enemy->think
);
```

---

## 5. action の粒度（設計ルール）

* **1 action = 1 intent**（目的が1つになるよう分割）
* `main` はオーケストレーション寄りにして、内部ロジックを詰め込みすぎない
* request 連鎖が長くなる場合は action を分割してフェーズ化する

---

## 6. コメント

* コメントは主に **「なぜ」**を書く（何をしているかはコードで読む）
* actor には役割・保持状態の意味・主要な request 先を短く書くとレビューが速い
