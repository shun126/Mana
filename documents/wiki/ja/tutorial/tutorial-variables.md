# 変数で状態を覚える

案内役へ2回話しかけ、会話回数を表示します。値を保存するために **変数**を使います。

## 動かして確かめる

次は**ファイル全体**です。Mana フォルダーの `lesson.mn` を置き換えて保存し、`mana lesson.mn` で実行してください。

```mana
actor Event
{
    action main
    {
        awaitCompletion(10, Guide->talk);
        awaitCompletion(10, Guide->talk);
    }
}

actor Guide
{
    int mTalkCount;

    action init
    {
        mTalkCount = 0;
    }

    action talk
    {
        mTalkCount = mTalkCount + 1;
        print("Talk count: %d\n", mTalkCount);
    }
}
```

**期待する出力：**

```text
Talk count: 1
Talk count: 2
```

[同梱の完成コード](../../../../examples/tutorial/05-variables.mn)は、`mana examples/tutorial/05-variables.mn` でも実行できます。

## 型、名前、値

`int mTalkCount;` は、整数を保存する変数を宣言しています。`int` は扱う値の種類である **型**、`mTalkCount` は変数名です。`init` Action で最初の値を0にしています。

`mTalkCount = mTalkCount + 1;` は、現在の値に1を足してから保存する **代入**です。数学の等式とは違い、右側で計算した値を左側へ入れる操作です。1回目は0から1、2回目は1から2になります。

## 値を表示する

`print("Talk count: %d\n", mTalkCount);` の `%d` は、後ろに渡した整数値を表示する位置を表します。`\n` は改行です。

変数の値を表示すると、どこまで処理が進み、値がどう変わったかを確認できます。

## Action をまたいで値を覚える

今回の変数は `Guide` の中、Action の外に宣言した **Actor 変数**です。`Guide` に属する Action から直接参照でき、他の Actor から直接変更されることはありません。

`m` で始める名前は Actor のメンバーだと分かりやすくする約束で、言語の必須条件ではありません。

この変数は会話の Action が終わっても残ります。ただし、プログラムを終了して起動し直すと、また0から始まります。ファイルにセーブデータを保存したわけではありません。

状態を `Guide` に持たせることで、その状態を使う Action と一緒にカプセル化できます。詳しくは [変数リファレンス](../reference/reference-variables.md)を参照してください。

## その処理の中で使うローカル変数

Action 内に宣言する変数は、その処理で使う **ローカル変数**です。

**talk の中身を置き換える例：**

```mana
int count = 0;
count = count + 1;
print("Talk count: %d\n", count);
```

この場合は、会話のたびに `count = 0` から始まるので、2回とも1を表示します。

## よく使う型

以下は **Action 内に書く宣言の例**です。

```mana
int count = 3;
float distance = 1.5;
bool hasKey = true;
string message = "Welcome!";
```

| 型 | 保存する値 |
| --- | --- |
| `int` | 整数 |
| `float` | 小数を含む数 |
| `bool` | `true` または `false` の二つの値 |
| `string` | 文字列 |

## 一つ変えてみる

完成コードへ戻し、`Event` に3回目の会話を依頼する行を追加してください。`Talk count: 3` まで表示できれば、状態の保持を確認できています。

## 次に読む

[条件によって処理を変える](./tutorial-conditions.md)で、値に応じた分岐を作ります。
