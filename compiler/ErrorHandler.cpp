/*!
mana (compiler)

@file	ErrorHandler.cpp
@brief	エラーや警告に関係するソースファイル
@detail	このファイルはエラーや警告に関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#include "../runner/common/Setup.h"
#include "ErrorHandler.h"
#include "Lexer.h"

namespace mana
{
	struct Message
	{
		const char* mJaJp;
		const char* mEnUs;
	};

	namespace
	{
		//! 現在有効な診断の収集先
		DiagnosticBag* CurrentDiagnosticBag = nullptr;

		[[nodiscard]] const char* ToLabel(const DiagnosticSeverity severity)
		{
			switch (severity)
			{
			case DiagnosticSeverity::Warning:	return "warning";
			case DiagnosticSeverity::Fatal:		return "fatal";
			case DiagnosticSeverity::Error:
			default:							return "error";
			}
		}

		/*!
		診断の重大度を Trace の重大度へ対応付けます
		*/
		[[nodiscard]] TraceLevel ToTraceLevel(const DiagnosticSeverity severity)
		{
			switch (severity)
			{
			case DiagnosticSeverity::Warning:	return TraceLevel::Warning;
			case DiagnosticSeverity::Error:
			case DiagnosticSeverity::Fatal:
			default:							return TraceLevel::Error;
			}
		}

		/*!
		診断を収集先へ送ります

		収集先が無い場合は従来通り標準出力へ出力します。
		*/
		void Report(const DiagnosticSeverity severity, const DiagnosticPhase phase, const std::string& message)
		{
			Diagnostic diagnostic;
			diagnostic.mSeverity = severity;
			diagnostic.mPhase = phase;
			diagnostic.mMessage = message;

			if (phase == DiagnosticPhase::Compile)
			{
				diagnostic.mFilename = lexer::GetCurrentFilename();
				diagnostic.mLineNo = lexer::GetCurrentLineNo();
			}
			else if (CurrentDiagnosticBag)
			{
				diagnostic.mFilename = CurrentDiagnosticBag->GetTargetFilename();
			}

			if (CurrentDiagnosticBag)
			{
				CurrentDiagnosticBag->Add(std::move(diagnostic));
			}
			else
			{
				Trace(ToTraceLevel(diagnostic.mSeverity), { diagnostic.ToString(), "\n" });
			}
		}
	}

	std::string Diagnostic::ToString() const
	{
		if (mLineNo > 0)
		{
#if defined(MANA_TARGET_WINDOWS)
			return Concat({ mFilename, "(", std::to_string(mLineNo), "): ", ToLabel(mSeverity), ": ", mMessage });
#else
			return Concat({ mFilename, ":", std::to_string(mLineNo), " ", ToLabel(mSeverity), ": ", mMessage });
#endif
		}
		return Concat({ mFilename, ": ", ToLabel(mSeverity), ": ", mMessage });
	}

	DiagnosticBag::DiagnosticBag(const std::string_view targetFilename, DiagnosticHandler handler)
		: mTargetFilename(targetFilename)
		, mHandler(std::move(handler))
		, mPrevious(CurrentDiagnosticBag)
	{
		CurrentDiagnosticBag = this;
	}

	DiagnosticBag::~DiagnosticBag()
	{
		CurrentDiagnosticBag = mPrevious;
	}

	void DiagnosticBag::Add(Diagnostic&& diagnostic)
	{
		if (diagnostic.mSeverity != DiagnosticSeverity::Warning)
		{
			++mErrorCount;
		}

		mDiagnostics.emplace_back(std::move(diagnostic));

		if (mHandler)
		{
			mHandler(mDiagnostics.back());
		}
	}

	const std::vector<Diagnostic>& DiagnosticBag::Get() const noexcept
	{
		return mDiagnostics;
	}

	std::vector<Diagnostic> DiagnosticBag::Release() noexcept
	{
		return std::move(mDiagnostics);
	}

	size_t DiagnosticBag::GetErrorCount() const noexcept
	{
		return mErrorCount;
	}

	std::string_view DiagnosticBag::GetTargetFilename() const noexcept
	{
		return mTargetFilename;
	}

	DiagnosticBag* DiagnosticBag::GetCurrent() noexcept
	{
		return CurrentDiagnosticBag;
	}

	void CompileError(const std::string& message)
	{
		Report(DiagnosticSeverity::Error, DiagnosticPhase::Compile, message);
		++yynerrs;
	}

	void CompileError(std::initializer_list<std::string_view> message)
	{
		CompileError(Concat(message));
	}

	void CompileWarning(const std::string& message)
	{
		Report(DiagnosticSeverity::Warning, DiagnosticPhase::Compile, message);
	}

	void CompileWarning(std::initializer_list<std::string_view> message)
	{
		CompileWarning(Concat(message));
	}

	void LinkerError(const std::string& message)
	{
		Report(DiagnosticSeverity::Error, DiagnosticPhase::Link, message);
	}

	void LinkerError(std::initializer_list<std::string_view> message)
	{
		LinkerError(Concat(message));
	}

	void LinkerWarning(const std::string& message)
	{
		Report(DiagnosticSeverity::Warning, DiagnosticPhase::Link, message);
	}

	void LinkerWarning(std::initializer_list<std::string_view> message)
	{
		LinkerWarning(Concat(message));
	}

	void Fatal(const FatalType type)
	{
		Message message[] = {
			{ "メモリが足りません", "Not enough memory" }
		};
		Fatal(std::string(message[static_cast<uint8_t>(type)].mJaJp));
	}

	void Fatal(const std::string& message)
	{
		Report(DiagnosticSeverity::Fatal, DiagnosticPhase::Link, message);
		++yynerrs;
	}

	void Fatal(std::initializer_list<std::string_view> message)
	{
		Fatal(Concat(message));
	}

	void FatalNoMemory()
	{
		Fatal(FatalType::NoMemory);
	}
}
