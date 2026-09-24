# Mana 言語リファレンス

このセクションは、Mana の構文や言語機能を正確に調べるためのリファレンスです。

Tutorial は「順番に学ぶ」ため、Concepts は「なぜそう動くのか」を理解するための資料です。Language Reference では、用途ごとに構文・制約・例を確認できることを優先します。

## 基礎

1. [ソースコードの構造](./reference-source-code.md)
2. [型](./reference-types.md)
3. [変数](./reference-variables.md)
4. [定数](./reference-constants.md)
5. [式](./reference-expressions.md)
6. [演算子](./reference-operators.md)
7. [文](./reference-statements.md)

## Function とデータ型

8. [Function](./reference-functions.md)
9. [Struct](./reference-struct.md)

## Actor 実行モデル

10. [Actor](./reference-actor.md)
11. [Action](./reference-action.md)
12. [Request](./reference-request.md)
13. [実行制御](./reference-execution-control.md)

## 構成と連携

14. [Module](./reference-module.md)
15. [Phantom](./reference-phantom.md)
16. [Namespace と using](./reference-namespace.md)
17. [Native Function](./reference-native.md)
18. [ソースファイルと import / include](./reference-source-files.md)
19. [定義済みシンボル](./reference-predefined-symbols.md)
20. [CLI](./reference-cli.md)

## 記述方針

各ページでは、可能な限り次の順で説明します。

1. 概要
2. 構文
3. 動作
4. 制約
5. 例
6. 関連項目

このリファレンスは現行コンパイラ、VM、テストを基準にします。旧 Primer に残っている古い構文より、現在の実装を優先します。
