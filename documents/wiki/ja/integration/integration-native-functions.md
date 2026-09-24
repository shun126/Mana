# Native Functions

Native Function は、Mana スクリプトからゲームやツール側の C++ 処理を呼び出すための境界です。

Mana 側では `native` を使って関数を宣言し、C++ 側では同じ名前の関数を `mana::VM` に登録します。

## 最小構成

Mana 側で次のように宣言します。

```mana
native int add(int a, int b);

actor Main
{
    action main
    {
        int result = add(10, 20);
        print("%d\n", result);
    }
}
```

C++ 側では、外部関数を登録します。

```cpp
void Add(const std::shared_ptr<mana::Actor>& actor, void*)
{
    const int32_t a = actor->GetParameterInteger(0);
    const int32_t b = actor->GetParameterInteger(1);
    actor->SetReturnInteger(a + b);
}

std::shared_ptr<mana::VM> vm = std::make_shared<mana::VM>();
vm->RegisterFunction("add", &Add);
```

登録した VM に Program Image を読み込み、通常どおり `Run()` すると、Mana の `add()` 呼び出しから C++ の `Add()` が実行されます。

## 外部関数の型

現行 VM の外部関数型は次の形式です。

```cpp
using ExternalFunctionType =
    std::function<void(const std::shared_ptr<mana::Actor>& actor,
                       void* structPointer)>;
```

第1引数の `actor` は、その native 関数を実行している Actor です。

第2引数の `structPointer` は Struct の native メソッドを呼び出した場合に、その Struct インスタンスを参照するために使用されます。

## 引数を取得する

Native Function の引数は、実行中 Actor から取得します。

代表的な API は次の通りです。

```cpp
actor->GetParameterInteger(index);
actor->GetParameterFloat(index);
actor->GetParameterString(index);
actor->GetParameterActor(index);
actor->GetParameterPointer(index);
actor->GetParameterAddress(index);
```

引数数は次で取得できます。

```cpp
const int32_t count = actor->GetArgumentCount();
```

Mana 側の宣言と C++ 側で読み取る型・順番は、組み込み側の契約として一致させてください。

## 戻り値を返す

戻り値がある native 関数では `SetReturn*()` を使用します。

```cpp
actor->SetReturnInteger(value);
actor->SetReturnFloat(value);
actor->SetReturnString(text);
actor->SetReturnActor(otherActor);
actor->SetReturnPointer(pointer);
actor->SetReturnData(data, size);
```

例えば Mana 側が

```mana
native float getSpeed();
```

なら、C++ 側では次のように値を設定します。

```cpp
void GetSpeed(const std::shared_ptr<mana::Actor>& actor, void*)
{
    actor->SetReturnFloat(3.5f);
}
```

## Struct の native メソッド

Mana では Struct のメンバーとして native 関数を宣言できます。

```mana
struct Position
{
    float x;
    float y;

    native void normalize();
}
```

この場合、C++ 側で解決される外部名は `Struct名::メソッド名` です。

```cpp
vm->RegisterFunction("Position::normalize", &NormalizePosition);
```

コールバックの第2引数 `structPointer` には対象 Struct のアドレスが渡されます。

```cpp
void NormalizePosition(const std::shared_ptr<mana::Actor>&,
                       void* structPointer)
{
    // structPointer をホスト側で対応するレイアウトとして扱う
}
```

Struct のメモリレイアウトを C++ 側で直接扱う場合は、Mana 側の型定義との一致を厳密に管理してください。

## C++ メンバー関数を登録する

`VM::RegisterMemberFunction()` を使うと、C++ オブジェクトのメンバー関数をラッパーなしで登録できます。

```cpp
class GameBridge
{
public:
    void PlaySound(const std::shared_ptr<mana::Actor>& actor, void* structPointer)
    {
        // ゲーム側処理
    }
};

auto bridge = std::make_shared<GameBridge>();
vm->RegisterMemberFunction("playSound", bridge, &GameBridge::PlaySound);
```

現行 API には生ポインタ、`shared_ptr`、`weak_ptr` を利用するオーバーロードがあります。

`shared_ptr` を渡すオーバーロードは内部で弱参照として保持します。登録先オブジェクトが先に破棄された場合、呼び出し時にエラーが Trace へ出力されます。

## 登録名を一致させる

VM は実行時に文字列名で外部関数を検索します。

Mana 側の宣言名と `RegisterFunction()` の登録名が一致していない場合、VM は外部関数が見つからないことを Error Trace として報告します。

組み込み時には、Program Image をロードして実行する前に必要な Native Function をすべて登録しておくことを推奨します。

## 設計上の役割

Native Function は、Mana 自身へレンダリング、物理、サウンド、アセット管理などを持ち込むための仕組みではありません。

それらをホストアプリケーション側へ残したまま、Mana から必要な操作だけを公開する境界として設計すると、スクリプトとゲームエンジンの責務を分離しやすくなります。

## 関連項目

- [VM](./integration-vm.md)
- [Program Image](./integration-program-image.md)
- [Native Function リファレンス](../reference/reference-native.md)
- [Error Handling](./integration-error-handling.md)
