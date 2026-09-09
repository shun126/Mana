/*!
mana (compiler)

@file	Compiler.h
@brief	コンパイラの組み込み用インターフェース
@detail	このファイルはコンパイラをライブラリとして利用する為のヘッダーファイルです。
		コマンドライン引数やファイル入出力に依存せず、コンパイル結果を値として
		返します。
@author	Shun Moriya
@date	2026-
*/

#pragma once
#include "ErrorHandler.h"
#include <cstdint>
#include <string>
#include <vector>

namespace mana
{
	/*!
	コンパイルの入力
	*/
	struct CompileOptions final
	{
		//! コンパイルするソースファイル名
		std::string mSourceFilename;

		/*!
		ソースファイルより先に読み込むファイル名（コマンドラインの -I 相当）

		先頭に指定したファイルほど先に読み込まれます。
		*/
		std::vector<std::string> mForcedIncludeFiles;

		//! 真ならばシンボル表・構文木・中間コードのダンプを生成します（--debug 相当）
		bool mGenerateDump = false;

		//! 真ならばC++の型宣言ヘッダーを生成します（-t 相当）
		bool mGeneratePublicTypeDecl = false;

		/*!
		診断が報告される度に呼ばれるコールバック

		報告と同時に表示したい場合に指定します。省略した場合でも全ての診断は
		CompileResult::mDiagnostics に格納されます。
		*/
		DiagnosticHandler mDiagnosticHandler;
	};

	/*!
	コンパイルの結果

	Compile() はファイルを書き出しません。生成物は全てこの構造体に格納されるので、
	保存するかどうかは呼び出し側が決定します。
	*/
	struct CompileResult final
	{
		//! エラーが1件も発生しなければ真
		bool mSucceeded = false;

		//! 生成されたプログラムイメージ。失敗した場合は空
		std::vector<uint8_t> mProgramImage;

		//! CompileOptions::mGeneratePublicTypeDecl が真で成功した場合のC++ヘッダー
		std::string mPublicTypeDecl;

		//! CompileOptions::mGenerateDump が真の場合のマークダウン
		std::string mDump;

		//! 発生した全ての診断
		std::vector<Diagnostic> mDiagnostics;
	};

	/*!
	manaのソースファイルをコンパイルします

	この関数は例外を送出しません。内部で発生した例外は捕捉され、
	CompileResult::mDiagnostics へ致命的エラーとして格納されます。

	@attention	コンパイラはグローバルな状態を持つため、この関数を同時に複数の
				スレッドから呼び出す事はできません。

	@param[in]	options	コンパイル設定
	@return		コンパイル結果
	*/
	[[nodiscard]] CompileResult Compile(const CompileOptions& options);
}
