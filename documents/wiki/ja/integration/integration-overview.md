# 組み込み概要

Mana は、コンパイラと VM をライブラリとして C++ アプリケーションへ組み込めます。

## 最小構成

```cpp
#include "compiler/Compiler.h"
#include "runner/Mana.h"

mana::CompileOptions options;
options.mSourceFilename = "main.mn";

const mana::CompileResult result = mana::Compile(options);
if (!result.mSucceeded)
    return;

auto image = std::make_shared<std::vector<uint8_t>>(result.mProgramImage);
auto vm = std::make_shared<mana::VM>();
vm->LoadProgram(std::shared_ptr<const void>(image, image->data()));

while (vm->Run())
{
}
```

組み込み側は大きく3段階に分かれます。

1. `mana::Compile()` でソースを Program Image へ変換する
2. 必要なら `mana::ProgramImage` で Actor / Action を事前確認する
3. `mana::VM` へ Program Image をロードして実行する

## Compiler と VM を分離する

Mana Compiler はファイルを書き出すことを前提にしていません。`Compile()` は `CompileResult` を返し、Program Image は `std::vector<uint8_t>` として取得できます。

そのため、エディタ内コンパイル、アセットビルド、サーバー側ビルドなどにも組み込めます。

VM はコンパイル済み Program Image を受け取って実行します。実行環境に Compiler を含めず、ビルド済み Program Image だけを配布する構成も可能です。

## ソースをファイル以外から供給する

`CompileOptions::mSourceResolver` を差し替えると、ファイルシステム以外からソースを供給できます。

例えば次のような用途があります。

- エディタの未保存バッファ
- ゲームエンジンのアセットシステム
- パッケージ内の仮想ファイル
- テスト用のメモリ上ソース

詳しくは後続の SourceResolver ページで扱います。

## エラーの扱い

コンパイルエラーは `CompileResult::mDiagnostics` に格納されます。

`Compile()` は内部例外を捕捉し、C++ 呼び出し側へ例外を越境させない設計です。一方、VM 実行時の問題は Trace や実行状態として扱われます。

## スレッドについて

現行 `Compile()` はコンパイラ内部にグローバル状態を持つため、複数スレッドから同時実行できません。

複数アセットを並列ビルドする場合でも、Mana Compiler の呼び出し部分は直列化してください。

## 関連項目

- [Compiler](./integration-compiler.md)
- [VM](./integration-vm.md)
- [Program Image](./integration-program-image.md)
