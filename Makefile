BUILDDIR := $(abspath build)
SUBDIRS := $(patsubst %/Makefile,%,$(wildcard */Makefile))
HEADERS := $(addprefix $(BUILDDIR)/,$(addsuffix .h,$(notdir $(SUBDIRS))))

.PHONY: all clean

all: $(HEADERS)

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/%.h: %/Makefile | $(BUILDDIR)
	$(MAKE) -C $* $*.h
	cp -f $*/$*.h $@

clean:
	rm -rf $(BUILDDIR)
	for d in $(SUBDIRS); do \
		$(MAKE) -C $$d clean; \
	done
