# -*- coding: shift_jis -*-
#
# mana (compiler)
#
# @file		version.rb
# @brief	�o�[�W�������Ɋւ���X�N���v�g�t�@�C��
# @detail	���̃t�@�C���̓o�[�W�������Ɋւ���X�N���v�g�t�@�C���ł��B
# @author	Shun Moriya
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
/*!
mana (compiler)

@file	version.h
@brief	�o�[�W�������Ɋւ���w�b�_�[�t�@�C��
@detail	���̃t�@�C���̓o�[�W�������Ɋւ���w�b�_�[�t�@�C���ł��B
		version.rb�ɂ�莩�����������̂Œ��ӂ��ĉ�����
@author	Shun Moriya
@date	2003-
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
