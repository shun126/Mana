# -*- coding: utf-8 -*-
#
# mana (compiler)
#
# @file		check_output.py
# @brief	Runs a command and compares its standard output with a file
# @detail	Used by CTest for tests whose whole output is known.
#			usage: python check_output.py <expected file> <command> [arguments...]
# @author	Shun Moriya
# @date		2026-
#

import os
import subprocess
import sys

if len(sys.argv) < 3:
	print('usage: python check_output.py <expected file> <command> [arguments...]', file=sys.stderr)
	sys.exit(2)

with open(sys.argv[1], encoding='utf-8') as file:
	expected = file.read().replace('\r\n', '\n')

command = sys.argv[2:]
if os.path.exists(command[0]):
	command[0] = os.path.abspath(command[0])
cp = subprocess.run(command, capture_output=True, text=True, encoding='utf-8', errors='replace')
actual = cp.stdout.replace('\r\n', '\n')

if cp.returncode != 0:
	print(f'exit code {cp.returncode}, expected 0')
if actual != expected:
	expected_lines = expected.splitlines()
	actual_lines = actual.splitlines()
	for index in range(max(len(expected_lines), len(actual_lines))):
		e = expected_lines[index] if index < len(expected_lines) else '<missing>'
		a = actual_lines[index] if index < len(actual_lines) else '<missing>'
		if e != a:
			print(f'first difference at line {index + 1}:')
			print(f'  expected: {e!r}')
			print(f'  actual  : {a!r}')
			break
	else:
		print('output differs only in trailing newlines')
if cp.returncode != 0 or actual != expected:
	print('stderr :')
	print(cp.stderr)
	sys.exit(1)
print('output matches ' + sys.argv[1])
