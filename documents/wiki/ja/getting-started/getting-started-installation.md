# Mana を使う準備

このページの目標は、ターミナルで `mana --version` を実行し、バージョン情報を表示することです。

ここではリポジトリからツールを**ビルド**します。ビルドとは、Mana の処理系を実装した C++ ソースなどから、使える実行ファイルを作る作業です。Mana スクリプトを書くたびに行う作業ではありません。

## 1. リポジトリを取得する

Git が使える場合は、ターミナルで次を実行します。Git はソースコードを取得・管理するツールです。

```text
git clone https://github.com/shun126/Mana.git
cd Mana
```

`cd` は作業するフォルダーを移動するコマンドです。ここで入った、`mana.sln` や `Makefile` のあるフォルダーを、この教材では **Mana フォルダー**と呼びます。

Git を使わない場合は、リポジトリのソースを ZIP として取得して展開し、そのフォルダーでターミナルを開いてください。

## 2. 使用する OS の手順でビルドする

### Windows / Visual Studio

必要なものは、Visual Studio 2022 の C++ デスクトップ開発環境（MSVC v143 と Windows SDK）、Bison 3.8、Flex 2.6.4 です。Bison と Flex は、Mana の文法を処理する C++ コードの生成に使います。

1. Visual Studio Installer で C++ によるデスクトップ開発を用意します。
2. Windows で動く Bison と Flex の実行ファイルを用意します。
3. Windows の「環境変数」で、次のユーザー環境変数を設定します。
4. 設定後に Visual Studio を開き直し、`mana.sln` を開きます。
5. 構成を `Release`、プラットフォームを `x64` にし、ソリューション エクスプローラーの `mana` プロジェクトをビルドします。依存する `manac` もビルドされます。

| 環境変数名 | 値の例 |
| --- | --- |
| `GNU_BISON_BIN` | `C:\tools\bison\bin\bison.exe` |
| `GNU_FLEX_BIN` | `C:\tools\flex\bin\flex.exe` |

値は手元の配置に合わせ、**実行ファイル名まで含めた絶対パス**にしてください。現在の Bison 呼び出しはパスを引用符で囲まないため、Bison は空白を含まないパスに配置してください。

バージョン情報のヘッダーも更新する場合は、Python 3 を用意し、`PYTHON_BIN` に Python の実行ファイルのパスを設定します。この呼び出しも空白を含まないパスを使います。未設定の場合はリポジトリに含まれるヘッダーを使用します。

ビルド成功後、Mana フォルダーの PowerShell で確認します。

```powershell
.\x64\Release\mana.exe --version
```

バージョン情報が表示されたら、同じ PowerShell に次を入力します。

```powershell
Set-Alias mana (Resolve-Path .\x64\Release\mana.exe).Path
mana --version
```

これで、この PowerShell を開いている間は `mana` という短い名前で実行できます。PATH の変更は不要です。新しく PowerShell を開いたら、Mana フォルダーで再び `Set-Alias` を実行してください。

### Linux / make

C++17 を扱える Clang、make、Bison 3.8、Flex 2.6.4 を用意します。必要なパッケージの導入方法は使用する Linux 環境に従ってください。

端末で次を実行し、各ツールが利用できることを確認します。

```bash
clang++ --version
make --version
bison --version
flex --version
```

Mana フォルダーでビルドします。

```bash
make
./driver/mana --version
```

バージョン情報が表示されたら、Bash で次を実行します。

```bash
mana_executable="$(pwd)/driver/mana"
mana() { "$mana_executable" "$@"; }
mana --version
```

以降は同じ端末で `mana` と入力できます。この関数は実行ファイルの場所を覚え、引数を渡して呼び出します。現在のシェルだけで有効なので、新しい端末を開いたら、Mana フォルダーで再び設定してください。

## 3. 最初のプログラムへ進む

以降の教材は、**Mana フォルダーを作業場所にし、ここで設定した同じターミナルを使う**前提です。`mana --version` が動けば準備は完了です。

うまくいかない場合は、次の順番で調べます。

| 状況 | 確認すること |
| --- | --- |
| Bison / Flex を見つけられずビルドに失敗する | 導入場所と環境変数。Windows の値は実行ファイルまで指定したか |
| 実行ファイルが見つからない | ビルドが成功したか。Windows は `Release / x64` を選んだか |
| フルパスなら動くが `mana` では動かない | このターミナルで短縮名を設定したか |
| ソースファイルが見つからない | ターミナルの作業場所と、指定したファイルの保存場所が一致するか |

次は [はじめての Mana プログラム](./getting-started-first-program.md) です。
