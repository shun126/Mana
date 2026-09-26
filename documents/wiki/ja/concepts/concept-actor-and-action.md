# Actor と Action の役割

Mana では、Actor と Action を分けて考えます。

Actor は **状態を持つ実行主体**、Action は **その Actor が実行できる行動**です。

```mana
actor Guard
{
    bool mAlert;

    action watch
    {
    }

    action move
    {
    }
}
```

この例では `Guard` が Actor、`watch` と `move` が Action です。

## Actor は状態を保持する

Actor のメンバー変数は、その Actor が持つ状態を表します。その Actor に属する Action から変数名を直接参照できます。

```mana
actor Guide
{
    int mTalkCount;

    action init
    {
        mTalkCount = 0;
    }
}
```

Action が終わっても Actor 自体は残るため、次に別の Action が実行されたときも状態を参照できます。

この性質により、例えば次を表現できます。

- 会話した回数
- 扉が開いているか
- NPC が警戒中か
- イベントがどこまで進んだか

## Action は Actor の外部から依頼できる行動

Action は Request の対象になります。

```mana
request(3, Guard->move);
```

ここで `Guard->move` は Action への参照です。

Action を Actor の中へ置くことで、「その行動は誰の責任か」がコード上でも明確になります。

```text
Guard
 ├─ watch
 ├─ move
 └─ talk
```

## Function との違い

Action と Function は、どちらも処理をまとめられますが役割が異なります。

Function は現在の処理から普通に呼び出し、終了すると呼び出し元へ戻ります。

```mana
int clampHp(int hp)
{
    if (hp < 0)
        return 0;
    return hp;
}
```

一方 Action は Actor の実行モデルに参加し、Request と Priority の影響を受けます。

```mana
request(5, Enemy->damage);
```

大まかには次のように分けられます。

| Function | Action |
| --- | --- |
| 計算や共通処理 | Actor の行動 |
| 通常の呼び出し | Request で依頼 |
| 呼び出し元へ戻る | Priority によって割り込み・保留される |
| Actor 外の処理にも使える | Actor に属する |

「HPを計算する」は Function、「敵がダメージを受ける」は Action、というように考えると整理しやすくなります。

## Action は一つずつしか存在しないのか

一つの Actor は複数の Action 定義を持てます。

さらに実行時には、Priority の異なる Request が同じ Actor に届くことがあります。

例えば、

```text
priority 1 : patrol
priority 5 : damage
```

という状態になれば、`damage` が `patrol` に割り込みます。

`damage` が終わった後、保存されていた `patrol` の実行位置へ戻って再開できます。

このため Action は単なる「Actor のメンバー関数」ではなく、**中断と再開を含む Actor の実行状態の単位**でもあります。

## 同じ Actor の状態を Action 間で共有する

Action は同じ Actor のメンバー変数へアクセスできます。

```mana
actor Door
{
    bool mOpened;

    action init
    {
        mOpened = false;
    }

    action open
    {
        if (mOpened)
            return;

        mOpened = true;
        print("Door opened\n");
    }
}
```

このように、Actor の状態と Action を近くに置くことで、「状態を持つもの」と「その状態を変える行動」を一つのまとまりとして記述できます。

## `init` と `main`

通常の Actor では、VM がプログラムをロードした後に `init` と `main` を自動的に Request します。

`init` は初期状態を設定する用途、`main` は Actor の基本動作を始める用途として使えます。

```mana
actor NPC
{
    int mState;

    action init
    {
        mState = 0;
    }

    action main
    {
        print("NPC started\n");
    }
}
```

ただし、すべての Actor に必ず両方を書く必要はありません。必要な Action だけを定義できます。

## Action の責務を大きくしすぎない

一つの Action にゲーム全体の処理を詰め込むと、Actor に分けた利点が薄れます。

例えば、

```text
EventController.main
    NPCの会話
    Doorのアニメーション
    Guardの移動
    SE再生
    状態保存
```

を全部直接行うより、

```text
EventController
    ├─ Guide->talk を Request
    ├─ Gate->open を Request
    └─ Guard->move を Request
```

と責務を分ける方が Mana の設計に合っています。

## Actor / Action を設計するときの考え方

まずゲーム上の責務を言葉にしてみます。

```text
誰が？        何をする？
Guide         talk
Gate          open
Guard         move
Event         start
```

この「誰が」を Actor、「何をする」を Action にすると、自然な構造になりやすくなります。

## ここまでで覚えておきたいこと

- Actor は状態を持つ実行主体
- Action は Actor が実行できる行動
- Action は Request の対象になる
- Function は通常処理、Action は Actor の実行モデルに参加する
- Action は Priority により中断・再開されることがある
- Actor のメンバー変数は複数 Action から共有できる

## 次に読む

Actor に Action を実行してもらう仕組みが Request です。次は Priority と組み合わせた動作を整理します。

[Request と Priority](./concept-request-and-priority.md)
