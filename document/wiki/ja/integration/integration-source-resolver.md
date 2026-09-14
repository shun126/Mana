# SourceResolver

`mana::SourceResolver` は、Mana Compiler へソースコードを供給するためのインターフェースです。

通常はファイルシステムから `.mn` を読み込みますが、エディタの未保存バッファ、アセットデータベース、パッケージ内データ、ネットワーク上の仮想ファイルなどからソースを供給することもできます。

## Compiler との関係

`CompileOptions::mSourceResolver` に独自 Resolver を指定します。

```cpp
mana::CompileOptions options;
options.mSourceFilename = "main.mn";
options.mSourceResolver = resolver;

mana::CompileResult result = mana::Compile(options);
```

`mSourceResolver` を省略した場合は、既定の `mana::FileSourceResolver` が使用されます。

## インターフェース

独自 Resolver では、次の2関数を実装します。

```cpp
class SourceResolver
{
public:
    virtual std::string Resolve(
        std::string_view from,
        std::string_view filename) const = 0;

    virtual bool Read(
        std::string_view path,
        std::string& outText) const = 0;
};
```

役割は明確に分かれています。

- `Resolve()` : 参照元と指定名から、ソースを識別する位置を決める
- `Read()` : 解決済み位置から実際のソース文字列を取得する

## FileSourceResolver

標準の `FileSourceResolver` はファイルシステムから読み込みます。

```text
project/
├─ main.mn
└─ actor/
   └─ npc.mn
```

`main.mn` が次のように書かれている場合、

```mana
import "actor/npc.mn";
```

相対パスは `main.mn` のあるディレクトリを基準に解決されます。

さらに `actor/npc.mn` から別ファイルを読み込む場合は、今度は `npc.mn` の場所が基準になります。

最初のソースだけは現在の作業ディレクトリを基準にします。

## メモリ上のソースを供給する

例えばエディタの未保存内容を直接コンパイルしたい場合は、次のような Resolver を作れます。

```cpp
class MemorySourceResolver final : public mana::SourceResolver
{
public:
    std::map<std::string, std::string, std::less<>> files;

    std::string Resolve(
        std::string_view,
        std::string_view filename) const override
    {
        return std::string(filename);
    }

    bool Read(
        std::string_view path,
        std::string& outText) const override
    {
        const auto it = files.find(path);
        if (it == files.end())
            return false;

        outText = it->second;
        return true;
    }
};
```

利用側は次のようになります。

```cpp
auto resolver = std::make_shared<MemorySourceResolver>();
resolver->files["main.mn"] = R"(
actor Main
{
    action main
    {
        print("Hello\n");
    }
}
)";

mana::CompileOptions options;
options.mSourceFilename = "main.mn";
options.mSourceResolver = resolver;

const mana::CompileResult result = mana::Compile(options);
```

## import とパスの一意性

`import` は、同じ解決済みパスのソースを二度読み込まない仕組みです。

そのため独自 `SourceResolver` では、同じソースを表す指定に対して可能な限り同じ解決済み文字列を返すことが重要です。

例えば次の2つが同じデータを表すのに、

```text
scripts/npc.mn
scripts/./npc.mn
```

異なる解決結果のまま返すと、Compiler からは別ソースとして見える可能性があります。

アセットIDや正規化済み仮想パスなど、安定した識別子へ正規化する設計を推奨します。

## 読み込み失敗

`Resolve()` が空文字列を返した場合、Compiler は指定位置を解決できなかったものとして診断します。

`Resolve()` が位置を返しても `Read()` が `false` を返した場合は、その解決済み位置を開けなかったものとして診断されます。

エディタ統合では、診断表示に使いやすい論理パスを `Resolve()` の結果として返しておくと、どの仮想ファイルで問題が起きたかを利用者へ示しやすくなります。

## 改行コード

`Read()` が返す文字列の改行コードを Resolver 側で統一する必要はありません。

Compiler の Lexer が読み込み後に LF へ正規化します。

## `-I` 相当の強制読み込み

`CompileOptions::mForcedIncludeFiles` に指定したソースも、同じ `SourceResolver` を通して読み込まれます。

```cpp
options.mForcedIncludeFiles.push_back("common.mn");
```

CLI の `-I common.mn` に相当します。

## 用途例

SourceResolver を差し替えることで、次のような統合が可能です。

- ゲームエディタ上の未保存 Mana スクリプトをそのままコンパイルする
- Unreal Engine などのアセットからソースを供給する
- zip / pak などのパッケージ内から読む
- 単体テストでファイルI/Oを使わずソースを与える
- 論理パスと実ファイルの場所を分離する

## 関連項目

- [Compiler](./integration-compiler.md)
- [Diagnostics](./integration-diagnostics.md)
- [ソースファイルと import / include](../reference/reference-source-files.md)
