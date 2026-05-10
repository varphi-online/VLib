BUILDDIR ?= build

vjson.h: vjson_h.h vjson.c
	../stb.sh $@ VJSON_IMPLEMENTATION vjson_h.h vjson.c

.PHONY: clean

clean:
	rm -rf build/
