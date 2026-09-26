# Mana を使う準備

このページの目標は、ターミナルで `mana --version` を実行し、バージョン情報を表示することです。

ここではリポジトリからツールを**ビルド**します。ビルドとは、Mana の処理系を実装した C++ ソースなどから、使える実行ファイルを作る作業です。Mana スクリプトを書くたびに行う作業ではありません。

## 1. リポジトリを取得する

Git が使える場合は、ターミナルで次を実行します。Git はソースコードを取得・管理するツールです。

```text
git clone https://github.com/shun126/Mana.git
cd Mana
```

`cd` は作業するフォルダーを移動するコマンドです。ここで入った、`CMakeLists.txt` のあるフォルダーを、この教材では **Mana フォルダー**と呼びます。

Git を使わない場合は、リポジトリのソースを ZIP として取得して展開し、そのフォルダーでターミナルを開いてください。

## 2. 使用する OS の手順でビルドする

### Windows / Visual Studio

必要なものは、Visual Studio 2022 以降の C++ デスクトップ開発環境（MSVC v143 以降と Windows SDK）、CMake 3.20 以降、Python 3、Bison 3.8 以降、Flex 2.6.4 以降です。Bison と Flex は、Mana の文法を処理する C++ コードの生成に使います。

1. Visual Studio Installer で C++ によるデスクトップ開発を用意します。
2. Windows で動く Bison と Flex の実行ファイルを用意します。
3. Mana フォルダーの PowerShell で、必須の環境変数に Bison と Flex の実行ファイルの絶対パスを設定し、次のコマンドを実行します。パスは実際の配置に合わせてください。

```powershell
$env:BISON_EXECUTABLE = "C:\path\to\bison.exe"
$env:FLEX_EXECUTABLE = "C:\path\to\flex.exe"
cmake -S . -B build -A x64
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
```

Bison と Flex が PATH にある場合も、両方の環境変数が必要です。Visual Studio で CMakeLists.txt を開く場合は、Windows のユーザー環境変数として設定して Visual Studio を再起動するか、ローカルの `CMakeSettings.json` の `environments` に設定してください。32 ビット版を作る場合は `-A Win32` を使用し、別のビルドフォルダーを指定してください。CMake は見つかった最新の Visual Studio を使います。特定のバージョンを使う場合は、`-G "Visual Studio 17 2022"` のようにジェネレーター名を `-G` で指定してください。

ビルド成功後、Mana フォルダーの PowerShell で確認します。

```powershell
.\build\Release\mana.exe --version
```

バージョン情報が表示されたら、同じ PowerShell に次を入力します。

```powershell
Set-Alias mana (Resolve-Path .\build\Release\mana.exe).Path
mana --version
```

これで、この PowerShell を開いている間は `mana` という短い名前で実行できます。PATH の変更は不要です。新しく PowerShell を開いたら、Mana フォルダーで再び `Set-Alias` を実行してください。

### Linux / CMake

C++17 を扱えるコンパイラ、CMake 3.20 以降、Make、Python 3、Bison 3.8 以降、Flex 2.6.4 以降を用意します。必要なパッケージの導入方法は使用する Linux 環境に従ってください。

端末で次を実行し、各ツールが利用できることを確認します。

```bash
cmake --version
bison --version
flex --version
```

Mana フォルダーでビルドします。

```bash
export BISON_EXECUTABLE="$(command -v bison)"
export FLEX_EXECUTABLE="$(command -v flex)"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/mana --version
```

バージョン情報が表示されたら、Bash で次を実行します。

```bash
mana_executable="$(pwd)/build/mana"
mana() { "$mana_executable" "$@"; }
mana --version
```

以降は同じ端末で `mana` と入力できます。この関数は実行ファイルの場所を覚え、引数を渡して呼び出します。現在のシェルだけで有効なので、新しい端末を開いたら、Mana フォルダーで再び設定してください。

## 3. 最初のプログラムへ進む

以降の教材は、**Mana フォルダーを作業場所にし、ここで設定した同じターミナルを使う**前提です。`mana --version` が動けば準備は完了です。

うまくいかない場合は、次の順番で調べます。

| 状況 | 確認すること |
| --- | --- |
| Bison / Flex を見つけられずビルドに失敗する | `BISON_EXECUTABLE` と `FLEX_EXECUTABLE` が実行ファイルを指しているか |
| 実行ファイルが見つからない | ビルドが成功したか。Windows は `Release / x64` でビルドしたか |
| フルパスなら動くが `mana` では動かない | このターミナルで短縮名を設定したか |
| ソースファイルが見つからない | ターミナルの作業場所と、指定したファイルの保存場所が一致するか |

次は [はじめての Mana プログラム](./getting-started-first-program.md) です。
