## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

include $(top_srcdir)/Make_global.am

if ! WITH_KBUILDDIR
KBUILD_OUTPUT=/lib/modules/`uname -r`/build
else
KBUILD_OUTPUT=$(KBUILDDIR)
endif
if ! WITH_MAXSETS
IP_SET_MAX=256
else
IP_SET_MAX=$(MAXSETS)
endif
if ENABLE_VERBOSE
V=1
else
V=0
endif

SUBDIRS		= lib src

modules_sparse:
	${MAKE} -C $(KBUILD_OUTPUT) M=$$PWD/kernel/net/netfilter \
			V=$V C=2 CF=-D__CHECK_ENDIAN__ \
			IP_SET_MAX=$(IP_SET_MAX) KDIR=$$PWD/kernel modules

modules:
	${MAKE} -C $(KBUILD_OUTPUT) M=$$PWD/kernel/net/netfilter V=$V \
			IP_SET_MAX=$(IP_SET_MAX) KDIR=$$PWD/kernel modules

modules_install:
	${MAKE} -C $(KBUILD_OUTPUT) M=$$PWD/kernel/net/netfilter \
			KDIR=$$PWD/kernel modules_install

modules_clean:
	${MAKE} -C $(KBUILD_OUTPUT) M=$$PWD/kernel/net/netfilter \
			KDIR=$$PWD/kernel clean

update_includes:
	./update ip_set.h
	./update ip_set_bitmap.h
	./update ip_set_hash.h
	./update ip_set_list.h

tests:
	cd tests; ./runtest.sh

cleanup_dirs := . include/libipset lib src tests

tidy: distclean modules_clean
	rm -rf .deps $(foreach dir,$(cleanup_dirs),$(wildcard $(dir)/*~))
	rm -rf aclocal.m4 autom4te.cache 
	rm -rf config.* configure compile depcomp install-sh libtool ltmain.sh
	rm -rf Makefile Makefile.in lib/Makefile lib/Makefile.in src/Makefile src/Makefile.in
	rm -rf missing stamp-h1 m4/libtool.m4 m4/lt*.m4

.PHONY: modules modules_instal modules_clean update_includes tests
