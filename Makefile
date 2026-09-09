# mana
# Copyright (c) 2003 Shun Moriya
#
# compiler : アプリケーションへ組み込めるコンパイラ本体 (libmana.a)
# driver   : コンパイラを制御するコマンドラインツール (mana)
#

.PHONY: all compiler driver clean depend cppcheck test

all: driver

compiler:
	cd compiler && $(MAKE)

driver: compiler
	cd driver && $(MAKE)

clean:
	cd driver && $(MAKE) clean
	cd compiler && $(MAKE) clean

depend:
	cd compiler && $(MAKE) depend
	cd driver && $(MAKE) depend

cppcheck:
	cd compiler && $(MAKE) cppcheck
	cd driver && $(MAKE) cppcheck

test: driver
	cd driver && $(MAKE) test
