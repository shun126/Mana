# mana
# Copyright (c) 2009 Shun Moriya <shun126@users.sourceforge.jp>

MANA	= ../../../../compiler/trunk

CP	= cp -f
RM	= rm -f

TARGET	= sample.mx
SOURCES	= sample.mn

$(TARGET): $(SOURCES)
	make -C $(MANA)
	$(MANA)/mana $(SOURCES)

clean:
	make -C $(MANA) clean
	$(RM) $(TARGET)
