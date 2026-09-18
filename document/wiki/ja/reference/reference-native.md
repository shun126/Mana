# Native Function

`native` は、Mana スクリプトから C++ 側に登録された外部関数を呼び出すための宣言です。

## グローバル native 関数

```mana
native int nativeAdd(int a, int b);
```

`native` 関数は宣言だけを持ち、Mana 側に本文を書きません。

```mana
actor Main
{
    action main
    {
        int value = nativeAdd(10, 20);
        print("%d\n", value);
    }
}
```

実行時には VM が関数名から C++ 側の登録済み関数を検索して呼び出します。

C++ 側では、例えば `RegisterFunction` で登録できます。

```cpp
vm->RegisterFunction("nativeAdd", &OnNativeAdd);
```

## Struct の native メソッド

`native` は Struct メンバーとしても宣言できます。

```mana
struct Vec
{
    float x;
    float y;

    native void normalize();
}

void update(Vec value)
{
    value.normalize();
}
```

Struct の native メソッドは、外部関数名として `Struct名::メソッド名` の形式で解決されます。

```text
Vec::normalize
```

VM の外部関数コールバックには、実行中 Actor に加えて Struct インスタンスを指すポインタが渡されます。

## 宣言構文

```mana
native 戻り値型 関数名(引数...);
```

Struct 内では次の形式です。

```mana
struct TypeName
{
    native 戻り値型 メソッド名(引数...);
}
```

## 通常Functionとの違い

通常の Mana Function は Mana Compiler が生成した命令列へ分岐して実行されます。

`native` Function は Mana 側に本文を持たず、実行時に名前で登録済み外部関数を検索します。

## C++ 側の登録型

現行 VM の基本コールバック型は次の形式です。

```cpp
std::function<void(const std::shared_ptr<mana::Actor>& actor,
                   void* structPointer)>
```

引数と戻り値の受け渡しは Actor の外部関数向け API と VM スタックを通して行われます。詳細は Integration の Native Functions で扱います。

## 外部関数が見つからない場合

VM に同名の外部関数が登録されていない場合、VM は外部関数が見つからないことをエラーとして報告します。

Script側の宣言とC++側の登録名を一致させてください。

## 注意点

`native` は C++ と Mana の境界です。Mana 側の宣言と C++ 側の引数・戻り値の扱いを一致させる必要があります。

特に Struct native メソッドでは、通常のグローバル native 関数と異なり Struct インスタンスへのポインタが渡されます。

## 関連項目

- [Function](./reference-functions.md)
- [Struct](./reference-struct.md)
- [定義済みシンボル](./reference-predefined-symbols.md)
