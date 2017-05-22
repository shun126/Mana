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

$result = 0

################################################################################
def dump_exit_status(status)
#	p status
	if status != nil then
		puts status.inspect
#		puts 'exit status: ' + status.exitstatus.to_s
	end
end

################################################################################
def success(argument)
	command = MANA + ' ' + argument

	print command

	IO.popen(command, $stderr => [:child, $stdout]) { |pipe|
		output = pipe.read
		pipe.close_write
		
		if $? != nil && $?.exited? && $?.exitstatus == 0 then
			puts ' ... Success'
		else
			puts ' ... Failed'
			puts output
			$result += 1
		end

		#dump_exit_status($?)
	}
end

################################################################################
def fail(argument, error_message)
	command = MANA + ' ' + argument

	print command

	IO.popen(command, $stderr => [:child, $stdout]) { |pipe|
		output = pipe.read
		pipe.close_write
		
		if $? != nil && $?.exited? then
			if $?.exitstatus != 0 && output.include?(error_message)
				puts ' ... Success'
			else
				puts ' ... Failed'
				puts output
				$result += 1
			end
		end

		#dump_exit_status($?)
	}
end

################################################################################
success('--copyright')
success('--version')
success('--help')
#fail('test_missing_file.mn', 'unable to open')
success('test_actor_01.mn')
success('test_constant_01.mn')
#success('test_function_01.mn')
fail('test_function_02.mn', 'incomplete type name')
success('test_struct_01.mn')
success('test_variable_01.mn')

################################################################################
exit $result
