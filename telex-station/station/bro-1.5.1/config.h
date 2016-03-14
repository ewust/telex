/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/* Enable active mapping processing */
/* #undef ACTIVE_MAPPING */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* basic_string not usable with non-char template arg */
/* #undef BASIC_STRING_BROKEN */

/* enable IPV6 processing */
/* #undef BROv6 */

/* Old libpcap versions (< 0.6.1) need defining pcap_freecode and
   pcap_compile_nopcap */
/* #undef DONT_HAVE_LIBPCAP_PCAP_FREECODE */

/* should explicitly declare socket() and friends */
/* #undef DO_SOCK_DECL */

/* Enable DFA state expiration */
/* #undef EXPIRE_DFA_STATES */

/* Define to 1 if you have the bpf_set_bufsize function. */
/* #undef HAVE_BPF_SET_BUFSIZE */

/* Define to 1 if you have the <curses.h> header file. */
/* #undef HAVE_CURSES_H */

/* Define to 1 if you have the <GeoIPCity.h> header file. */
/* #undef HAVE_GEOIPCITY_H */

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the `getopt_long' function. */
#define HAVE_GETOPT_LONG 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `GeoIP' library (-lGeoIP). */
/* #undef HAVE_LIBGEOIP */

/* Define to 1 if you have the `magic' library (-lmagic). */
/* #undef HAVE_LIBMAGIC */

/* Define to 1 if you have the `nsl' library (-lnsl). */
/* #undef HAVE_LIBNSL */

/* Define to 1 if you have the pcap library (-lpcap). */
#define HAVE_LIBPCAP 1

/* Define to 1 if you have the `readline' library (-lreadline). */
/* #undef HAVE_LIBREADLINE */

/* Define to 1 if you have the `resolv' library (-lresolv). */
/* #undef HAVE_LIBRESOLV */

/* Define to 1 if you have the `socket' library (-lsocket). */
/* #undef HAVE_LIBSOCKET */

/* Define to 1 if you have the `ssl' library (-lssl). */
#define HAVE_LIBSSL 1

/* Define to 1 if you have the `str' library (-lstr). */
/* #undef HAVE_LIBSTR */

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 1

/* We are on a Linux system */
#define HAVE_LINUX /**/

/* Define to 1 if you have the <magic.h> header file. */
/* #undef HAVE_MAGIC_H */

/* Define to 1 if you have the `mallinfo' function. */
#define HAVE_MALLINFO 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* async dns support */
#define HAVE_NB_DNS /**/

/* Define to 1 if you have the <ncurses.h> header file. */
/* #undef HAVE_NCURSES_H */

/* Define to 1 if you have the <netinet/ether.h> header file. */
#define HAVE_NETINET_ETHER_H 1

/* Define to 1 if you have the <netinet/if_ether.h> header file. */
#define HAVE_NETINET_IF_ETHER_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <netinet/ip6.h> header file. */
#define HAVE_NETINET_IP6_H 1

/* Define to 1 if you have the <net/ethernet.h> header file. */
#define HAVE_NET_ETHERNET_H 1

/* We are on a OpenBSD system */
/* #undef HAVE_OPENBSD */

/* have os-proto.h */
/* #undef HAVE_OS_PROTO_H */

/* Define to 1 if you have the <pcap-int.h> header file. */
/* #undef HAVE_PCAP_INT_H */

/* Define to 1 if you have the `pfopen' function. */
/* #undef HAVE_PFOPEN */

/* line editing & history powers */
/* #undef HAVE_READLINE */

/* Define to 1 if you have the `sigaction' function. */
/* #undef HAVE_SIGACTION */

/* Define to 1 if you have the `sigset' function. */
#define HAVE_SIGSET 1

/* Define to 1 if you have the <socket.h> header file. */
/* #undef HAVE_SOCKET_H */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasestr' function. */
#define HAVE_STRCASESTR 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the `strftime' function. */
#define HAVE_STRFTIME 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strsep' function. */
#define HAVE_STRSEP 1

/* Define to 1 if the system has the type `struct sembuf'. */
#define HAVE_STRUCT_SEMBUF 1

/* Define to 1 if you have the <sys/ethernet.h> header file. */
/* #undef HAVE_SYS_ETHERNET_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <termcap.h> header file. */
#define HAVE_TERMCAP_H 1

/* Define to 1 if you have the <term.h> header file. */
#define HAVE_TERM_H 1

/* Define to 1 if the system has the type `union semun'. */
/* #undef HAVE_UNION_SEMUN */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Some libpcap versions use an extra parameter (error) in pcap_compile_nopcap
   */
/* #undef LIBPCAP_PCAP_COMPILE_NOPCAP_HAS_ERROR_PARAMETER */

/* Include krb5.h */
/* #undef NEED_KRB5_H */

/* Compatibility for Darwin */
/* #undef NEED_NAMESER_COMPAT_H */

/* d2i_x509 uses const char** */
#define OPENSSL_D2I_X509_USES_CONST_CHAR /**/

/* Name of package */
#define PACKAGE "bro"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Have a version string in libpcap */
#define PCAP_VERSION_STRING /**/

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* signal function return value */
#define RETSIGVAL /**/

/* have sin_len field in sockaddr_in */
/* #undef SIN_LEN */

/* The size of `long int', as computed by sizeof. */
#define SIZEOF_LONG_INT 8

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `void *', as computed by sizeof. */
#define SIZEOF_VOID_P 8

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* should we declare syslog() and openlog() */
/* #undef SYSLOG_INT */

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Include Endace DAG support */
/* #undef USE_DAG */

/* GeoIP geographic lookup functionality */
/* #undef USE_GEOIP */

/* enable use of 64-bit integers */
/* #undef USE_INT64 */

/* Use libclamav */
/* #undef USE_LIBCLAMAV */

/* Use OpenSSL */
#define USE_OPENSSL /**/

/* Use Google's perftools */
/* #undef USE_PERFTOOLS */

/* Use select-based main loop */
#define USE_SELECT_LOOP /**/

/* Version number of package */
#define VERSION "1.5.1"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#define YYTEXT_POINTER 1

/* irix's BSD style signals */
/* #undef _BSD_SIGNALS */

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* HP-UX ansi compiler */
/* #undef _HPUX_SOURCE */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* ultrix can't hack const */
/* #undef const */

/* Define int32_t */
/* #undef int32_t */

/* use sigset() instead of signal() */
#define signal sigset

/* define to int if socklen_t not available */
/* #undef socklen_t */

/* Define u_int16_t */
/* #undef u_int16_t */

/* Define u_int32_t */
/* #undef u_int32_t */

/* Define u_int8_t */
/* #undef u_int8_t */
