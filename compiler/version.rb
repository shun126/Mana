# -*- coding: shift_jis -*-
#
# mana (compiler)
#
# @file		version.rb
# @brief	バージョン情報に関するスクリプトファイル
# @detail	このファイルはバージョン情報に関するスクリプトファイルです。
# @author	Shun Moriya <shun@mnu.sakura.ne.jp>
# @date		2003-
#/

require 'date'
require 'json'

now = Time.now
year = now.year
month = now.month
day = now.day

json = open('version.json') do |io|
  JSON.load(io)
end
major_version = json["MAJOR_VERSION"]
minor_version = json["MINOR_VERSION"]

File.open("version.h", "w") do |file|
	body = <<EOS
/*
 * mana (compiler)
 *
 * @file	mana_version.h
 * @brief	バージョン情報に関するヘッダーファイル
 * @detail	このファイルはバージョン情報に関するヘッダーファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___VERSION_H___)
#define ___VERSION_H___

#define BUILD_YEAR #{year}
#define BUILD_MONTH #{month}
#define BUILD_DAY #{day}
#define BUILD_MAJOR_VERSION "#{major_version}"
#define BUILD_MINOR_VERSION "#{minor_version}"

#endif
EOS
	file.puts(body)
end
