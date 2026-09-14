# Mana を使う準備

Mana のソースコードをコンパイルして実行するには、コマンドラインツール `mana` を用意します。

このページでは、GitHub の Mana リポジトリからビルドする方法を説明します。

## 必要なもの

Mana のビルドには次のツールを使用します。

- C++ コンパイラ
- Bison 3.8
- Flex 2.6.4

Windows では Visual Studio、Linux では一般的な C++ 開発環境を利用できます。

## ソースコードを取得する

Git を利用する場合は、次のようにリポジトリを取得できます。

```bash
git clone https://github.com/shun126/Mana.git
cd Mana
```

Git を利用しない場合は、GitHub からソースコードを取得して展開しても構いません。

## Linux でビルドする

必要な開発ツールを準備したあと、リポジトリのルートディレクトリで `make` を実行します。

```bash
make
```

## Visual Studio でビルドする

Windows では `mana.sln` を Visual Studio で開いてビルドできます。

Bison と Flex を使用するため、環境変数 `GNU_BISON_BIN` と `GNU_FLEX_BIN` にそれぞれの実行ファイルがあるディレクトリを設定します。

現在のリポジトリでは Visual Studio 2022 を開発環境として利用できます。

## 動作を確認する

ビルドした `mana` コマンドを実行できる状態にしたら、リポジトリに含まれているサンプルを実行して確認できます。

```bash
mana sample/Sample.mn
```

`mana` が見つからない場合は、ビルドされた実行ファイルの場所を確認し、そのディレクトリから実行するか PATH を設定してください。

## 次に読む

準備ができたら [はじめての Mana プログラム](./getting-started-first-program.md) へ進みます。
