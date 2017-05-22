/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "common/Setup.h"
#include "VM.h"
#include <cmath>
#include <cstdlib>

namespace mana
{
	struct mana_vec2
	{
		float x, y;
	};

	struct mana_vec3
	{
		float x, y, z;
	};

	struct mana_vec4
	{
		float x, y, z, w;
	};

	struct mana_color
	{
		float r, g, b, a;
	};

	inline void mana_function_get_user_data(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 0);
		actor->SetReturnInteger(actor->GetUserData());
	}

	inline void mana_function_set_user_data(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 1);
		actor->SetUserData(actor->GetParameterInteger(0));
	}

	inline void mana_function_get_user_pointer(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 0);
		actor->SetReturnPointer(actor->GetUserPointer());
	}

	inline void mana_function_set_user_pointer(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 1);
		actor->SetUserPointer(actor->GetParameterPointer(0));
	}

	inline void mana_function_debug(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 0);
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		actor->SetReturnInteger(1);
#else
		actor->SetReturnInteger(0);
#endif
	}

	inline void mana_function_rand(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 2);

		const int32_t min_ = actor->GetParameterInteger(0);
		const int32_t max_ = actor->GetParameterInteger(1);
		actor->SetReturnInteger(min_ + (((uint32_t)(rand() & 32767) * (max_ - min_)) >> 15));
	}

	inline void mana_function_rand_float(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 0);
		actor->SetReturnFloat(static_cast<float>(rand() & 32767) / 32768.0f);
	}

	inline void mana_function_wait(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 1);

		if(actor->GetVirtualMachine()->IsFrameChanged())
		{
			const int32_t frame = actor->GetParameterInteger(0);
			if(frame > 0)
				actor->Repeat(true);
			actor->GetStack().Set<int32_t>(0, frame - 1);
		}
		else
		{
			actor->Repeat(true);
		}
	}

	inline void mana_function_sin(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 1);

		const float degree = actor->GetParameterFloat(0);
		const float radian = ToRadian(AngleMod<float>(degree, 180.f));
		const float answer = std::sin(radian);

		actor->SetReturnFloat(answer);
	}

	inline void mana_function_cos(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 1);

		const float degree = actor->GetParameterFloat(0);
		const float radian = ToRadian(AngleMod<float>(degree, 180.f));
		const float answer = std::cos(radian);

		actor->SetReturnFloat(answer);
	}

	inline void mana_function_atan2(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 2);

		const float x = actor->GetParameterFloat(0);
		const float z = actor->GetParameterFloat(1);
		const float a = ToDegree(std::atan2(z, x));

		actor->SetReturnFloat(a);
	}

	inline void mana_function_tan(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 1);

		const float degree = actor->GetParameterFloat(0);
		const float radian = ToRadian(AngleMod<float>(degree, 180.f));
		const float answer = std::tan(radian);

		actor->SetReturnFloat(answer);
	}

	inline void mana_function_angle_mod(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 1);

		const float degree = actor->GetParameterFloat(0);
		const float answer = AngleMod<float>(degree, 180.f);

		actor->SetReturnFloat(answer);
	}

	inline void mana_function_sqrt(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 1);

		const float value = actor->GetParameterFloat(0);
		const float answer = std::sqrt(value);

		actor->SetReturnFloat(answer);
	}

	inline void mana_function_find_actor(const std::shared_ptr<Actor>& actor)
	{
		MANA_ASSERT_PARAMETER(actor, 1);

		const char* name = actor->GetParameterString(0);
		actor->SetReturnActor(actor->GetVirtualMachine()->GetActor(name));
	}

	/*!
	組み込み関数(debug,wait,rand,frand,sin,cos,atan2,tan,angleMod,sqrt,find,)を登録します
	*/
	inline void mana_function_initialize(VM& vm)
	{
		vm.RegistFunction("getUserData", mana_function_get_user_data);
		vm.RegistFunction("setUserData", mana_function_set_user_data);
		vm.RegistFunction("getUserPointer", mana_function_get_user_pointer);
		vm.RegistFunction("setUserPointer", mana_function_set_user_pointer);
		vm.RegistFunction("debug", mana_function_debug);
		vm.RegistFunction("wait", mana_function_wait);
		vm.RegistFunction("rand", mana_function_rand);
		vm.RegistFunction("frand", mana_function_rand_float);
		vm.RegistFunction("sin", mana_function_sin);
		vm.RegistFunction("cos", mana_function_cos);
		vm.RegistFunction("atan2", mana_function_atan2);
		vm.RegistFunction("tan", mana_function_tan);
		vm.RegistFunction("angleMod", mana_function_angle_mod);
		vm.RegistFunction("sqrt", mana_function_sqrt);
		vm.RegistFunction("find,", mana_function_find_actor);
	}
}
