SUBDIRS = core machine devices #runtime

all:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d || exit 1; \
	done

clean:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d clean || exit 1; \
	done

.PHONY: all clean
