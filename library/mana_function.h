/*!
 * mana (library)
 *
 * @file	mana_function.h
 * @brief	mana�̑g�ݍ��݊֐��֌W�̃w�b�_�[�t�@�C��
 * @detail	���̃t�@�C����mana�̑g�ݍ��݊֐��֌W�̃w�b�_�[�t�@�C���ł��B
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_FUNCTION_H___)
#define ___MANA_FUNCTION_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*! �g�ݍ��݌^vec2���` */
typedef struct mana_vec2
{
	float x, y;
}mana_vec2;

/*! �g�ݍ��݌^vec3���` */
typedef struct mana_vec3
{
	float x, y, z;
}mana_vec3;

/*! �g�ݍ��݌^vec4���` */
typedef struct mana_vec4
{
	float x, y, z, w;
}mana_vec4;

/*! �g�ݍ��݌^color���` */
typedef struct mana_color
{
	float r, g, b, a;
}mana_color;

/*! �g�ݍ��݊֐��������� */
void mana_function_initialize(void);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
