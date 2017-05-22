# -*- coding: utf-8 -*-
#
# mana (compiler)
#
# @file		test.py
# @brief	Script files related to testing
# @detail	This file is a script file about testing.
# @author	Shun Moriya
# @date		2017-
#

import subprocess
import os

cwd = os.getcwd()
print("pwd:" + cwd)

MANA = '../compiler/mana'

################################################################################
def success(argument):
	print(MANA + ' ' + argument, end='')
	command = [MANA, argument]
	cp = subprocess.run(command, capture_output=True, text=True)
	if cp.returncode == 0:
		print(" ... Success")
	else:
		print(" ... Failed")
		print('stdout :')
		print(cp.stdout)
		print('stderr :')
		print(cp.stderr)
		global Result
		Result += 1

################################################################################
def fail(argument, error_message):
	print(MANA + ' ' + argument, end='')
	command = [MANA, argument]
	cp = subprocess.run(command, capture_output=True, text=True)
	#if cp.returncode != 0 and (error_message in cp.stdout or error_message in cp.stderr):
	if (error_message in cp.stdout or error_message in cp.stderr):
		print(" ... OK")
	else:
		print(" ... NG")
		print('stdout :')
		print(cp.stdout)
		print('stderr :')
		print(cp.stderr)
		global Result
		Result += 1

################################################################################
Result = 0
success('--copyright')
success('--version')
success('--help')
fail('test_missing_file.mn', 'unable to open')
success('test_actor_01.mn')
success('test_constant_01.mn')
#success('test_function_01.mn')
fail('test_function_02.mn', 'incomplete type name')
success('test_struct_01.mn')
success('test_variable_01.mn')

################################################################################
exit(Result)
