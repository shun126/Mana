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

import glob
import subprocess
import os
import sys

if len(sys.argv) < 2:
	print('usage: python test.py <path to mana executable>', file=sys.stderr)
	print('  e.g. python test.py ../../build/mana', file=sys.stderr)
	print('  or run every suite with: ctest --test-dir build --output-on-failure', file=sys.stderr)
	sys.exit(2)
MANA = os.path.abspath(sys.argv[1])

# テストスクリプトは相対パスで渡すので、どこから起動されてもこのディレクトリで実行します
os.chdir(os.path.dirname(os.path.abspath(__file__)))
print("pwd:" + os.getcwd())

# コンパイルエラー時の終了コードです。クラッシュ（Windowsでは0xC0000005など）と区別する為に厳密に比較します
COMPILE_ERROR_EXIT_CODE = 1

Result = 0
Tested = set()

################################################################################
def run(command):
	cp = subprocess.run(command, capture_output=True, text=True, errors="ignore")
	cp.stdout = cp.stdout.replace('\r\n', '\n')
	cp.stderr = cp.stderr.replace('\r\n', '\n')
	return cp

def report_failure(cp, *reasons):
	print(" ... Failed")
	if cp.returncode < 0:
		print(f"Process killed by signal {-cp.returncode}")
	else:
		print(f"Process exited with code {cp.returncode}")
	for reason in reasons:
		print(reason)
	print('stdout :')
	print(cp.stdout)
	print('stderr :')
	print(cp.stderr)
	global Result
	Result += 1

def remove_generated_files(argument):
	for extension in ('.h', '.md'):
		path = os.path.splitext(argument)[0] + extension
		if os.path.exists(path):
			os.remove(path)

################################################################################
# コマンドラインのスイッチだけを試します
def option(argument, expected_prefix):
	print('option test: ' + MANA + ' ' + argument, end='')
	cp = run([MANA, argument])
	if cp.returncode == 0 and cp.stdout.startswith(expected_prefix):
		print(" ... Success")
	else:
		report_failure(cp, f"expected stdout to start with: {expected_prefix!r}")

################################################################################
# コンパイルと実行が成功し、標準出力が期待値と完全に一致する事を確かめます。
# 何も出力しないスクリプトは expected を省略します（空出力を確かめます）。
# -t と --debug で型宣言ヘッダーとダンプが生成される事も確かめます。
def success(argument, expected=''):
	Tested.add(argument)
	print('success test: ' + MANA + ' ' + argument, end='')
	remove_generated_files(argument)
	cp = run([MANA, argument, "-t", "--debug"])
	reasons = []
	if cp.returncode != 0:
		reasons.append('expected exit code 0')
	if cp.stdout != expected:
		reasons.append(f"expected stdout: {expected!r}")
	for extension in ('.h', '.md'):
		path = os.path.splitext(argument)[0] + extension
		if not os.path.isfile(path) or os.path.getsize(path) == 0:
			reasons.append(f"missing generated file: {path}")
	if reasons:
		report_failure(cp, *reasons)
	else:
		print(" ... Success")

################################################################################
# コンパイルエラーになり、期待する診断が出力される事を確かめます
def fail(argument, error_message):
	Tested.add(argument)
	print('fail test: ' + MANA + ' ' + argument, end='')
	cp = run([MANA, argument])
	reasons = []
	if cp.returncode != COMPILE_ERROR_EXIT_CODE:
		reasons.append(f"expected exit code {COMPILE_ERROR_EXIT_CODE}")
	if error_message not in cp.stdout and error_message not in cp.stderr:
		reasons.append(f"missing diagnostic: {error_message}")
	if reasons:
		report_failure(cp, *reasons)
	else:
		print(" ... Success")

################################################################################
option('--copyright', 'mana - Copyright (C) 2002-')
option('--version', 'mana ')
option('--help', 'usage:mana')
success('TestActionRefExpression.mn')
success('TestActor01.mn',
	'TestActor1::init\n'
	'TestActor1::main\n'
	'TestActor1::talk\n'
	'TestActor2::talk\n'
	'TestActor2::talk\n')
success('TestBool01.mn', 'success\n')
fail('TestConstDeclInvalidAssign.mn', 'cannot assign to const')
fail('TestConstDeclInvalidCall.mn', 'constant expression')
fail('TestConstDeclInvalidDefine.mn', 'syntax error')
fail('TestConstDeclInvalidType.mn', 'incompatible type')
success('TestConstDeclOk.mn', '1 3 3 4\n')
success('TestConstant01.mn')
success('TestExtendNamespaceResolution.mn')
success('TestForwardReferenceUsing.mn')
success('TestForwardUsingActor.mn')
success('TestForwardUsingNamespace.mn')
success('TestForwardUsingNamespaceNested.mn')
success('TestFunction01.mn')
fail('TestFunction02.mn', 'incomplete type name')
success('TestGlobalInit.mn',
	'constValue: 7\n'
	'constFloat: 1.500000\n'
	'orderedB: 2\n'
	'runtimeValue: 563\n')
fail('TestMissingFile.mn', 'unable to open')
fail('TestReturnMissingValue.mn', 'missing return value')
fail('TestReturnVoidValue.mn', 'meaningless return value specification')
success('TestNamespaceFqn.mn')
success('TestStatement.mn')
success('TestStatementDo.mn')
success('TestStatementFor.mn')
success('TestStatementIf.mn')
success('TestStatementLoop.mn')
success('TestStatementSwitch.mn')
success('TestStatementWhile.mn')
success('TestStruct01.mn')
success('TestStructMethod01.mn')
success('TestStructNativeMethod.mn')
fail('TestStructMethodNonStruct.mn', 'member call on non-struct type')
success('TestStructMethodRequest.mn')
fail('TestStructMethodUnknown.mn', "unresolved method 'Unknown' for type 'S'")
success('TestStructNamespace.mn')
fail('TestNativeFunctionBody.mn', 'syntax error')
fail('TestUsingAmbiguousActor.mn', 'ambiguous actor reference')
fail('TestUsingAmbiguousNamespaceSymbol.mn', 'ambiguous using')
fail('TestUsingAmbiguousType.mn', 'ambiguous type reference')
success('TestUsingNamespace.mn')
success('TestUsingResolveNamespacePath.mn')
success('TestUsingResolveSymbolImport.mn')
success('TestUsingScopeOrder.mn')
success('TestUsingSymbol.mn')
success('TestVariable01.mn')
success('TestVariable02.mn')
success('TestAddressArithmetic.mn',
	'field 11 22 33\n'
	'array 100 200 400\n'
	'copy 11 22 33\n')
# 実行時エラーはアクターを停止させるだけで、プロセスの終了コードは0のままです
success('TestRuntimeDivideByZero.mn',
	'before 10\n'
	'mana: actor TestDivideByZeroRoot halted: script error: division by zero\n')
success('TestRuntimeSubscriptRange.mn',
	'before 1\n'
	'mana: actor TestSubscriptRangeRoot halted: script error: '
	'subscript out of range: index 100 is outside the array of 4 element(s)\n')

################################################################################
# 追加したスクリプトの登録漏れで、テストが黙って実行されない事を防ぎます
unlisted = sorted(set(glob.glob('*.mn')) - Tested)
for name in unlisted:
	print(f"unlisted test: {name} is not run by test.py ... Failed")
	Result += 1

print(f"{Result} failure(s)")
exit(Result)
