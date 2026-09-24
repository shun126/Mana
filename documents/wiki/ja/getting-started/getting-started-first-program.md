# はじめての Mana プログラム

文字を表示し、その文字を自分で変更します。[準備ページ](./getting-started-installation.md)で設定したターミナルを使ってください。

## 1. ソースファイルを作る

エディタに次の**ファイル全体**を入力し、Mana フォルダーへ `hello.mn` として保存します。UTF-8 のプレーンテキストで保存してください。

```mana
actor Hello
{
    action main
    {
        print("Hello, Mana!\n");
    }
}
```

## 2. 実行する

Mana フォルダーのターミナルへ、次を入力します。

```text
mana hello.mn
```

**期待する出力：**

```text
Hello, Mana!
```

表示できれば成功です。うまくいかない場合は、`hello.mn.txt` になっていないか、ファイルを保存したか、ターミナルの作業場所が Mana フォルダーかを確認します。

入力例と比較したいときは、[同梱の完成コード](../../../../examples/tutorial/01-hello.mn)も使えます。

```text
mana examples/tutorial/01-hello.mn
```

## 3. コードを読む

`actor Hello` は、`Hello` という名前の Actor を定義します。`Hello` は自分で付けた名前です。

その中の `action main` は、Actor が行う処理を定義します。`main` は起動時に実行される特別な Action 名です。今は、この中へ最初に動かす処理を書くと覚えてください。

`print("Hello, Mana!\n");` は文字を出力する文です。ここまでの例では、画面への出力はターミナルに表示されます。

`{` と `}` の対応をたどると、`print` が `main` の中にあり、`main` が `Hello` の中にあることが分かります。

## 4. 変更して結果を予想する

`print` の行を、次の2行に**置き換えて**ください。

```mana
print("Welcome!\n");
print("The gate is closed.\n");
```

保存し、もう一度 `mana hello.mn` を実行します。

```text
Welcome!
The gate is closed.
```

この Action の中では、上から順に文が実行されます。2行の順番を入れ替えると、出力も入れ替わります。

次は [コンパイルと実行・エラーの直し方](./getting-started-compile-and-run.md) で、実行までの仕組みと、間違えたときの調べ方を学びます。
