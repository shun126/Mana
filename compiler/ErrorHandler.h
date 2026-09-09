/*!
mana (compiler)

@file	ErrorHandler.h
@brief	エラーや警告に関係するヘッダーファイル
@detail	このファイルはエラーや警告に関係するヘッダーファイルです。
@author	Shun Moriya
@date	2017-
*/

#pragma once
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <string>
#include <string_view>
#include <vector>

namespace mana
{
	enum class CompileErrorType : uint8_t
	{
		OK = 0,
	};
	enum class LinkerErrorType : uint8_t
	{
		OK = 0,
	};
	enum class CompileWarningType : uint8_t
	{
		OK = 0,
	};
	enum class LinkerWarningType : uint8_t
	{
		OK = 0,
	};
	enum class FatalType : uint8_t
	{
		NoMemory,
	};

	//! 診断の重大度
	enum class DiagnosticSeverity : uint8_t
	{
		Warning,	//!< 警告。コンパイルは継続します
		Error,		//!< エラー。コンパイルは継続しますが成果物は生成されません
		Fatal		//!< 致命的エラー。コンパイルを継続できません
	};

	//! 診断が発生した工程
	enum class DiagnosticPhase : uint8_t
	{
		Compile,	//!< 字句解析・構文解析・意味解析・コード生成
		Link		//!< シンボルの解決とプログラムイメージの生成
	};

	/*!
	コンパイラが報告する診断1件

	文字列に整形される前の状態で保持するため、埋め込み先のアプリケーションは
	ファイル名や行番号を利用した独自の表示を行う事ができます。
	*/
	struct Diagnostic final
	{
		//! 重大度
		DiagnosticSeverity mSeverity = DiagnosticSeverity::Error;

		//! 発生した工程
		DiagnosticPhase mPhase = DiagnosticPhase::Compile;

		//! 発生したファイル名
		std::string mFilename;

		//! 発生した行番号。0ならば行情報がない事を表します
		int32_t mLineNo = 0;

		//! 本文
		std::string mMessage;

		/*!
		コンパイラの標準的な書式に整形します

		行情報がある場合はWindowsでは "file(line): error: message"、
		それ以外では "file:line error: message" になります。
		@return	整形された文字列（末尾に改行を含みません）
		*/
		[[nodiscard]] std::string ToString() const;
	};

	//! 診断が報告される度に呼ばれるコールバック
	using DiagnosticHandler = std::function<void(const Diagnostic&)>;

	/*!
	診断の収集先

	このオブジェクトが生存している間、CompileError等の呼び出しは全てここへ
	集約されます。生存していない間に報告された診断は標準出力へ出力されます。

	@attention	現在のコンパイラはグローバルな状態を持つため、同時に生存できる
				インスタンスは1つだけです。スレッド安全ではありません。
	*/
	class DiagnosticBag final
	{
	public:
		DiagnosticBag() = delete;
		DiagnosticBag(const DiagnosticBag& other) = delete;
		DiagnosticBag(DiagnosticBag&& other) noexcept = delete;
		DiagnosticBag& operator=(const DiagnosticBag& other) = delete;
		DiagnosticBag& operator=(DiagnosticBag&& other) noexcept = delete;

		/*!
		@param[in]	targetFilename	行情報を持たない診断に付与するファイル名
		@param[in]	handler			診断が追加される度に呼ばれるコールバック（省略可）
		*/
		explicit DiagnosticBag(const std::string_view targetFilename, DiagnosticHandler handler = nullptr);
		~DiagnosticBag();

		//! 診断を追加します
		void Add(Diagnostic&& diagnostic);

		//! 収集した全ての診断を取得します
		[[nodiscard]] const std::vector<Diagnostic>& Get() const noexcept;

		//! 収集した全ての診断を移動して取り出します
		[[nodiscard]] std::vector<Diagnostic> Release() noexcept;

		//! エラーと致命的エラーの合計数を取得します
		[[nodiscard]] size_t GetErrorCount() const noexcept;

		//! 行情報を持たない診断に付与するファイル名を取得します
		[[nodiscard]] std::string_view GetTargetFilename() const noexcept;

		//! 現在有効な収集先を取得します。存在しなければ nullptr を返します
		[[nodiscard]] static DiagnosticBag* GetCurrent() noexcept;

	private:
		std::string mTargetFilename;
		DiagnosticHandler mHandler;
		std::vector<Diagnostic> mDiagnostics;
		size_t mErrorCount = 0;
		DiagnosticBag* mPrevious = nullptr;
	};

	extern void CompileError(const std::string& message);
	extern void CompileError(std::initializer_list<std::string_view> message);

	extern void CompileWarning(const std::string& message);
	extern void CompileWarning(std::initializer_list<std::string_view> message);

	extern void LinkerError(const std::string& message);
	extern void LinkerError(std::initializer_list<std::string_view> message);

	extern void LinkerWarning(const std::string& message);
	extern void LinkerWarning(std::initializer_list<std::string_view> message);

	extern void Fatal(const FatalType type);
	extern void Fatal(const std::string& message);
	extern void Fatal(std::initializer_list<std::string_view> message);

	extern void FatalNoMemory();
}

extern int yynerrs;
//extern void yyerror(const char* message);
