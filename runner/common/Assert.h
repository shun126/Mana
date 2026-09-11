/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Platform.h"
#include "Fault.h"
#include "String.h"

/*
 * 不変条件が壊れた場合、manaはプロセスを終了させません。
 * mana::RaiseFault が報告した上で mana::FatalError を送出し、
 * 復帰の意味が定義できる場所で受け止められます。
 *
 *   コンパイル : mana::Compile が捕まえ、致命的な診断として返します
 *   実行       : mana::VM::Run が捕まえ、そのアクターだけを停止します
 */

#define MANA_WARNING(...)	(void)(mana::Trace(mana::TraceLevel::Warning, __VA_ARGS__))
#define MANA_ERROR(...)		(void)(mana::Trace(mana::TraceLevel::Error, __VA_ARGS__))
#define MANA_BUG(...)		mana::RaiseFault(__FILE__, __LINE__, { "BUG!: ", __VA_ARGS__ })
#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
#define MANA_CHECK(expression) (void)( \
	(!!(expression)) || \
	(mana::Trace(mana::TraceLevel::Error, { #expression, ": ", __FILE__, "(", std::to_string(__LINE__), ")\n" }), 0) \
	)
#define MANA_ASSERT(expression) (void)( \
	(!!(expression)) || \
	(mana::RaiseFault(__FILE__, __LINE__, { "assertion failed: ", #expression }), 0) \
	)
#define MANA_ASSERT_MESSAGE(expression, ...) (void)( \
	(!!(expression)) || \
	(mana::RaiseFault(__FILE__, __LINE__, { "assertion failed: ", #expression, ": ", __VA_ARGS__ }), 0) \
	)
#else
#define MANA_CHECK(expression)	((void)0)
#define MANA_ASSERT(expression)	((void)0)
#define MANA_ASSERT_MESSAGE(expression, ...) ((void)0)
#endif
#define MANA_VERIFY(expression)	(void)( \
	(!!(expression)) || \
	(mana::RaiseFault(__FILE__, __LINE__, { "verification failed: ", #expression }), 0) \
	)
#define MANA_VERIFY_MESSAGE(expression, ...) (void)( \
	(!!(expression)) || \
	(mana::RaiseFault(__FILE__, __LINE__, { "verification failed: ", #expression, ": ", __VA_ARGS__ }), 0) \
	)
#define MANA_NOT_IMPLEMENTED() \
	mana::RaiseFault(__FILE__, __LINE__, "not implemented")
