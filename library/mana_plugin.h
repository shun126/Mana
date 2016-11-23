/*!
 * mana (library)
 *
 * @file	mana_plugin.h
 * @brief	プラグイン関係のヘッダーファイル
 * @detail	このファイルはプラグイン関係のヘッダーファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_PLUGIN_H___)
#define ___MANA_PLUGIN_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*! プラグインシステムの初期化 */
extern void mana_plugin_initialize(void);

/*! プラグインシステムの終了 */
extern void mana_plugin_finalize(void);

/*! プラグインファイルの読み込む */
extern mana_bool mana_plugin_load(const char* file_name);

/*! ディレクトリ内のプラグインファイルを読み込む */
extern mana_bool mana_plugin_regist(const char* directory_name);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
