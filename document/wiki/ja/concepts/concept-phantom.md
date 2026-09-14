# Phantom

`phantom` は、実行時に必要なタイミングで Actor を生成するための **Actor 定義のテンプレート** です。

構文は Actor とよく似ています。

```mana
phantom EnemyTemplate
{
    action appear
    {
        print("Enemy appeared\n");
    }
}
```

ただし、`actor` と `phantom` では VM にロードされたときの扱いが異なります。

## Actor は起動時に生成される

通常の `actor` は、Program Image を VM にロードしたときにインスタンス化され、VM の Actor 一覧へ登録されます。

```mana
actor Guide
{
    action main
    {
    }
}
```

`Guide` はプログラムの常駐する実行主体として扱われます。

## Phantom は起動時に生成されない

`phantom` は VM 起動時には Actor インスタンスを作りません。

VM は Phantom の定義情報を保持し、C++ 側から必要になったときに生成できます。

概念的には次の流れです。

```text
Mana source
    |
    | phantom EnemyTemplate
    v
Program Image
    |
    v
Mana VM
    |
    | 定義だけ保持
    |
    | C++: CreateActorFromPhantom(...)
    v
実行時 Actor
```

## C++ から生成する

現行 VM には、Phantom から Actor を生成するための API があります。

```cpp
auto enemy = vm->CreateActorFromPhantom("EnemyTemplate", "Enemy01");
```

第1引数は Phantom の定義名、第2引数は生成する Actor の名前です。

これにより、同じ定義から複数の実行時 Actor を作るような用途へ発展させられます。

## どのような場面で使うか

例えば、ゲーム中に必要になったタイミングで生成されるものが考えられます。

- 敵キャラクター
- 一時的なイベント Actor
- 動的に配置されるギミック
- スポーンされる NPC

起動時から常に存在する必要がない実行主体を、あらかじめ Mana 側で定義しておく場合に適しています。

## Actor と Phantom の違い

| | Actor | Phantom |
| --- | --- | --- |
| 定義に Action を持てる | はい | はい |
| VM ロード時に自動生成 | はい | いいえ |
| 通常の起動時実行対象 | はい | いいえ |
| C++ から必要時に生成 | 必須ではない | 主な用途 |
| 主な目的 | 常駐する実行主体 | 動的生成用テンプレート |

## `init` / `main` をどう考えるか

Phantom は定義しただけではインスタンスが存在しないため、通常の Actor のようにロード直後から `init` / `main` が実行されるわけではありません。

生成後の詳細なライフサイクルや C++ API の使い方は Integration で扱います。

## スクリプトから直接生成する機能

現行仕様では、Mana スクリプトから `phantom` を直接インスタンス化するための構文は定義されていません。

Phantom の生成は C++ 側の VM API が担当します。

この点は、Phantom が Mana とゲームエンジン側の境界に近い機能であることを示しています。

## Module との違い

Module と Phantom はどちらも Actor そのものとは異なりますが、目的は大きく違います。

```text
Module
  -> 既存 Actor に共通定義を再利用する

Phantom
  -> 新しい Actor を実行時に生成するための定義
```

Module は「機能の部品」、Phantom は「生成用テンプレート」と考えると区別しやすくなります。

## まとめ

- Phantom は Actor 定義のテンプレート
- VM ロード時には自動でインスタンス化されない
- C++ の `CreateActorFromPhantom()` から生成できる
- 動的な敵、NPC、ギミックなどに利用できる
- 現行仕様ではスクリプトから直接生成する構文はない

次は、大規模な Mana プログラムで名前を整理する **Namespace** の考え方を整理します。
