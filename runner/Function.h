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
	struct Vector2
	{
		float x, y;
	};

	struct Vector3
	{
		float x, y, z;
	};

	struct Vector4
	{
		float x, y, z, w;
	};

	struct Color
	{
		float r, g, b, a;
	};

	namespace builtInFunction
	{
		inline void IsDebug(const std::shared_ptr<Actor>& actor)
		{
			MANA_ASSERT_PARAMETER(actor, 0);
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
				actor->SetReturnInteger(1);
#else
				actor->SetReturnInteger(0);
#endif
		}

		inline void RandI(const std::shared_ptr<Actor>& actor)
		{
			MANA_ASSERT_PARAMETER(actor, 2);

			const int32_t min_ = actor->GetParameterInteger(0);
			const int32_t max_ = actor->GetParameterInteger(1);
			actor->SetReturnInteger(min_ + ((static_cast<uint32_t>(rand() & 32767) * (max_ - min_)) >> 15));
		}

		inline void RandF(const std::shared_ptr<Actor>& actor)
		{
			MANA_ASSERT_PARAMETER(actor, 0);
			actor->SetReturnFloat(static_cast<float>(rand() & 32767) / 32768.0f);
		}

		inline void Delay(const std::shared_ptr<Actor>& actor)
		{
			MANA_ASSERT_PARAMETER(actor, 1);

			if (actor->GetVirtualMachine()->IsFrameChanged())
			{
				const int32_t frame = actor->GetParameterInteger(0);
				if (frame > 0)
					actor->Repeat(true);
				actor->GetStack().Set<int32_t>(0, frame - 1);
			}
			else
			{
				actor->Repeat(true);
			}
		}

		inline void Sin(const std::shared_ptr<Actor>& actor)
		{
			MANA_ASSERT_PARAMETER(actor, 1);

			const float degree = actor->GetParameterFloat(0);
			const float radian = ToRadian(AngleMod<float>(degree, 180.f));
			const float answer = std::sin(radian);

			actor->SetReturnFloat(answer);
		}

		inline void Cos(const std::shared_ptr<Actor>& actor)
		{
			MANA_ASSERT_PARAMETER(actor, 1);

			const float degree = actor->GetParameterFloat(0);
			const float radian = ToRadian(AngleMod<float>(degree, 180.f));
			const float answer = std::cos(radian);

			actor->SetReturnFloat(answer);
		}

		inline void Atan2(const std::shared_ptr<Actor>& actor)
		{
			MANA_ASSERT_PARAMETER(actor, 2);

			const float x = actor->GetParameterFloat(0);
			const float z = actor->GetParameterFloat(1);
			const float a = ToDegree(std::atan2(z, x));

			actor->SetReturnFloat(a);
		}

		inline void Tan(const std::shared_ptr<Actor>& actor)
		{
			MANA_ASSERT_PARAMETER(actor, 1);

			const float degree = actor->GetParameterFloat(0);
			const float radian = ToRadian(AngleMod<float>(degree, 180.f));
			const float answer = std::tan(radian);

			actor->SetReturnFloat(answer);
		}

		inline void AngleMod(const std::shared_ptr<Actor>& actor)
		{
			MANA_ASSERT_PARAMETER(actor, 1);

			const float degree = actor->GetParameterFloat(0);
			const float answer = mana::AngleMod<float>(degree, 180.f);

			actor->SetReturnFloat(answer);
		}

		inline void Sqrt(const std::shared_ptr<Actor>& actor)
		{
			MANA_ASSERT_PARAMETER(actor, 1);

			const float value = actor->GetParameterFloat(0);
			const float answer = std::sqrt(value);

			actor->SetReturnFloat(answer);
		}

		inline void FindActor(const std::shared_ptr<Actor>& actor)
		{
			MANA_ASSERT_PARAMETER(actor, 1);

			const char* name = actor->GetParameterString(0);
			actor->SetReturnActor(actor->GetVirtualMachine()->FindActor(name));
		}
	}

	/**
	* 組み込み関数(debug,wait,irand,frand,sin,cos,atan2,tan,angleMod,sqrt,find,)を登録します
	*/
	inline void FunctionInitialize(VM& vm)
	{
		vm.RegisterFunction("debug", builtInFunction::IsDebug);
		vm.RegisterFunction("delay", builtInFunction::Delay);
		vm.RegisterFunction("irand", builtInFunction::RandI);
		vm.RegisterFunction("frand", builtInFunction::RandF);
		vm.RegisterFunction("sin", builtInFunction::Sin);
		vm.RegisterFunction("cos", builtInFunction::Cos);
		vm.RegisterFunction("atan2", builtInFunction::Atan2);
		vm.RegisterFunction("tan", builtInFunction::Tan);
		vm.RegisterFunction("angleMod", builtInFunction::AngleMod);
		vm.RegisterFunction("sqrt", builtInFunction::Sqrt);
		vm.RegisterFunction("find,", builtInFunction::FindActor);
	}
}
