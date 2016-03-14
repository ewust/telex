include $(top_srcdir)/Make_global.am

AM_CFLAGS += ${libmnl_CFLAGS}

lib_LTLIBRARIES = libipset.la

libipset_la_LDFLAGS = -version-info $(LIBVERSION)
libipset_la_LIBADD  = ${libmnl_LIBS}
libipset_la_SOURCES = \
	data.c \
	icmp.c \
	icmpv6.c \
	mnl.c \
	parse.c \
	print.c \
	session.c \
	types.c

#%.o: %.c
#	${AM_VERBOSE_CC} ${CC} ${AM_DEPFLAGS} ${AM_CFLAGS} ${CFLAGS} -o $@ -c $<
