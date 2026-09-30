/*!
mana (web)

@file	MemorySourceResolver.h
@brief	メモリ上のソースを供給するソース供給元
@detail	Web Playground のエディタの内容をコンパイラへ渡す為のソース供給元です。
		ファイルシステムには一切触れず、ファイル名とソースの対応表から読み込みます。
@author	Shun Moriya
@date	2026-
*/

#pragma once
#include "../../compiler/SourceResolver.h"
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace mana::web
{
	/*!
	ファイル名とソースの対応表から読み込むソース供給元

	位置は "/" 区切りの相対位置として扱います。include や import に書かれた
	位置は読み込んでいる側のディレクトリを基準に解決するので、
	FileSourceResolver と同じ書き方で複数ファイルを扱えます。
	*/
	class MemorySourceResolver final : public SourceResolver
	{
	public:
		//! ソースを登録します。同じ名前が既にあれば置き換えます
		void SetFile(const std::string_view filename, const std::string_view text)
		{
			mFiles[Normalize(filename)] = std::string(text);
		}

		//! 登録した全てのソースを削除します
		void Clear() noexcept
		{
			mFiles.clear();
		}

		[[nodiscard]] std::string Resolve(const std::string_view from, const std::string_view filename) const override
		{
			if (filename.empty())
				return std::string();

			if (from.empty() || filename.front() == '/')
				return Normalize(filename);

			const size_t slash = from.find_last_of('/');
			if (slash == std::string_view::npos)
				return Normalize(filename);

			std::string joined(from.substr(0, slash + 1));
			joined.append(filename);
			return Normalize(joined);
		}

		[[nodiscard]] bool Read(const std::string_view path, std::string& outText) const override
		{
			const auto it = mFiles.find(path);
			if (it == mFiles.end())
				return false;
			outText = it->second;
			return true;
		}

	private:
		/*!
		"./" と "../" を取り除き、"\\" を "/" に揃えます

		対応表の外を指す "../" は捨てます。メモリ上には外側が存在しない為です。
		*/
		[[nodiscard]] static std::string Normalize(const std::string_view path)
		{
			std::vector<std::string> parts;
			std::string part;
			auto flush = [&parts, &part]()
			{
				if (part == "..")
				{
					if (!parts.empty())
						parts.pop_back();
				}
				else if (!part.empty() && part != ".")
				{
					parts.push_back(part);
				}
				part.clear();
			};

			for (const char c : path)
			{
				if (c == '/' || c == '\\')
					flush();
				else
					part.push_back(c);
			}
			flush();

			std::string normalized;
			for (const std::string& p : parts)
			{
				if (!normalized.empty())
					normalized.push_back('/');
				normalized += p;
			}
			return normalized;
		}

		std::map<std::string, std::string, std::less<>> mFiles;
	};
}
