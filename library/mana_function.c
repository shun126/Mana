/*!
 * mana (library)
 *
 * @file	mana_function.c
 * @brief	manaの組み込み関数関係のソースファイル
 * @detail	このファイルはmanaの組み込み関数関係のソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___LIBMANA_H___)
#include "libmana.h"
#endif
#include <stdlib.h>
#include <math.h>

static void mana_function_get_user_data(mana_actor* actor)
{
	MANA_ASSERT_PARAMETER(actor, 0);
	mana_actor_set_return_integer(actor, mana_actor_get_user_data(actor));
}

static void mana_function_set_user_data(mana_actor* actor)
{
	MANA_ASSERT_PARAMETER(actor, 1);
	mana_actor_set_user_data(actor, mana_actor_get_parameter_integer(actor, 0));
}

static void mana_function_get_user_pointer(mana_actor* actor)
{
	MANA_ASSERT_PARAMETER(actor, 0);
	mana_actor_set_return_pointer(actor, mana_actor_get_user_pointer(actor));
}

static void mana_function_set_user_pointer(mana_actor* actor)
{
	MANA_ASSERT_PARAMETER(actor, 1);
	mana_actor_set_user_pointer(actor, mana_actor_get_parameter_pointer(actor, 0));
}

static void mana_function_debug(mana_actor* actor)
{
	MANA_ASSERT_PARAMETER(actor, 0);
#if defined(NDEBUG)
	mana_actor_set_return_integer(actor, 0);
#else
	mana_actor_set_return_integer(actor, 1);
#endif
}

static void mana_function_rand(mana_actor* actor)
{
	int max, min;

	MANA_ASSERT_PARAMETER(actor, 2);

	min = mana_actor_get_parameter_integer(actor, 0);
	max = mana_actor_get_parameter_integer(actor, 1);
	mana_actor_set_return_integer(actor, (min + (((unsigned int)(rand() & 32767) * (max - min)) >> 15)));
}

static void mana_function_rand_float(mana_actor* actor)
{
	MANA_ASSERT_PARAMETER(actor, 0);
	mana_actor_set_return_float(actor, (float)(rand() & 32767) / 32768.0f);
}

static void mana_function_wait(mana_actor* actor)
{
	MANA_ASSERT_PARAMETER(actor, 1);

	if(mana_is_frame_changed(mana_actor_get_parent(actor)))
	{
		float second;
		second	= mana_actor_get_parameter_float(actor, 0);
		if(second > 0.0f)
		{
			mana_actor_repeat(actor, MANA_TRUE);
		}
		mana_stack_set_real(&actor->stack, 0, second - mana_actor_get_delta_time());
	}
	else
	{
		mana_actor_repeat(actor, MANA_TRUE);
	}
}

static void mana_function_sin(mana_actor* actor)
{
	float degree, answer;

	MANA_ASSERT_PARAMETER(actor, 1);

	degree = mana_actor_get_parameter_float(actor, 0);
	answer = sinf(mana_angle_mod(degree, 180.f) * (MANA_PI / 180.f));

	mana_actor_set_return_float(actor, answer);
}

static void mana_function_cos(mana_actor* actor)
{
	float degree, answer;

	MANA_ASSERT_PARAMETER(actor, 1);

	degree = mana_actor_get_parameter_float(actor, 0);
	answer = cosf(mana_angle_mod(degree, 180.f) * (MANA_PI / 180.f));

	mana_actor_set_return_float(actor, answer);
}

static void mana_function_atan2(mana_actor* actor)
{
	float x, z, a;

	MANA_ASSERT_PARAMETER(actor, 2);

	x = mana_actor_get_parameter_float(actor, 0);
	z = mana_actor_get_parameter_float(actor, 1);
	a = mana_angle_mod(atan2f(z, x), MANA_PI) * (180.0f / MANA_PI);

	mana_actor_set_return_float(actor, a);
}

static void mana_function_tan( mana_actor *actor )
{
	float degree, answer;

	MANA_ASSERT_PARAMETER(actor, 1);

	degree = mana_actor_get_parameter_float(actor, 0);
	answer = tanf(mana_angle_mod(degree, 180.f) * (MANA_PI / 180.f));

	mana_actor_set_return_float(actor, answer);
}

static void mana_function_angle_mod( mana_actor *actor )
{
	float degree, answer;

	MANA_ASSERT_PARAMETER(actor, 1);

	degree = mana_actor_get_parameter_float(actor, 0);
	answer = mana_angle_mod(degree, 180.f);

	mana_actor_set_return_float(actor, answer);
}

static void mana_function_sqrt( mana_actor *actor )
{
	float value, answer;

	MANA_ASSERT_PARAMETER(actor, 1);

	value = mana_actor_get_parameter_float(actor, 0);
	answer = sqrtf(value);

	mana_actor_set_return_float(actor, answer);
}

static void mana_function_find_actor(mana_actor* actor)
{
	const char* name;

	MANA_ASSERT_PARAMETER(actor, 1);

	name = mana_actor_get_parameter_string(actor, 0);

	mana_actor_set_return_actor(actor, mana_get_actor(mana_actor_get_parent(actor), name));
}

/*!
 * 組み込み関数(debug,wait,rand,frand,sin,cos,atan2,tan,angleMod,sqrt,findActor)を登録します
 */
void mana_function_initialize(void)
{
	mana_regist_function("getUserData", mana_function_get_user_data);
	mana_regist_function("setUserData", mana_function_set_user_data);
	mana_regist_function("getUserPointer", mana_function_get_user_pointer);
	mana_regist_function("setUserPointer", mana_function_set_user_pointer);
	mana_regist_function("debug", mana_function_debug);
	mana_regist_function("wait", mana_function_wait);
	mana_regist_function("rand", mana_function_rand);
	mana_regist_function("frand", mana_function_rand_float);
	mana_regist_function("sin", mana_function_sin);
	mana_regist_function("cos", mana_function_cos);
	mana_regist_function("atan2", mana_function_atan2);
	mana_regist_function("tan", mana_function_tan);
	mana_regist_function("angleMod", mana_function_angle_mod);
	mana_regist_function("sqrt", mana_function_sqrt);
	mana_regist_function("findActor", mana_function_find_actor);
}
