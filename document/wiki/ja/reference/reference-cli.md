# CLI

`mana` は、Mana ソースのコンパイル、Program Image の出力、Program Image の実行を行うコマンドラインツールです。

## 基本形

```text
mana [options] input
```

## コンパイルして実行

```bash
mana main.mn
```

出力ファイルを指定しない場合、ソースをコンパイルした後、その Program Image をそのまま Mana VM で実行します。

## Program Image を保存

```bash
mana main.mn -o game.mx
```

`-o filename` を指定すると、コンパイル結果の Program Image をファイルへ保存し、自動実行は行いません。

ファイル名の拡張子は任意です。

現行CLIでは、値を付けずに `-o` を指定した場合、入力ソースと同じベース名の `.mx` ファイル名を自動生成します。

```bash
mana main.mn -o
```

この場合は `main.mx` が出力先になります。

## Program Image を実行

```bash
mana --execute game.mx
```

`--execute` を指定すると、入力を Mana ソースではなくコンパイル済み Program Image として読み込み、VM で実行します。

現行 `driver/Main.cpp` では短縮形 `-e` は実装されていません。

## C++ 型宣言ヘッダーを生成

```bash
mana main.mn -t public_types.h
```

`-t filename` は、コンパイラが生成する公開型宣言をファイルへ出力します。

値を付けずに `-t` を指定した場合、入力ソースと同じベース名の `.h` が出力先になります。

```bash
mana main.mn -t
```

この場合は `main.h` が生成されます。

## 強制インクルード

```bash
mana -I common.mn main.mn
```

`-I filename` は、指定ファイルをコンパイル対象へ強制追加します。

複数回指定できます。

```bash
mana -I common.mn -I platform.mn main.mn
```

## 情報表示

```bash
mana --help
mana --version
mana --copyright
```

| オプション | 内容 |
| --- | --- |
| `--help` | 使用方法を表示 |
| `--version` | Mana のバージョンを表示 |
| `--copyright` | 著作権表示 |

## 公開オプション一覧

| オプション | 内容 |
| --- | --- |
| `-o filename` | Program Image の出力先 |
| `-t filename` | C++ 型宣言ヘッダーの出力先 |
| `-I filename` | 強制インクルード。複数指定可 |
| `--execute` | 入力を Program Image として実行 |
| `--help` | ヘルプ表示 |
| `--version` | バージョン表示 |
| `--copyright` | 著作権表示 |

## 終了コード

コンパイルに失敗した場合や出力ファイルを保存できない場合、CLI は失敗を表す終了コードを返します。

ビルドスクリプトやCIでは終了コードとコンパイラ診断を確認してください。

## 関連項目

- [ソースファイルと import / include](./reference-source-files.md)
- [Compiler と VM](../concepts/concept-compiler-and-vm.md)
