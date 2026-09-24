# Compiler と VM

Mana は、ソースコードをそのまま直接実行する方式ではありません。

Mana Compiler がソースコードを解析して **Program Image** を生成し、Mana VM がその Program Image を実行します。

```text
Mana source (.mn)
        |
        v
Mana Compiler
        |
        v
Program Image
        |
        v
Mana VM
        |
        v
Actor / Action の実行
```

## スクリプト言語でもコンパイルできる

スクリプト言語であっても、実行前にソースコードを別の形式へ変換することができます。

Mana でいうコンパイルは、CPU が直接実行するネイティブマシンコードを作ることではありません。

**Mana VM 用の Program Image を作ること**です。

## Compiler の役割

Compiler は、ソースを読み、構文と意味を解析し、VM が実行できる形式へ変換します。

概念的には次の流れです。

```text
ソースを読む
    ↓
構文を解析する
    ↓
名前や型を解析する
    ↓
エラーを検出する
    ↓
Program Image を生成する
```

Mana が namespace や前方参照を扱えるのも、単純に上から1行ずつ実行するのではなく、コンパイル段階でソース全体を解析するためです。

## Program Image とは

Program Image は、Compiler と VM の間をつなぐコンパイル済みデータです。

Actor、Action、命令、定数など、VM がプログラムを実行するために必要な情報を保持します。

```text
Source Code
    ↓ Compiler
Program Image
    ↓ VM
Runtime State
```

ソースコードと、実行中の Actor の状態は別のものです。

## VM の役割

Mana VM は Program Image をロードし、Actor と Action を実行します。

主な役割は次の通りです。

- Actor の管理
- Action の実行
- Request の処理
- Priority による割り込みと再開
- VM 命令の実行
- C++ 側との連携

Tutorial で使った `request` や `awaitCompletion` も、この VM の実行モデルによって動作します。

## Program Image をロードしたとき

通常の `actor` は Program Image のロード時に VM 上へ生成されます。

その後、初期化処理を行い、Actor の `init`、続いて `main` が起動されます。

一方、`phantom` は定義情報として保持されますが、ロード時には Actor インスタンスを生成しません。

## CLI ではまとめて実行できる

```bash
mana source.mn
```

とすると、ソースをコンパイルしてそのまま VM で実行できます。

Program Image をファイルへ出力することもできます。

```bash
mana source.mn -o program.bin
```

コンパイル済みファイルは次のように実行できます。

```bash
mana --execute program.bin
```

内部の役割で見ると、次の違いです。

```text
mana source.mn
    = Compile + Execute

mana source.mn -o program.bin
    = Compile

mana --execute program.bin
    = Execute
```

## Compiler と VM は組み込み可能

Mana の Compiler と VM は、コマンドラインツールだけに閉じた仕組みではありません。

Compiler はライブラリとして、VM はランタイムとしてゲームやエディタへ組み込めます。

```text
Editor / Game Engine
        |
        +--> Mana Compiler
        |        |
        |        v
        |   Program Image
        |        |
        +--> Mana VM
```

詳しい C++ API は Integration で扱います。

## コンパイル時と実行時

Compiler と VM が分かれているため、問題が検出される段階も分かれます。

Compiler は、構文や名前解決、型など、実行前に判断できる問題を検出します。

VM は、実際に Actor と Action が動いているときにしか判断できない問題を扱います。

この分離により、可能な誤りは実行前に見つけつつ、ゲーム実行時の制御は VM に任せられます。

## Program Image の互換性

Program Image は Mana VM が読み込む実行形式です。

現行 VM はロード時にシグネチャ、バージョン、ビット数などを確認します。

そのため Program Image は、対応する Mana VM と組み合わせて利用するコンパイル済み形式として扱います。

## まとめ

- Mana は Compiler と VM に分かれている
- `.mn` ソースを Compiler が Program Image へ変換する
- Program Image は Mana VM 用の実行形式
- VM が Actor / Action / Request を実行する
- Compiler は実行前の解析とエラー検出を担当する
- CLI では Compile と Execute をまとめても分けても使える
- Compiler と VM は C++ アプリケーションへ組み込める

これで Concepts の基本項目は完了です。

次は **Language Reference** で、Mana の構文と各言語機能を正確に調べられる形へ整理します。
