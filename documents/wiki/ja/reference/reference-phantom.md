# Phantom

`phantom` は、VM 起動時にはインスタンスを作らず、C++ 側から必要なタイミングで Actor を生成するためのテンプレートです。

## 構文

```mana
phantom EnemyTemplate
{
    int mHp;

    action main
    {
    }

    action damage
    {
    }
}
```

文法上の本体は `actor` と同じ形式で、Action やメンバーを定義できます。

## Actor との違い

通常の `actor` は Program Image のロード時に VM がインスタンスを生成し、Actor 一覧へ登録します。

`phantom` はロード時には Actor 一覧へ生成されません。VM は Phantom の定義情報を保持し、C++ 側から明示的に生成されたときに Actor インスタンスを作ります。

| | `actor` | `phantom` |
| --- | --- | --- |
| VMロード時に生成 | される | されない |
| Actionを定義 | できる | できる |
| Actor変数を持つ | できる | できる |
| 主な用途 | 常駐する実行主体 | 動的生成用テンプレート |

## C++ から生成する

現行 VM API では `CreateActorFromPhantom` を使用します。

```cpp
std::shared_ptr<mana::Actor> enemy =
    vm->CreateActorFromPhantom("EnemyTemplate", "Enemy01");
```

第1引数は Phantom の定義名、第2引数は生成する Actor の名前です。

生成された Actor は VM の Actor 一覧へ登録され、Phantom に定義されていた Action と Actor 変数領域を持ちます。

## 自動起動との違い

VM が Program Image をロードすると、通常の Actor に対して `init` と `main` の Request を行います。

Phantom はその時点では Actor として生成されていないため、このロード時の一括 Request の対象にはなりません。

動的生成した Actor をどのように初期化・開始するかは、生成側の C++ コードと VM API の利用方法を含めて設計してください。

## スクリプトからの生成

現行言語には、Mana スクリプトから Phantom を直接インスタンス化する構文はありません。

Phantom の生成はホスト側 C++ API の責務です。

## エラー

存在しない Phantom 名を `CreateActorFromPhantom` に渡すと、VM は `Phantom not found` の実行時エラーを発生させます。

## 関連項目

- [Actor](./reference-actor.md)
- [Module](./reference-module.md)
- [Compiler と VM](../concepts/concept-compiler-and-vm.md)
