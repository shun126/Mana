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
import sys

cwd = os.getcwd()
print("pwd:" + cwd)

if len(sys.argv) < 2:
	MANA = '../compiler/mana'
else:
	MANA = sys.argv[1]

################################################################################
def success(argument):
	print(MANA + ' ' + argument, end='')
	command = [MANA, argument]
	cp = subprocess.run(command, capture_output=True, text=True, errors="ignore")
	if cp.returncode == 0:
		print(" ... Success")
	else:
		print(" ... Failed")
		if cp.returncode < 0:
			print(f"Process killed by signal {-cp.returncode}")
		else:
			print(f"Process exited with code {cp.returncode}")
		print('stdout :')
		if len(cp.stdout) > 0:
			print(cp.stdout)
		print('stderr :')
		if len(cp.stderr) > 0:
			print(cp.stderr)
		global Result
		Result += 1

################################################################################
def fail(argument, error_message):
	print(MANA + ' ' + argument, end='')
	command = [MANA, argument]
	cp = subprocess.run(command, capture_output=True, text=True, errors="ignore")
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
success('test_bool_01.mn')
#success('test_function_01.mn')
fail('test_function_02.mn', 'incomplete type name')
success('test_struct_01.mn')
success('test_struct_namespace.mn')
success('test_variable_01.mn')
success('test_global_init.mn')
success('test_namespace_fqn.mn')
success('test_action_ref_expression.mn')
success('test_using_namespace.mn')
success('test_using_symbol.mn')
success('test_forward_reference_using.mn')
success('test_forward_using_namespace.mn')
success('test_forward_using_namespace_nested.mn')
success('test_forward_using_actor.mn')
success('test_using_resolve_namespace_path.mn')
success('test_using_resolve_symbol_import.mn')
success('test_using_scope_order.mn')
fail('test_using_ambiguous_actor.mn', 'ambiguous actor reference')
fail('test_using_ambiguous_namespace_symbol.mn', 'ambiguous using')
success('test_extend_namespace_resolution.mn')
################################################################################
exit(Result)
