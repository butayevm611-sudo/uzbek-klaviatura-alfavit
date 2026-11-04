CC      = gcc
CFLAGS  = -O2 -Wall -Wextra
LDLIBS  = -l:libxkbcommon.so.0

xkbtest: xkbtest.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

test: xkbtest
	./xkbtest
	./xkbtest "Özbekiston, şahar, ğalla, çiroq, maʼno"

clean:
	rm -f xkbtest

.PHONY: test clean
