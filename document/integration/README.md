# Mana Integration

このセクションでは、Mana Compiler と Mana VM を C++ アプリケーションやゲームエンジンへ組み込む方法を説明します。

## 全体像

```text
Mana source
   |
   v
mana::Compile()
   |
   v
CompileResult::mProgramImage
   |
   +--> mana::ProgramImage で内容を照会
   |
   v
mana::VM::LoadProgram()
   |
   +--> Native Functions でホスト機能と接続
   |
   v
VM::Run()
```

Compiler へソースを供給する経路は `SourceResolver` で差し替えられ、コンパイル時の問題は `Diagnostic`、実行時の問題は Trace / ScriptError / FatalError などでホスト側へ通知できます。

## 読む順番

1. [組み込み概要](./integration-overview.md)
2. [Compiler](./integration-compiler.md)
3. [VM](./integration-vm.md)
4. [Program Image](./integration-program-image.md)
5. [Native Functions](./integration-native-functions.md)
6. [SourceResolver](./integration-source-resolver.md)
7. [Diagnostics](./integration-diagnostics.md)
8. [Error Handling](./integration-error-handling.md)

## 目的別に探す

| やりたいこと | ページ |
| --- | --- |
| Mana をゲームへ組み込む全体像を知る | [組み込み概要](./integration-overview.md) |
| C++ から Mana ソースをコンパイルする | [Compiler](./integration-compiler.md) |
| Program Image を実行する | [VM](./integration-vm.md) |
| Actor / Action 一覧を実行せず取得する | [Program Image](./integration-program-image.md) |
| Mana からゲーム側 C++ を呼ぶ | [Native Functions](./integration-native-functions.md) |
| エディタやアセットからソースを供給する | [SourceResolver](./integration-source-resolver.md) |
| コンパイルエラーをIDEやCIへ表示する | [Diagnostics](./integration-diagnostics.md) |
| 実行時エラーや内部Faultを扱う | [Error Handling](./integration-error-handling.md) |

## 対象読者

Language Reference が Mana スクリプトを書く人向けなのに対し、Integration は Mana をゲームやツールへ組み込む C++ 開発者向けです。

Mana の言語仕様そのものを調べたい場合は [Language Reference](../reference/README.md) を参照してください。
