# 第４章 命令を使ってみよう
## 外部ファイルの読み込み
外部命令定義など全マップ共通の命令を各イベントスクリプト内に書く事は非常に効率が悪い。
そこで、各イベントスクリプトファイルはimport命令を使用することで、共用ファイルを読み込み、定義の共有を実現する事が出来ます。
また、別にinclude命令がありますが、import命令は重複読み込みのチェックを行い、includeはチェックを行いません。それ以外の動作はimportと同じです。

## 例2-9 共有ファイルの定義
common.mn

```
int ending_flag;
native void GameFlagOn(int a);
native void GameFlagOff(int a);
```
yongfu.mn
```
import "common.mn";

action Luyue::init
{
    GameFlagOn(ending_flag);
    GameFlagOff(ending_flag);
}
```
## Manaをつかいこなそう
工事中。セーブデータの作り方、共有できる定数の書き方などを書く予定
