# Actor

`actor` は、Mana で独立して Action を実行する基本的な実行単位です。

Actor は状態を保持し、複数の Action を持ち、他の Actor から Request を受け取れます。

## 構文

```mana
actor ActorName
{
    members
}
```

例:

```mana
actor NPC
{
    int mTalkCount;

    action talk
    {
        mTalkCount++;
        print("Hello\n");
    }
}
```

## Actor のメンバー

Actor の中には、主に次のものを書けます。

- Action
- 変数
- 定数
- `extend` による Module の取り込み

例:

```mana
actor Guard
{
    int mAlertLevel;
    const int kMaxAlert = 3;

    action patrol
    {
    }
}
```

Actor のメンバー変数は Actor の状態として保持され、Action が終了しても値は残ります。

## VM 起動時の生成

通常の `actor` は Program Image のロード時に Mana VM がインスタンス化します。

これは `phantom` との大きな違いです。Phantom はロード時にはインスタンス化されず、C++ 側から明示的に生成します。

## `init` と `main`

Mana VM はプログラムをロードした後、Actor に対して特別な Action を要求します。

現行VMでは、

1. `init` を Priority 1 で Request
2. `main` を Priority 0 で Request

の順で全 Actor に送ります。

Actor がその Action を定義していない場合、その Request は実行されません。

```mana
actor Example
{
    action init
    {
        print("init\n");
    }

    action main
    {
        print("main\n");
    }
}
```

`init` は状態の初期化、`main` は通常の開始処理として使えます。

## Actor 型

`actor` は Actor への参照を保持する型としても使用できます。

```mana
actor target;
```

Actor 参照に対して Action を指定できます。

```mana
request(1, target->talk);
```

Action 参照の詳細は [Request](./reference-request.md) を参照してください。

## Namespace

Actor は namespace 内に定義できます。

```mana
namespace Game::NPC
{
    actor Shopkeeper
    {
        action talk
        {
        }
    }
}
```

完全修飾名では `Game::NPC::Shopkeeper` となります。

## Actor はキャラクターに限定されない

Actor は実行主体であり、ゲームキャラクター専用の概念ではありません。

例えば次のような役割にも使用できます。

- イベント進行
- UI 制御
- ギミック
- シーン管理
- バトル進行
- エフェクト制御

## 関連項目

- [Action](./reference-action.md)
- [Request](./reference-request.md)
- [実行制御](./reference-execution-control.md)
- [Module](../concepts/concept-module.md)
- [Phantom](../concepts/concept-phantom.md)
