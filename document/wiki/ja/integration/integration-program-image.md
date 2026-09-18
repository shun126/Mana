# Program Image

`mana::ProgramImage` は、コンパイル済み Program Image を読み込み、含まれている Actor / Action / Phantom を実行前に調べるためのクラスです。

`mana::VM` が「実行する」ためのAPIなのに対し、`ProgramImage` は「中身を調べる」ためのAPIです。

## ロードする

```cpp
auto bytes = std::make_shared<std::vector<uint8_t>>(result.mProgramImage);

mana::ProgramImage image;
const bool loaded = image.LoadProgram(
    std::shared_ptr<const void>(bytes, bytes->data()),
    bytes->size());
```

ロード結果は `bool` で返ります。

```cpp
if (!loaded)
{
    std::cerr << image.GetLastError() << '\n';
}
```

`IsLoaded()` でも状態を確認できます。

## Actor一覧を取得する

```cpp
for (std::string_view name : image.GetActorNames())
{
    std::cout << name << '\n';
}
```

特定Actorの存在確認もできます。

```cpp
if (image.HasActor("Game::NPC::Guide"))
{
}
```

## Actor の Action を調べる

```cpp
for (std::string_view action :
     image.GetActorActionNames("Game::NPC::Guide"))
{
    std::cout << action << '\n';
}
```

```cpp
if (image.HasActorAction("Game::NPC::Guide", "talk"))
{
}
```

ゲーム側から名前でRequestする前の検証や、エディタUIの候補一覧生成に利用できます。

## Phantom を調べる

Phantomについても存在確認とAction一覧の取得ができます。

```cpp
if (image.HasPhantom("EnemyTemplate"))
{
    const auto actions =
        image.GetPhantomActionNames("EnemyTemplate");
}
```

```cpp
image.HasPhantomAction("EnemyTemplate", "damage");
```

## Program Image の寿命

`ProgramImage` はロードしたバイト列を `shared_ptr<const void>` として保持します。

Actor名やAction名の戻り値は `std::string_view` なので、これらのビューは Program Image が保持するデータの寿命と関係します。取得した `string_view` を、元の `ProgramImage` より長く保持しないようにしてください。

## 用途

`ProgramImage` は特に次の用途に向いています。

- エディタでActor一覧を表示する
- Action名をコンボボックスから選択する
- C++側の設定に指定されたActor / Action名を事前検証する
- Phantomの生成候補を列挙する
- Program Imageを実行せず検査する

## VMとの違い

| | `ProgramImage` | `VM` |
| --- | --- | --- |
| Program Imageを読む | する | する |
| Actor / Action一覧を調べる | 主用途 | 一部検索APIあり |
| Actionを実行する | しない | する |
| Requestする | しない | する |
| Native Functionを登録する | しない | する |

## 関連項目

- [Compiler](./integration-compiler.md)
- [VM](./integration-vm.md)
- [Actor](../reference/reference-actor.md)
- [Action](../reference/reference-action.md)
