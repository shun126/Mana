# 処理を繰り返す

3、2、1と表示してから門を開きます。同じ形の処理を繰り返すために `while` を使います。

## 動かして確かめる

**ファイル全体**です。Mana フォルダーへ `lesson.mn` として保存し、準備ページで設定したターミナルから `mana lesson.mn` を実行してください。前の章のコードへ追加せず、ファイル全体を置き換えます。

```mana
actor Event
{
    action main
    {
        int remaining = 3;
        while (remaining > 0)
        {
            print("Remaining: %d\n", remaining);
            remaining = remaining - 1;
        }
        awaitCompletion(10, Gate->open);
    }
}

actor Gate
{
    action open
    {
        print("Gate: Open.\n");
    }
}
```

**期待する出力：**

```text
Remaining: 3
Remaining: 2
Remaining: 1
Gate: Open.
```

[同梱の完成コード](../../../../examples/tutorial/07-loops.mn)は、Mana フォルダーから次のコマンドでも実行できます。

```text
mana examples/tutorial/07-loops.mn
```


## while は毎回、条件を先に調べる

`while (remaining > 0)` は、残りが0より大きい間、ブロックを繰り返します。毎回1を引くので、やがて0になり、繰り返しを抜けます。

| 条件を調べるときの値 | 行うこと |
| --- | --- |
| 3 | 3を表示し、2へ変更 |
| 2 | 2を表示し、1へ変更 |
| 1 | 1を表示し、0へ変更 |
| 0 | ブロックへ入らず、門を開ける処理へ進む |

これは秒数のカウントダウンではありません。繰り返し自体には、時間を待つ機能はありません。

## 一つ変えてみる

最初の `remaining` を5にすると、5から1まで表示します。0にすると、数字を一度も表示せず門を開きます。実行前に結果を予想してください。

## 回数を指定する for

**完成コードの main 全体を置き換える例：**

```mana
for (int i = 0; i < 3; i++)
{
    print("Step: %d\n", i);
}
awaitCompletion(10, Gate->open);
```

`for` の括弧内は「最初に行う処理; 続ける条件; 1回終わるたびに行う処理」です。`i++` は値を1増やします。ここでは0、1、2を表示し、門を開きます。

## 終わらないときは止めて調べる

完成コードから `remaining = remaining - 1;` を消すと、条件が変わらず、繰り返しが終わりません。このような状態を **無限ループ**と呼びます。試す必要はありませんが、誤って起きたらターミナルで Ctrl+C を押して止めてください。

「他の Actor がいつか変数を変えるだろう」と考えて待ち続けるだけのループにも注意が必要です。Mana では他の処理へ実行機会を渡す仕組みが関係します。[待機と同期](./tutorial-wait-and-synchronization.md)で `yield()` を学びます。

## 必要になったら調べるもの

`break` は繰り返しを途中で終了し、`continue` はその回の残りを飛ばします。`do-while` は少なくとも1回処理してから条件を調べます。

専用の `loop` 構文も含め、詳しい使い方は [文リファレンス](../reference/reference-statements.md)へ進んでください。まずは、終了条件を持つ `while` と `for` を使えれば十分です。
## 次に読む

[関数で処理をまとめる](./tutorial-functions.md)へ進みます。
