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

現行VMはプログラムロード後、通常Actorに対して `init` を最高優先度（2147483647）、`main` を Priority 0 で Request します。

```text
init : Priority 2147483647
main : Priority 0
```

このため、`init` は `main` より高いPriorityで初期化処理を実行できます。

## 秒単位の待機と時間の更新

`FunctionInitialize(*vm)` で組み込み関数を登録すると、Mana から `native void delay(float seconds);` を宣言して秒単位で待機できます。以前のフレーム数を受け取る `delay` とは互換性がありません。

ホストが時間を管理する場合は `vm->Run(deltaSeconds)` に有限かつ非負の経過秒数を渡します。`Run(0.0)` は命令を実行しますが時間を進めません。初回に `Run(0.0)` を呼ぶと、その時点から待機を開始できます。指定時間は呼び出しの冒頭で一度だけ加算されます。

引数なしの `Run()` は単調時計で前回の更新からの経過時間を計測します。ポーズや倍速を制御する場合は、引数付きの呼び出しに統一してください。`GetElapsedSeconds()` は VM の累積秒数、`GetDeltaTime()` は直前の更新の経過秒数を返します。ロードと `Restart()` で時間はリセットされます。`Restart()` 後の Action 起動には別途 Request が必要です。

待機期限は Action の割り込みごとに保持され、割り込み中も時間は進みます。期限に達した後、次にその Action が実行可能になった時点で再開します。

## 状態の確認

主な状態確認APIには次があります。

- `IsRunning()`
- `GetFrameCounter()`
- `GetDeltaTime()`
- `IsFrameChanged()`

ゲームエンジンとの連携では、VMを独立したアプリケーションとしてではなく、ホスト側更新ループの一部として扱うことを想定できます。

## 関連項目

- [Program Image](./integration-program-image.md)
- [Compiler](./integration-compiler.md)
- [Phantom](../reference/reference-phantom.md)
- [Native Function](../reference/reference-native.md)
