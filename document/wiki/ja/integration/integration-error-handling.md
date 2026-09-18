# Error Handling

Mana をゲームやツールへ組み込む場合、エラーを一種類として扱うのではなく、**コンパイル時 / Program Image 読み込み時 / スクリプト実行時 / Mana 内部不整合** に分けて考えると扱いやすくなります。

## 全体像

```text
Mana source
   |
   | Compile diagnostics
   v
mana::Compile()
   |
   | Program Image load error
   v
mana::VM::LoadProgram()
   |
   | ScriptError / runtime trace
   v
mana::VM::Run()
   |
   | FatalError / FaultHandler
   v
Mana または組み込み側の不整合
```

それぞれで復帰単位と通知経路が異なります。

## コンパイル時のエラー

`mana::Compile()` は、失敗を `CompileResult` として返します。

```cpp
const mana::CompileResult result = mana::Compile(options);

if (!result.mSucceeded)
{
    for (const auto& diagnostic : result.mDiagnostics)
        ShowError(diagnostic);
}
```

Compiler 内部で発生した例外も `Compile()` の境界で捕捉され、致命的診断として `mDiagnostics` へ格納されます。

そのため通常の組み込みコードでは、コンパイルエラーのために `Compile()` 全体を例外で制御する必要はありません。

詳細は [Diagnostics](./integration-diagnostics.md) を参照してください。

## Program Image の読み込みエラー

`VM::LoadProgram()` は Compiler API と異なり、読み込みに失敗した場合に例外を送出する場合があります。

例えば現行実装では、次のような問題が検査されます。

- ファイルを開けない
- Mana Program Image のシグネチャではない
- Program Image のバージョンが一致しない
- 32bit / 64bit の形式が実行側と一致しない

ホスト側ではロード境界で例外を捕捉してください。

```cpp
try
{
    vm->LoadProgram("event.mx");
}
catch (const std::exception& e)
{
    LogError(e.what());
    return false;
}
```

メモリから読み込む場合も、不正な Program Image に対する例外をホスト側で扱う設計にしてください。

## ProgramImage で事前検査する

実行せずに Program Image を調べたい場合は `mana::ProgramImage` を利用できます。

```cpp
mana::ProgramImage image;

if (!image.LoadProgram(program, size))
{
    LogError(image.GetLastError());
    return false;
}
```

`ProgramImage::LoadProgram()` は `bool` で成功を返し、失敗理由は `GetLastError()` から取得できます。

エディタやアセットインポータでは、VM に渡す前の検査として利用できます。

## スクリプト実行エラー

0除算、配列範囲外、不正な自己待機など、Mana スクリプトの実行中に継続できない問題は `mana::ScriptError` として扱われます。

`VM::RunActor()` は `ScriptError` を Actor 単位で捕捉します。

```text
Actor A
  ScriptError
     |
     v
  Actor A を停止

Actor B / C
  実行継続
```

エラーを起こした Actor は停止しますが、それだけを理由に VM 全体や他の Actor が停止する設計ではありません。

エラー内容は `TraceLevel::Error` として Trace に出力されます。

## Trace をホスト側へ接続する

`print()` の出力、警告、実行時エラーなどは Trace 経由で受け取れます。

```cpp
void OnTrace(
    void* userData,
    const mana::TraceLevel level,
    const char* message,
    const std::size_t length)
{
    // ゲームエンジン側のログへ送る
}

mana::SetTraceHandler(&OnTrace, hostContext);
```

重大度は次の3種類です。

```cpp
mana::TraceLevel::Info
mana::TraceLevel::Warning
mana::TraceLevel::Error
```

標準出力が利用しにくいゲームエンジンへ組み込む場合は、起動時に TraceHandler を設定してエンジン側ログへ接続することを推奨します。

### TraceHandler の注意点

TraceHandler は利用開始前に一度設定し、実行中に頻繁に差し替えないでください。

また、Handler 自身から例外を送出しないでください。

Trace は必ずしも1行単位でコールバックされるとは限らないため、行単位のログが必要ならホスト側で改行までバッファリングします。

## Mana 内部の不整合

スクリプトの誤りではなく、Mana 自身または組み込み側の不整合によって内部の前提が崩れた場合は `mana::FatalError` が使用されます。

この系統は `RaiseFault()` から報告され、Error Trace を出力した後に `FatalError` を送出します。

Actor 実行中に発生した `std::exception` は `VM::RunActor()` の境界で捕捉され、その Actor が停止します。他の Actor は継続できます。

## FaultHandler

内部不整合をデバッガーやクラッシュレポートへ接続したい場合は `SetFaultHandler()` を利用できます。

```cpp
void OnFault(
    void* userData,
    const char* file,
    int line,
    const char* message)
{
    // debugger break / crash reporter / telemetry など
}

mana::SetFaultHandler(&OnFault, hostContext);
```

FaultHandler は巻き戻しが始まる前に呼ばれるため、Mana 開発中にその場でデバッガーを止めたい場合にも利用できます。

Handler から戻った後は `FatalError` が送出されます。

FaultHandler 自身から例外を送出してはいけません。

## ScriptError と FatalError の違い

| 種類 | 意味 | FaultHandler | Actor |
| --- | --- | --- | --- |
| `ScriptError` | スクリプト側の実行時エラー | 呼ばれない | 該当Actorを停止 |
| `FatalError` | Manaまたは組み込み側の内部不整合 | 呼ばれる | Actor実行境界なら該当Actorを停止 |

スクリプト作者へ見せるエラーと、Mana/エンジン開発者が調査すべき内部不整合を分離することが重要です。

## 推奨するホスト側の方針

組み込み側では次のように責務を分けると扱いやすくなります。

1. Compiler の問題は `Diagnostic` としてエディタへ表示する
2. Program Image のロード失敗はロード処理の例外として扱う
3. Runtime の `ScriptError` は Trace へ送り、該当Actorだけを停止する
4. `FatalError` は Trace + FaultHandler で開発者へ通知する
5. `TraceHandler` と `FaultHandler` はアプリケーション初期化時に設定する

これにより、1つの不正なスクリプトでゲームプロセス全体を即座に終了させるのではなく、問題の種類に応じた復帰単位を選べます。

## 関連項目

- [Diagnostics](./integration-diagnostics.md)
- [VM](./integration-vm.md)
- [Program Image](./integration-program-image.md)
- [Native Functions](./integration-native-functions.md)
