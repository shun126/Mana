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
	command = [MANA, argument, "--debug"]
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
		print(" ... Success")
	else:
		print(" ... Failed")
		if cp.returncode < 0:
			print(f"Process killed by signal {-cp.returncode}")
		else:
			print(f"Process exited with code {cp.returncode}")
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
fail('TestMissingFile.mn', 'unable to open')
success('TestActor01.mn')
success('TestConstant01.mn')
success('TestConstDeclOk.mn')
success('TestBool01.mn')
#success('TestFunction01.mn')
fail('TestFunction02.mn', 'incomplete type name')
success('TestStruct01.mn')
success('TestStructMethod01.mn')
success('TestStructMethodRequest.mn')
success('TestStructNamespace.mn')
success('TestVariable01.mn')
success('TestGlobalInit.mn')
success('TestNamespaceFqn.mn')
success('TestActionRefExpression.mn')
success('TestUsingNamespace.mn')
success('TestUsingSymbol.mn')
success('TestForwardReferenceUsing.mn')
success('TestForwardUsingNamespace.mn')
success('TestForwardUsingNamespaceNested.mn')
success('TestForwardUsingActor.mn')
success('TestUsingResolveNamespacePath.mn')
success('TestUsingResolveSymbolImport.mn')
success('TestUsingScopeOrder.mn')
fail('TestUsingAmbiguousActor.mn', 'ambiguous actor reference')
fail('TestUsingAmbiguousType.mn', 'ambiguous type reference')
fail('TestUsingAmbiguousNamespaceSymbol.mn', 'ambiguous using')
success('TestExtendNamespaceResolution.mn')
fail('TestStructMethodNonStruct.mn', 'member call on non-struct type')
fail('TestStructMethodUnknown.mn', "unresolved method 'Unknown' for type 'S'")
fail('TestConstDeclInvalidDefine.mn', 'syntax error')
fail('TestConstDeclInvalidCall.mn', 'constant expression')
fail('TestConstDeclInvalidType.mn', 'incompatible type')
fail('TestConstDeclInvalidAssign.mn', 'cannot assign to const')
################################################################################
exit(Result)
