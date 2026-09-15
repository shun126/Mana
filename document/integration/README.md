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
   v
VM::Run()
```

## 読む順番

1. [組み込み概要](./integration-overview.md)
2. [Compiler](./integration-compiler.md)
3. [VM](./integration-vm.md)
4. [Program Image](./integration-program-image.md)

この後、Native Functions、SourceResolver、Diagnostics、Error Handling を追加します。

## 対象読者

Language Reference が Mana スクリプトを書く人向けなのに対し、Integration は Mana をゲームやツールへ組み込む C++ 開発者向けです。
