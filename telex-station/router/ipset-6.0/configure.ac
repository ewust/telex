dnl Boilerplate
AC_INIT([ipset], [6.0], [kadlec@blackhole.kfki.hu])
AC_CANONICAL_HOST
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects])

dnl Shortcut: Linux supported alone
case "$host" in
*-*-linux*) ;;
*) AC_MSG_ERROR([Linux systems supported exclusively!]);;
esac

dnl Additional arguments
dnl Kernel build directory or source tree
AC_ARG_WITH([kbuild],
            AS_HELP_STRING([--with-kbuild=PATH],
                           [Path to kernel build directory]),
            [KBUILDDIR="$withval";])
AC_ARG_WITH([ksource],
            AS_HELP_STRING([--with-ksource=PATH],
                           [Path to kernel source directory, if not the same as the kernel build directory]),
            [KSOURCEDIR="$withval";])
AM_CONDITIONAL(WITH_KBUILDDIR, test "$KBUILDDIR" != "")
AC_SUBST(KBUILDDIR)

dnl Sigh: check kernel version dependencies
if test "$KBUILDDIR" != ""
then
	kbuilddir="$KBUILDDIR"
else
	kbuilddir="/lib/modules/`uname -r`/build"
fi

if test -n "$KSOURCEDIR"; then
	ksourcedir="$KSOURCEDIR"
elif test -e "$kbuilddir/include/linux/netfilter/nfnetlink.h"; then
	ksourcedir="$kbuilddir"
else
	ksourcedir="/lib/modules/$(uname -r)/source"
fi
if test ! -e "$ksourcedir/include/linux/netfilter/nfnetlink.h"
then
	AC_MSG_ERROR([Invalid kernel source directory $ksourcedir])
fi

if test ! -e "$kbuilddir/.config"
then
	AC_MSG_ERROR([The kernel build directory $kbuilddir is not configured])
fi

AC_PROG_GREP

if test "X`$GREP '^CONFIG_IP6_NF_IPTABLES=' $kbuilddir/.config`" = "X"
then
	AC_MSG_ERROR([The kernel build directory $kbuilddir is not configured with IP6_NF_IPTABLES support (ip6tables)])
fi

if test "X`$GREP 'NFNL_SUBSYS_IPSET' $ksourcedir/include/linux/netfilter/nfnetlink.h`" = "X"
then
	AC_MSG_ERROR([The kernel source directory $ksourcedir is not patched with netlink.patch to support ipset])
fi

dnl Maximal number of sets supported by the kernel, default 256
AC_ARG_WITH([maxsets],
	    AS_HELP_STRING([--with-maxsets=256],
	    		   [Maximal numer of sets supported by the kernel]),
	    [MAXSETS="$withval";])
AM_CONDITIONAL(WITH_MAXSETS, test "$MAXSETS" != "")
AC_SUBST(MAXSETS)

dnl Verbose compiling
AC_ARG_ENABLE([verbose],
	      AS_HELP_STRING([--enable-verbose],
	                     [Enable verbose mode at compiling/linking.]),
	      [case "${enableval}" in
	       yes)	enable_verbose=yes ;;
	       no)	enable_verbose=no ;;
	       *) AC_MSG_ERROR([bad value ${enableval} for --enable-verbose]) ;;
	       esac], [enable_verbose=no])
	      
AM_CONDITIONAL([ENABLE_VERBOSE], [test "x$enable_verbose" = xyes])

dnl Enable debugging
AC_ARG_ENABLE([debug],
	      AS_HELP_STRING([--enable-debug],
	                     [Enable compiling with debug support.]),
	      [case "${enableval}" in
	       yes)	enable_verbose=yes ;;
	       no)	enable_verbose=no ;;
	       *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
	       esac], [enable_debug=no])
	      
AM_CONDITIONAL([ENABLE_DEBUG], [test "x$enable_debug" = xyes])

dnl Checks for programs
: ${CFLAGS=""}

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S

dnl Checks for libraries
PKG_CHECK_MODULES([libmnl], [libmnl >= 1])

dnl Checks for header files

dnl Checks for declarations
AC_CHECK_DECLS([NLA_F_NESTED, NLA_F_NET_BYTEORDER, NLA_TYPE_MASK],,
		[AC_MSG_ERROR([System kernel header files are older than 2.6.24, use CFLAGS for non-default location])],
		[#include <sys/socket.h>
#include <linux/netlink.h>])

dnl Checks for typedefs, structures
AC_CHECK_TYPES([union nf_inet_addr],,,[#include <linux/types.h>
#include <netinet/in.h>
#include <linux/netfilter.h>])

dnl Checks for  compiler characteristics.
dnl Check extra warning flags except
dnl	-Wconversion		-> we need it
dnl	-Wunreachable-code	-> fails with ntoh*
dnl	-Wpointer-arith		-> limbnl uses it
if test "x$enable_debug" = "xyes"
then
AX_CFLAGS_GCC_OPTION(-Waggregate-return)
AX_CFLAGS_GCC_OPTION(-Wbad-function-cast)
AX_CFLAGS_GCC_OPTION(-Wcast-align)
AX_CFLAGS_GCC_OPTION(-Wcast-qual)
AX_CFLAGS_GCC_OPTION(-Werror)
AX_CFLAGS_GCC_OPTION(-Wextra)
AX_CFLAGS_GCC_OPTION(-Wfloat-equal)
AX_CFLAGS_GCC_OPTION(-Wformat=2)
AX_CFLAGS_GCC_OPTION(-Winit-self)
AX_CFLAGS_GCC_OPTION(-Winline)
AX_CFLAGS_GCC_OPTION(-Wmissing-declarations)
AX_CFLAGS_GCC_OPTION(-Wmissing-format-attribute)
AX_CFLAGS_GCC_OPTION(-Wmissing-prototypes)
AX_CFLAGS_GCC_OPTION(-Wnested-externs)
AX_CFLAGS_GCC_OPTION(-Wno-missing-field-initializers)
AX_CFLAGS_GCC_OPTION(-Woverlength-strings)
AX_CFLAGS_GCC_OPTION(-Wpacked)
AX_CFLAGS_GCC_OPTION(-Wredundant-decls)
AX_CFLAGS_GCC_OPTION(-Wshadow)
AX_CFLAGS_GCC_OPTION(-Wsign-compare)
AX_CFLAGS_GCC_OPTION(-Wstrict-prototypes)
AX_CFLAGS_GCC_OPTION(-Wswitch-default)
AX_CFLAGS_GCC_OPTION(-Wundef)
AX_CFLAGS_GCC_OPTION(-Wunused)
AX_CFLAGS_GCC_OPTION(-Wwrite-strings)
fi
dnl Checks for library functions.

dnl Generate output
AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile])
AC_OUTPUT
