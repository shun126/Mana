# 終わりに
エラーコードとワーニングコード、既知の不都合を紹介します。

## コンパイルオプション

|option|comment|
|---|---|
|-o ファイル名|出力ファイル名を指定します|
|-v|バーボーズモード|

## Error code
* parse error
 * 文法エラー
* syntax error
 * 文字列エラー

## Warning code

### 変数領域のバウンダリがずれました
変数のアドレス割り当てが割り切れないアドレスになった場合表示されます。
例えば、char型変数の後ろにfloat型を並べるとcharとfloatの間に3バイトの未使用領域が割り当てられます。

## 既知の不都合
完成していないので後日まとめます

## あとがき
しばらくお待ちください

## 参考文献
John R.Levine and Tony Mason and Doug Brown, lex & yacc. O’Rilly & Associates, Inc.
Donnely and Stallman, The Bison Manual. Bison配布キットの一部
五月女健治, yacc/lexプログラムジェネレータ on UNIX
原田賢一,　コンパイラ構成法