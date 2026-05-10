GPERFCMD ?= ./gperf
GPERF_UNDEF := \#undef TOTAL_KEYWORDS\n\#undef MIN_WORD_LENGTH\n\#undef MAX_WORD_LENGTH\n\#undef MIN_HASH_VALUE\n\#undef MAX_HASH_VALUE
BUILDDIR ?= build


# STB style header
vhtml.h: vhtml_h.h vhtml.c $(BUILDDIR)/codegen/tags.c $(BUILDDIR)/codegen/attributes.c
	../stb.sh $@ VHTML_IMPLEMENTATION vhtml_h.h $(BUILDDIR)/codegen/tags.c --text "$(GPERF_UNDEF)" $(BUILDDIR)/codegen/attributes.c vhtml.c

.PHONY: all

all: loop perf debug

debug: test/test.c vhtml_h.h vhtml.c $(BUILDDIR)/codegen/tags.c $(BUILDDIR)/codegen/attributes.c
	gcc -I. vhtml.c $(BUILDDIR)/codegen/tags.c $(BUILDDIR)/codegen/attributes.c test/test.c -g -o $(BUILDDIR)/bin/test

loop: test/loop.c vhtml.h | $(BUILDDIR)
	gcc test/loop.c -O3 -march=native -o $(BUILDDIR)/bin/loop

perf: test/loop.c vhtml.h | $(BUILDDIR)
	gcc test/loop.c -fno-omit-frame-pointer -march=native -g -o $(BUILDDIR)/bin/perf_test

$(BUILDDIR)/codegen/tags.c: tags.gperf | $(BUILDDIR)
	$(GPERFCMD) tags.gperf > $@

$(BUILDDIR)/codegen/attributes.c: attributes.gperf | $(BUILDDIR)
	mkdir -p build
	$(GPERFCMD) attributes.gperf > $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)
	mkdir -p $(BUILDDIR)/codegen
	mkdir -p $(BUILDDIR)/bin

.PHONY: clean

clean:
	rm -rf build perf.data
