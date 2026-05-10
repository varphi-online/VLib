BUILDDIR ?= build

vhttp.h: vhttp_h.h vhttp.c
	../stb.sh $@ VHTTP_IMPLEMENTATION vhttp_h.h vhttp.c

.PHONY: clean

clean:
	rm -rf build/
