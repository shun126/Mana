# -*- coding: shift_jis -*-
#
# mana (compiler)
#
# @file		mana_version.rb
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

json = open('mana_version.json') do |io|
  JSON.load(io)
end
major_version = json["MAJOR_VERSION"]
minor_version = json["MINOR_VERSION"]

File.open("mana_version.h", "w") do |file|
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

#if !defined(___MANA_VERSION_H___)
#define ___MANA_VERSION_H___

#define MANA_BUILD_YEAR #{year}
#define MANA_BUILD_MONTH #{month}
#define MANA_BUILD_DAY #{day}
#define MANA_BUILD_MAJOR_VERSION "#{major_version}"
#define MANA_BUILD_MINOR_VERSION "#{minor_version}"

#endif
EOS
	file.puts(body)
end
