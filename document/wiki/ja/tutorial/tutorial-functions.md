# 関数で処理をまとめる

鍵があと何個必要かを計算する処理に、名前を付けて使います。入力を受け取り、結果を返す処理を **関数（Function）**として定義できます。

## 動かして確かめる

**ファイル全体**です。Mana フォルダーへ `lesson.mn` として保存し、準備ページで設定したターミナルから `mana lesson.mn` を実行してください。前の章のコードへ追加せず、ファイル全体を置き換えます。

```mana
int remainingKeys(int required, int owned)
{
    if (owned >= required)
    {
        return 0;
    }
    return required - owned;
}

actor Event
{
    action main
    {
        int missing = remainingKeys(3, 1);
        print("Missing keys: %d\n", missing);
    }
}
```

**期待する出力：**

```text
Missing keys: 2
```

[同梱の完成コード](../../../../examples/tutorial/08-functions.mn)は、Mana フォルダーから次のコマンドでも実行できます。

```text
mana examples/tutorial/08-functions.mn
```


## 入力と結果を読む

`int remainingKeys(int required, int owned)` を分解すると、次の意味になります。

| 部分 | 意味 |
| --- | --- |
| 最初の `int` | 結果として整数を返す |
| `remainingKeys` | 関数の名前 |
| `int required` | 必要数を受け取る引数 |
| `int owned` | 所持数を受け取る引数 |

`remainingKeys(3, 1)` と呼ぶと、`required` に3、`owned` に1を渡します。`return` は計算結果を返し、その呼び出しを終了します。今回は `3 - 1` の結果である2が返り、`missing` に入ります。

必要数以上の鍵を持つ場合は、先に `return 0;` へ到達するので、下の引き算は実行しません。

## 一つ変えてみる

呼び出しを `remainingKeys(3, 5)` に変更してください。出力は `Missing keys: 0` です。

前のイベントへこの関数を追加するなら、`remainingKeys(3, 5) == 0` を `if` の条件として使い、必要数を持っているときに門を開けられます。

## 関数と Action を使い分ける

関数は、呼び出した処理の中で計算などを行い、結果を呼び出し元へ返します。Action は Actor の行動で、Request の対象になります。

| 目的 | この教材での書き方 |
| --- | --- |
| 必要な鍵の数を計算する | `remainingKeys(3, 1)` |
| 門に開く行動を依頼する | `awaitCompletion(10, Gate->open)` |

## 値を返さない関数

**Actor の外へ追加する定義例：**

```mana
void printSeparator()
{
    print("-----\n");
}
```

`void` は結果の値を返さないことを表します。Action 内で `printSeparator();` と書くと呼び出せます。

C++ 側に実装する `native` 関数は、[組み込みガイド](../integration/integration-native-functions.md)で扱います。Mana で宣言するだけでは、対応するゲーム側の処理は作られません。
## 次に読む

[Priority で割り込みと復帰を扱う](./tutorial-priority.md)へ進みます。
