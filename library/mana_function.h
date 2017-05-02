/*!
 * mana (library)
 *
 * @file	mana_function.h
 * @brief	manaの組み込み関数関係のヘッダーファイル
 * @detail	このファイルはmanaの組み込み関数関係のヘッダーファイルです。
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_FUNCTION_H___)
#define ___MANA_FUNCTION_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*! 組み込み型vec2を定義 */
typedef struct mana_vec2
{
	float x, y;
}mana_vec2;

/*! 組み込み型vec3を定義 */
typedef struct mana_vec3
{
	float x, y, z;
}mana_vec3;

/*! 組み込み型vec4を定義 */
typedef struct mana_vec4
{
	float x, y, z, w;
}mana_vec4;

/*! 組み込み型colorを定義 */
typedef struct mana_color
{
	float r, g, b, a;
}mana_color;

/*! 組み込み関数を初期化 */
void mana_function_initialize(void);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
