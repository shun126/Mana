# コンパイルと実行

前のページでは、次のコマンドで Mana のソースファイルを実行しました。

```bash
mana hello.mn
```

このとき `mana` コマンドは、ソースコードをコンパイルし、その結果を Mana VM で実行します。

## ソースコードから実行まで

処理の流れを分けて考えると、次のようになります。

```text
hello.mn
   |
   v
Mana Compiler
   |
   v
Program Image
   |
   v
Mana VM
   |
   v
プログラムの実行結果
```

ここで重要なのは、Mana の「コンパイル」が CPU が直接実行するネイティブコードを作ることを意味するわけではないことです。Mana Compiler は、Mana VM が読み込んで実行する Program Image を生成します。

## コンパイルしてすぐ実行する

通常はソースファイルを指定するだけで、コンパイルと実行を続けて行えます。

```bash
mana source_file.mn
```

チュートリアルでは、主にこの方法を使用します。

## Program Image をファイルへ保存する

`-o` オプションを指定すると、コンパイル結果をファイルへ出力できます。

```bash
mana source_file.mn -o program_file
```

ゲームへの組み込みや、コンパイルと実行を別工程にしたい場合に利用できます。

## コンパイル済みファイルを実行する

保存した Program Image は `--execute` または `-e` を使って実行できます。

```bash
mana --execute program_file
```

または、

```bash
mana -e program_file
```

## コンパイルエラー

Mana Compiler がソースコードに問題を見つけた場合は、エラー内容を表示してコンパイルを中止します。

例えば、`;` を書き忘れたり、存在しない名前を参照したりするとコンパイル時に問題を発見できる場合があります。

エラーメッセージを確認してソースコードを修正し、もう一度コンパイルしてください。

## ここまでで覚えておきたいこと

- Mana のプログラムはテキストとして書く
- ソースファイルには通常 `.mn` を使う
- Mana Compiler がソースコードを Program Image へ変換する
- Mana VM が Program Image を実行する
- `mana source_file.mn` ではコンパイルと実行を続けて行える

ここまで理解できれば、Mana の言語機能を学ぶ準備は完了です。

次は Tutorial で Actor と Action から学びます。
