# 第５章 さらに使いこなしてみよう
この章ではゲームプログラムにManaの拡張命令を組み込む方法を説明します。

## モジュールとMix-in
実装中につき不完全。デバック中

```
module TestModule
{
    action sleep
    {
        wait(1.0);
    }
}
actor NiceGuy
{
    extend TestModule;
}
```
## ActorとPhantom
actorを定義するとそのactorは存在する事になりますが、戦闘における敵の思考など不特定多数のAIにManaを利用する時、敵の人数分actorを定義する事は現実的ではありません。 そこでactionは定義するが実体は実行時に複製される概念が必要です。それをManaではPhantomと呼びます。 複製されたPhantomに対してのrequestが解決されていない。

```
phantom TestPhantom
{
    action dead;
}
```
## Actorの継承
''仕様検討中''

```
actor ChildActor : ParentActor
{
    action dead;
}
```
