# VM

`mana::VM` は、Compiler が生成した Program Image を読み込み、Actor / Action を実行する実行環境です。

## Program Image をロードする

メモリ上の Program Image をロードできます。

```cpp
auto image = std::make_shared<std::vector<uint8_t>>(result.mProgramImage);
auto vm = std::make_shared<mana::VM>();

vm->LoadProgram(std::shared_ptr<const void>(image, image->data()));
```

ファイルとして保存済みなら、パスからロードすることもできます。

```cpp
vm->LoadProgram("game.mx");
```

## 実行する

```cpp
while (vm->Run())
{
}
```

`Run()` はVMの処理が継続している間 `true` を返します。

ゲームループへ組み込む場合は、ホスト側の更新単位に合わせて `Run()` を呼び出す構成にできます。

## Actor を検索する

```cpp
auto actor = vm->FindActor("Game::NPC::Guide");
```

Actor名は namespace を含む完全名で扱えます。

## C++ から Request する

VM全体へ名前指定で Request を送れます。

```cpp
vm->Request(10, "Game::NPC::Guide", "talk", nullptr);
```

また、Actor を取得してから直接呼び出す経路もあります。

## Actor の生成

既存Actorを複製するAPIと、Phantomから生成するAPIがあります。

```cpp
auto clone = vm->CloneActor(actor, "GuideClone");
auto enemy = vm->CreateActorFromPhantom("EnemyTemplate", "Enemy01");
```

Phantom は Program Image ロード時には通常Actorとして生成されず、`CreateActorFromPhantom()` で明示的に生成します。

## Native Function を登録する

```cpp
vm->RegisterFunction("nativeAdd", &OnNativeAdd);
```

Mana側で `native` 宣言した関数名と一致する名前を登録します。

## Programロード時の起動

現行VMはプログラムロード後、通常Actorに対して `init` を Priority 1、`main` を Priority 0 で Request します。

```text
init : Priority 1
main : Priority 0
```

このため、`init` は `main` より高いPriorityで初期化処理を実行できます。

## 実行状態

主な状態確認APIには次があります。

- `IsRunning()`
- `IsInInitAction()`
- `IsFinishInitAction()`
- `GetFrameCounter()`
- `GetDeltaTime()`
- `IsFrameChanged()`

ゲームエンジンとの連携では、VMを独立したアプリケーションとしてではなく、ホスト側更新ループの一部として扱うことを想定できます。

## 関連項目

- [Program Image](./integration-program-image.md)
- [Compiler](./integration-compiler.md)
- [Phantom](../reference/reference-phantom.md)
- [Native Function](../reference/reference-native.md)
