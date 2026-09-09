/*!
mana (compiler)

@file	SourceResolver.h
@brief	ソースの供給元に関係するヘッダーファイル
@detail	このファイルはコンパイラへソースを供給する仕組みのヘッダーファイルです。
		コンパイラはこのインターフェース経由でのみソースを読み込むため、
		ファイルシステム以外から供給する事ができます。
@author	Shun Moriya
@date	2026-
*/

#pragma once
#include <string>
#include <string_view>

namespace mana
{
	/*!
	ソースの供給元

	インクルードの解決と読み込みを担当します。既定の実装である
	FileSourceResolver はファイルシステムから読み込みますが、
	エディタの未保存バッファやアセット管理システムから供給する事もできます。
	*/
	class SourceResolver
	{
	public:
		SourceResolver() = default;
		SourceResolver(const SourceResolver& other) = default;
		SourceResolver(SourceResolver&& other) noexcept = default;
		SourceResolver& operator=(const SourceResolver& other) = default;
		SourceResolver& operator=(SourceResolver&& other) noexcept = default;
		virtual ~SourceResolver() = default;

		/*!
		ソースの位置を解決します

		解決に失敗した場合でも、診断に表示できる位置を返して下さい。
		読み込めるかどうかの判断は Read() が行います。

		@param[in]	from		読み込んでいる側の位置。最初のソースでは空
		@param[in]	filename	include や import に書かれた位置
		@return		解決された位置。解決できない場合は空文字列
		*/
		[[nodiscard]] virtual std::string Resolve(const std::string_view from, const std::string_view filename) const = 0;

		/*!
		ソースを読み込みます

		改行コードはコンパイラ側で正規化するため、そのまま返して構いません。

		@param[in]	path		Resolve() が返した位置
		@param[out]	outText		読み込んだ内容
		@retval		true		成功
		@retval		false		失敗
		*/
		[[nodiscard]] virtual bool Read(const std::string_view path, std::string& outText) const = 0;
	};

	/*!
	ファイルシステムから読み込む既定のソース供給元

	include や import に書かれた相対位置は、読み込んでいる側のファイルが
	あるディレクトリを基準に解決します。最初のソースだけは現在の作業
	ディレクトリを基準にします。
	*/
	class FileSourceResolver final : public SourceResolver
	{
	public:
		[[nodiscard]] std::string Resolve(const std::string_view from, const std::string_view filename) const override;
		[[nodiscard]] bool Read(const std::string_view path, std::string& outText) const override;
	};
}
