# -*- coding: utf-8 -*-
#
# mana (compiler)
#
# @file		test.rb
# @brief	テストに関するスクリプトファイル
# @detail	このファイルはテストに関するスクリプトファイルです。
# @author	Shun Moriya
# @date		2017-
#

MANA = '../../compiler/mana'

def success(command)
	p system(MANA + ' ' + command)
	p $?.to_i
	#raise Exception if !system(MANA + ' ' + command)
	#raise Exception if $?.to_i != 0
end

def fail(command, error_message)
	IO.popen(MANA + ' ' + command, $stderr => [:child, $stdout]) { |pipe|
		output = pipe.read
		puts output
		#raise Exception if $?.to_i == 0
		raise Exception if !output.include?(error_message)
	}
end

begin
	success('--copyright')
	success('--version')
	success('--help')
#	fail('../missing_file.mn', 'unable to open')
	success('test_actor_01.mn')
rescue => e
	p e.to_s
end
