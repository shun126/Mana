# mana
# Copyright (c) 2003 Shun Moriya
#
# compiler : アプリケーションへ組み込めるコンパイラ本体 (libmana.a)
# driver   : コンパイラを制御するコマンドラインツール (mana)
#

.PHONY: all compiler driver clean depend cppcheck test

# Debug / Release の切り替え。設定を変えたら clean してから作り直してください
# (オブジェクトはタイムスタンプでしか再ビルドされず、CONFIG の違いは見ません)。
CONFIG ?= Release

all: driver

compiler:
	cd compiler && $(MAKE) CONFIG=$(CONFIG)

driver: compiler
	cd driver && $(MAKE) CONFIG=$(CONFIG)

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
	cd driver && $(MAKE) CONFIG=$(CONFIG) test
