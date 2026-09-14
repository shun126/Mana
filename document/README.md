# Mana ドキュメント

Mana は、ゲーム内のキャラクターやイベントなど、複数の処理を協調させて記述するための Actor-oriented scripting language です。

このドキュメントは、Mana に初めて触れる人が「テキストでプログラムを書く」ところから始め、最終的に Actor / Action / Request を使ったゲームイベントを作れるようになることを目標にしています。

## はじめての方へ

次の順番で読むことをおすすめします。

1. [Mana とは](./getting-started/getting-started-what-is-mana.md)
2. [テキストでプログラムを書く](./getting-started/getting-started-programming-basics.md)
3. [Mana を使う準備](./getting-started/getting-started-installation.md)
4. [はじめての Mana プログラム](./getting-started/getting-started-first-program.md)
5. [コンパイルと実行](./getting-started/getting-started-compile-and-run.md)

## ドキュメントの構成

### Getting Started
Mana やプログラミングに慣れていない読者向けの入口です。ソースコード、コンパイル、VM といった言葉も初出時に説明します。

### Tutorial
実際に Mana のコードを書きながら、Actor、Action、Request、変数、条件分岐、同期などを順番に学びます。

### Concepts
Mana がどのような考え方で動いているかを説明します。Actor の実行モデル、Request と Priority、スケジューリングなどを扱います。

### Language Reference
構文、型、演算子、文、Actor、Action、Request、namespace、native などを正確に調べるための資料です。

### Integration
Mana Compiler と Mana VM を C++ アプリケーションやゲームエンジンへ組み込む開発者向けの資料です。

## 現在の状態

ドキュメントは全面改訂中です。まず日本語版を現行実装に合わせて整備し、内容が安定した後に英語版へ展開します。

従来の `primer_*.md` と `undocumented_features-*.md` は、移行が完了するまで資料として残します。
