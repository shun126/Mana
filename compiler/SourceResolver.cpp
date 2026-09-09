/*!
mana (compiler)

@file	SourceResolver.cpp
@brief	ソースの供給元に関係するソースファイル
@detail	このファイルはコンパイラへソースを供給する仕組みのソースファイルです。
@author	Shun Moriya
@date	2026-
*/

#include "SourceResolver.h"
#include "Path.h"
#include <fstream>
#include <sstream>

namespace mana
{
	namespace
	{
		/*!
		絶対位置かどうかを判定します
		*/
		[[nodiscard]] bool IsAbsolute(const std::string_view path)
		{
			if (path.empty())
				return false;

			if (path[0] == '/' || path[0] == '\\')
				return true;

#if defined(MANA_TARGET_WINDOWS)
			// ドライブレターから始まる位置
			if (path.size() >= 3 && path[1] == ':' && (path[2] == '/' || path[2] == '\\'))
				return true;
#endif
			return false;
		}

		/*!
		ファイルの位置から、それが置かれているディレクトリを取り出します
		*/
		[[nodiscard]] std::string DirectoryOf(const std::string_view path)
		{
			if (path.empty())
				return std::string();

			const std::string source(path);

			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];
			char fileName[_MAX_FNAME];
			char extension[_MAX_EXT];
			char directory[_MAX_PATH];

			splitpath(source.c_str(),
				drive, sizeof(drive), dir, sizeof(dir),
				fileName, sizeof(fileName), extension, sizeof(extension));
			makepath(directory, sizeof(directory), drive, dir, "", "");

			return std::string(directory);
		}
	}

	std::string FileSourceResolver::Resolve(const std::string_view from, const std::string_view filename) const
	{
		if (filename.empty())
			return std::string();

		std::string candidate;
		if (IsAbsolute(filename))
		{
			candidate.assign(filename);
		}
		else
		{
			// 読み込んでいる側のディレクトリを基準にします。
			// 最初のソースは読み込んでいる側が無いので作業ディレクトリを基準にします。
			std::string base = from.empty() ? getcurrentdirectory() : DirectoryOf(from);
			if (!base.empty() && base.back() != '/' && base.back() != '\\')
			{
				base += PathSeparator();
			}
			candidate = base;
			candidate.append(filename);
		}

		// 正規化できない場合でも、診断に表示できるよう組み立てた位置を返します
		char resolved[_MAX_PATH];
		if (fullpath(resolved, candidate.c_str(), sizeof(resolved)) != nullptr)
		{
			return std::string(resolved);
		}
		return candidate;
	}

	bool FileSourceResolver::Read(const std::string_view path, std::string& outText) const
	{
		if (path.empty())
			return false;

		// 改行コードはコンパイラ側で正規化するのでバイナリで読み込みます
		std::ifstream file(std::string(path), std::ios::in | std::ios::binary);
		if (!file.is_open())
			return false;

		std::ostringstream stream;
		stream << file.rdbuf();
		outText = stream.str();

		return true;
	}
}
