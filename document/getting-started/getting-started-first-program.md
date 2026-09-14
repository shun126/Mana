# はじめての Mana プログラム

ここでは、実際に小さな Mana プログラムを書いて実行します。

## ソースファイルを作る

テキストエディタを開き、次の内容を入力してください。

```mana
actor Hello
{
    action main
    {
        print("Hello, Mana!\n");
    }
}
```

`hello.mn` という名前で保存します。

## 実行する

`hello.mn` を保存したディレクトリで、次のコマンドを実行します。

```bash
mana hello.mn
```

正常にコンパイルされて実行されると、次の文字が表示されます。

```text
Hello, Mana!
```

## コードを読んでみる

最初からすべての構文を覚える必要はありません。ここでは大まかな役割だけ確認します。

```mana
actor Hello
```

`actor` は Mana の基本的な実行単位を定義します。この例では `Hello` という名前を付けています。

```mana
action main
```

`action` は Actor が行う処理を定義します。`main` は Actor の通常の実行で使われる Action です。

```mana
print("Hello, Mana!\n");
```

`print` は文字を出力します。末尾の `;` は一つの文の終わりを表します。文字列中の `\n` は改行を表します。

## 少し変更してみる

表示する文字を好きな内容へ変更し、もう一度実行してみてください。

```mana
print("Welcome to Mana!\n");
```

Mana の基本的な開発は、「ソースコードを変更する → コンパイルして実行する → 結果を確認する」という繰り返しです。

## 次に読む

次は [コンパイルと実行](./getting-started-compile-and-run.md) で、`mana` コマンドが内部で何をしているのかを説明します。
