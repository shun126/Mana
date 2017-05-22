# -*- coding: utf-8 -*-
#
# mana (compiler)
#
# @file		version.rb
# @brief	バージョン情報に関するスクリプトファイル
# @detail	このファイルはバージョン情報に関するスクリプトファイルです。
# @author	Shun Moriya
# @date		2017-
#

import json
import datetime

json_file = open('Version.json', 'r')
if json_file:
	json_object = json.load(json_file)
	if json_object:
		major_version = json_object["MAJOR_VERSION"]
		minor_version = json_object["MINOR_VERSION"]

		header_file = open('Version.h', 'w')
		if header_file:
			now = datetime.datetime.now()
			body = '''/*!
mana (compiler)

@file	Version.h
@brief	バージョン情報に関するヘッダーファイル
@detail	このファイルはバージョン情報に関するヘッダーファイルです。
		Version.rbにより自動生成されるので注意して下さい
@author	Shun Moriya
@date	2003-
*/

#pragma once
#include <cstdint>
namespace mana
{{
	namespace build
	{{
		static constexpr int32_t Year = {year};
		static constexpr int32_t Month = {month};
		static constexpr int32_t Day = {day};
		static constexpr int32_t Hour = {hour};
		static constexpr int32_t Minute = {minute};
		static const char* MajorVersion = "{_major_version}";
		static const char* MinorVersion = "{_minor_version}";
	}}
}}
'''.format(year = now.year, month = now.month, day = now.day, hour = now.hour, minute = now.minute, _major_version = major_version, _minor_version = minor_version).strip()
			header_file.write(body)
		else:
			print("version.h file open failed.")
	else:
		print("parse error in version.json")
else:
	print("version.json file open failed.")
