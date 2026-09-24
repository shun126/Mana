# Compiler

Mana Compiler を C++ から利用する入口は `mana::Compile()` です。

## 基本形

```cpp
mana::CompileOptions options;
options.mSourceFilename = "main.mn";

mana::CompileResult result = mana::Compile(options);
```

`Compile()` はファイルを書き出さず、生成物を `CompileResult` に格納して返します。

## CompileOptions

主な設定は次の通りです。

| メンバー | 内容 |
| --- | --- |
| `mSourceFilename` | エントリとなるソースファイル |
| `mForcedIncludeFiles` | ソースより先に読み込むファイル群。CLI の `-I` 相当 |
| `mGenerateDump` | シンボル表・構文木・中間コードのダンプを生成 |
| `mGeneratePublicTypeDecl` | C++ 型宣言ヘッダーを生成 |
| `mSourceResolver` | ソース供給方法を差し替える |
| `mDiagnosticHandler` | 診断発生時のコールバック |

`mSourceResolver` を省略した場合は、標準のファイルシステムから読み込みます。

## CompileResult

| メンバー | 内容 |
| --- | --- |
| `mSucceeded` | エラーがなければ `true` |
| `mProgramImage` | 生成された Program Image |
| `mPublicTypeDecl` | C++ 公開型宣言 |
| `mDump` | デバッグ用 Markdown ダンプ |
| `mDiagnostics` | 発生した全診断 |

コンパイル失敗時、`mProgramImage` は空になります。

## 診断を即時表示する

```cpp
options.mDiagnosticHandler = [](const mana::Diagnostic& diagnostic)
{
    std::cerr << diagnostic.ToString() << '\n';
};
```

ハンドラを指定しても、診断は `CompileResult::mDiagnostics` にも保持されます。

現行実装では、診断ハンドラ自身が例外を送出した場合も `Compile()` の外へ例外を越境させないよう処理されています。

## 強制インクルード

```cpp
options.mForcedIncludeFiles.push_back("common.mn");
options.mForcedIncludeFiles.push_back("platform.mn");
```

先に追加したファイルほど先に読み込まれます。

## 公開型宣言を生成する

```cpp
options.mGeneratePublicTypeDecl = true;
const mana::CompileResult result = mana::Compile(options);

if (result.mSucceeded)
{
    const std::string& header = result.mPublicTypeDecl;
}
```

CLI の `-t` と同じ生成機能を、ファイル出力せず文字列として取得できます。

## ダンプを生成する

```cpp
options.mGenerateDump = true;
```

成功・失敗の解析やコンパイラ開発用に、Markdown形式のダンプを `mDump` から取得できます。

## 例外境界

`Compile()` は内部で発生した例外を捕捉し、致命的な診断へ変換する設計です。通常、ホスト側は `mSucceeded` と `mDiagnostics` を確認すればよく、Mana内部例外を前提にした制御を組む必要はありません。

## スレッド安全性

現行コンパイラはグローバル状態を持つため、`Compile()` を複数スレッドから同時に呼び出すことはできません。

## 関連項目

- [組み込み概要](./integration-overview.md)
- [Program Image](./integration-program-image.md)
- [ソースファイル](../reference/reference-source-files.md)
