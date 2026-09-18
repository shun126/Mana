# Action を Request する

進行役の `Event` から、案内役の `Guide` に会話を依頼します。依頼するための命令が `request` です。

![Request の流れ](../../../assets/ja/diagrams/request-flow.svg)

## 動かして確かめる

**ファイル全体**です。Mana フォルダーへ `lesson.mn` として保存し、準備ページで設定したターミナルから `mana lesson.mn` を実行してください。前の章のコードへ追加せず、ファイル全体を置き換えます。

```mana
actor Event
{
    action main
    {
        print("Event: Request.\n");
        request(10, Guide->talk);
    }
}

actor Guide
{
    action talk
    {
        print("Guide: Welcome!\n");
    }
}
```

**期待する出力：**

```text
Event: Request.
Guide: Welcome!
```

[同梱の完成コード](../../../../examples/tutorial/03-request.mn)は、Mana フォルダーから次のコマンドでも実行できます。

```text
mana examples/tutorial/03-request.mn
```


## 依頼する相手と行動を指定する

**上のコードからの抜粋：**

```mana
request(10, Guide->talk);
```

括弧の中で命令へ渡す値を **引数**と呼びます。複数の引数は `,` で区切ります。

| 部分 | 意味 |
| --- | --- |
| `10` | Priority（優先度）。数が大きい方が高い |
| `Guide->talk` | `Guide` の `talk` という Action |
| `->` | 左の Actor が持つ、右の Action を指し示す記号 |

`10` は秒数でも繰り返し回数でもありません。この教材ではまず `10` を使い、優先度による割り込みは後の章で学びます。

`Guide` の定義が `Event` より後にあっても、この例はコンパイルできます。コンパイラはプログラム全体から名前を調べます。

## request は完了を待たない

`request` は相手へ依頼を出し、その完了を待たずに呼び出し側の処理を続けます。次の行へ進んだことは、相手の Action が終わった証明にはなりません。

また、一つの Actor で同じ Priority が使用中・予約済みの場合、その Priority の新しい依頼は受理されません。**request を二つ並べても、二つの仕事が必ず順番に実行されるわけではありません。** 詳細は [Priority](./tutorial-priority.md)で確認します。

会話が終わってから門を開けるには、完了を待つ命令が必要です。次の章で使います。

## 一つ変えてみる

`talk` の名前を `greet` に変更してください。定義の `action talk` と、依頼先の `Guide->talk` の両方を変更すると、同じ出力になります。

片方だけ変更すると、存在しない名前を参照することになります。コンパイラの診断を読み、名前をそろえて直してみてください。
## 次に読む

[会話して門を開ける](./tutorial-small-event.md)へ進みます。
