# Diagnostics

Mana Compiler は、警告やエラーを単なる文字列として標準出力へ流すだけでなく、`mana::Diagnostic` として構造化して返します。

エディタ、IDE、CI、ゲーム内ツールへ組み込む場合は、`CompileResult::mDiagnostics` を利用すると、ファイル名や行番号を保ったまま独自表示できます。

## CompileResult から取得する

最も基本的な方法は、コンパイル後に `mDiagnostics` を確認することです。

```cpp
mana::CompileOptions options;
options.mSourceFilename = "main.mn";

const mana::CompileResult result = mana::Compile(options);

for (const mana::Diagnostic& diagnostic : result.mDiagnostics)
{
    std::cout << diagnostic.ToString() << '\n';
}
```

診断が存在しても、すべてがコンパイル失敗を意味するわけではありません。

- Warning: 警告。コンパイルは継続する
- Error: エラー。解析は可能な範囲で継続するが、成果物は生成しない
- Fatal: そのコンパイルを継続できない致命的エラー

`CompileResult::mSucceeded` を最終的な成功判定に使用してください。

## Diagnostic の内容

`mana::Diagnostic` には次の情報があります。

```cpp
struct Diagnostic
{
    DiagnosticSeverity mSeverity;
    DiagnosticPhase mPhase;
    std::string mFilename;
    int32_t mLineNo;
    std::string mMessage;
};
```

### Severity

```cpp
mana::DiagnosticSeverity::Warning
mana::DiagnosticSeverity::Error
mana::DiagnosticSeverity::Fatal
```

### Phase

```cpp
mana::DiagnosticPhase::Compile
mana::DiagnosticPhase::Link
```

`Compile` は字句解析、構文解析、意味解析、コード生成などで発生した問題を表します。

`Link` はシンボル解決や Program Image 生成段階の問題を表します。

## ファイル名と行番号

`mFilename` と `mLineNo` を使うと、エディタ上で該当ソースへジャンプできます。

```cpp
for (const auto& diagnostic : result.mDiagnostics)
{
    editor.ShowDiagnostic(
        diagnostic.mFilename,
        diagnostic.mLineNo,
        diagnostic.mMessage);
}
```

`mLineNo == 0` は、診断に行情報がないことを表します。

`include` / `import` 先で発生した診断には、対象ソースのファイル名と行番号が保持されます。

独自 `SourceResolver` を使う場合、`Resolve()` が返す論理パスはそのまま診断上の位置として重要になります。

## 標準書式へ変換する

`Diagnostic::ToString()` は Mana の標準書式へ整形します。

```cpp
const std::string text = diagnostic.ToString();
```

行番号がある場合、プラットフォームに応じて例えば次のような形式になります。

```text
main.mn(12): error: message
```

または、

```text
main.mn:12 error: message
```

独自UIでは構造化フィールドを使い、CLIやログでは `ToString()` を使うという分け方ができます。

## 発生時に受け取る

コンパイル完了後ではなく、診断が発生した時点で受け取りたい場合は `CompileOptions::mDiagnosticHandler` を使用します。

```cpp
mana::CompileOptions options;
options.mSourceFilename = "main.mn";

options.mDiagnosticHandler = [](const mana::Diagnostic& diagnostic)
{
    LogDiagnostic(diagnostic);
};

const mana::CompileResult result = mana::Compile(options);
```

Handler を設定しても、診断は `CompileResult::mDiagnostics` にも残ります。

そのため、

- Handler: リアルタイム表示やログ
- `mDiagnostics`: コンパイル後の一覧表示やテスト

という使い分けができます。

## Handler から例外を出さない

組み込み側の Diagnostic Handler は例外を送出しない設計を推奨します。

現行 `Compile()` はコンパイル境界から例外を外へ出さないよう実装されており、Handler 内で例外が発生した場合もホスト側へ越境しないことを回帰テストしています。

ただし、診断処理そのものが失敗すると本来のエラー表示を失う原因になるため、Handler はできるだけ単純な処理にしてください。

## Compiler のスレッド制約

現在の Mana Compiler は診断収集を含めてグローバル状態を使用しています。

そのため、`mana::Compile()` を複数スレッドから同時に呼び出すことはできません。

エディタでバックグラウンドコンパイルを行う場合でも、Mana Compiler の呼び出し自体は1本に直列化してください。

## CI での利用

CI では `mSucceeded` と `mDiagnostics` を組み合わせて扱うと便利です。

```cpp
const mana::CompileResult result = mana::Compile(options);

if (!result.mSucceeded)
{
    for (const auto& diagnostic : result.mDiagnostics)
        std::cerr << diagnostic.ToString() << '\n';

    return 1;
}
```

警告を独自ポリシーでエラー扱いする場合も、`DiagnosticSeverity` を見てホスト側で判断できます。

## Compile Error と Runtime Error は別

このページで扱う `Diagnostic` は主に Compiler の診断です。

Program Image をロードした後に発生するスクリプト実行エラーや VM 内部エラーは、`Trace`、`ScriptError`、`FatalError` など別の経路で扱います。

それらは [Error Handling](./integration-error-handling.md) を参照してください。

## 関連項目

- [Compiler](./integration-compiler.md)
- [SourceResolver](./integration-source-resolver.md)
- [Error Handling](./integration-error-handling.md)
