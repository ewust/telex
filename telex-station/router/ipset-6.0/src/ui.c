/* Copyright 2007-2010 Jozsef Kadlecsik (kadlec@blackhole.kfki.hu)
 *
 * This program is free software; you can redistribute it and/or modify   
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 */
#include <assert.h>				/* assert */
#include <ctype.h>				/* tolower */
#include <string.h>				/* memcmp, str* */

#include <libipset/linux_ip_set.h>		/* IPSET_CMD_* */
#include <libipset/icmp.h>			/* id_to_icmp */
#include <libipset/icmpv6.h>			/* id_to_icmpv6 */
#include <libipset/types.h>			/* IPSET_*_ARG */
#include <libipset/session.h>			/* ipset_envopt_parse */
#include <libipset/parse.h>			/* ipset_parse_family */
#include <libipset/print.h>			/* ipset_print_family */
#include <libipset/utils.h>			/* STREQ */
#include <libipset/ui.h>			/* prototypes */

/* Commands and environment options */

const struct ipset_commands ipset_commands[] = {
	/* Order is important */

	{	/* c[reate], --create, n[ew], -N */
		.cmd = IPSET_CMD_CREATE,
		.name = { "create", "new" },
		.has_arg = IPSET_MANDATORY_ARG2,
		.help = "SETNAME TYPENAME [type-specific-options]\n"
			"        Create a new set",
	},
	{	/* a[dd], --add, -A  */
		.cmd = IPSET_CMD_ADD,
		.name = { "add", NULL },
		.has_arg = IPSET_MANDATORY_ARG2,
		.help = "SETNAME ENTRY\n"
		        "        Add entry to the named set",
	},
	{	/* d[el], --del, -D */
		.cmd = IPSET_CMD_DEL,
		.name = { "del", NULL },
		.has_arg = IPSET_MANDATORY_ARG2,
		.help = "SETNAME ENTRY\n"
		        "        Delete entry from the named set",
	},
	{	/* t[est], --test, -T */
		.cmd = IPSET_CMD_TEST,
		.name = { "test", NULL },
		.has_arg = IPSET_MANDATORY_ARG2,
		.help = "SETNAME ENTRY\n"
		        "        Test entry in the named set",
	},
	{	/* des[troy], --destroy, x, -X */
		.cmd = IPSET_CMD_DESTROY,
		.name = { "destroy", "x" },
		.has_arg = IPSET_OPTIONAL_ARG,
		.help = "[SETNAME]\n"
		        "        Destroy a named set or all sets",
	},
	{	/* l[ist], --list, -L */
		.cmd = IPSET_CMD_LIST,
		.name = { "list", NULL },
		.has_arg = IPSET_OPTIONAL_ARG,
		.help = "[SETNAME]\n"
		        "        List the entries of a named set or all sets",
	},
	{	/* s[save], --save, -S */
		.cmd = IPSET_CMD_SAVE,
		.name = { "save", NULL },
		.has_arg = IPSET_OPTIONAL_ARG,
		.help = "[SETNAME]\n"
		        "        Save the named set or all sets to stdout",
	},
	{	/* r[estore], --restore, -R */
		.cmd = IPSET_CMD_RESTORE,
		.name = { "restore", NULL },
		.has_arg = IPSET_NO_ARG,
		.help = "\n"
		        "        Restore a saved state",
	},
	{	/* f[lush], --flush, -F */
		.cmd = IPSET_CMD_FLUSH,
		.name = { "flush", NULL },
		.has_arg = IPSET_OPTIONAL_ARG,
		.help = "[SETNAME]\n"
		        "        Flush a named set or all sets",
	},
	{	/* ren[ame], --rename, e, -E */
		.cmd = IPSET_CMD_RENAME,
		.name = { "rename", "e" },
		.has_arg = IPSET_MANDATORY_ARG2,
		.help = "FROM-SETNAME TO-SETNAME\n"
		        "        Rename two sets",
	},
	{	/* sw[ap], --swap, w, -W */
		.cmd = IPSET_CMD_SWAP,
		.name = { "swap", "w" },
		.has_arg = IPSET_MANDATORY_ARG2,
		.help = "FROM-SETNAME TO-SETNAME\n"
		        "        Swap the contect of two existing sets",
	},
	{	/* h[elp, --help, -H */
		.cmd = IPSET_CMD_HELP,
		.name = { "help", NULL },
		.has_arg = IPSET_OPTIONAL_ARG,
		.help = "[TYPENAME]\n"
		        "        Print help, and settype specific help",
	},
	{	/* v[ersion], --version, -V */
		.cmd = IPSET_CMD_VERSION,
		.name = { "version", NULL },
		.has_arg = IPSET_NO_ARG,
		.help = "\n"
		        "        Print version information",
	},
	{	/* q[uit] */
		.cmd = IPSET_CMD_QUIT,
		.name = { "quit", NULL },
		.has_arg = IPSET_NO_ARG,
		.help = "\n"
		        "        Quit interactive mode",
	},
	{ },
};

/* Match a command: try to match as a prefix or letter-command */
bool
ipset_match_cmd(const char *arg, const char * const name[])
{
	size_t len;

	assert(arg);
	assert(name && name[0]);

	/* Ignore (two) leading dashes */
	if (arg[0] == '-')
		arg++;
	if (arg[0] == '-')
		arg++;

	len = strlen(arg);

	if (len > strlen(name[0]) || !len)
		return false;
	else if (len > 1 &&
		 ((strncmp(arg, name[0], len) == 0) ||
		  (name[1] != NULL && (strncmp(arg, name[1], len) == 0))))
		return true;
	else if (len != 1)
		return false;
	else return tolower(arg[0]) == name[0][0] ||
		    (name[1] != NULL && tolower(arg[0]) == name[1][0]);	
}

const struct ipset_envopts ipset_envopts[] = {
	{ .name = { "-o", "-output" },
	  .has_arg = IPSET_MANDATORY_ARG,	.flag = IPSET_OPT_MAX,
	  .parse = ipset_parse_output,
	  .help = "plain|save|xml\n"
	  	  "       Specify output mode for listing sets.\n"
	  	  "       Default value for \"list\" command is mode \"plain\"\n"
	  	  "       and for \"save\" command is mode \"save\".",
	},
	{ .name = { "-s", "-sorted" },
	  .parse = ipset_envopt_parse,
	  .has_arg = IPSET_NO_ARG,	.flag = IPSET_ENV_SORTED,
	  .help = "\n"
	          "        Print elements sorted (if supported by the set type).",
	},
	{ .name = { "-q", "-quiet" },
	  .parse = ipset_envopt_parse,
	  .has_arg = IPSET_NO_ARG,	.flag = IPSET_ENV_QUIET,
	  .help = "\n"
	          "        Suppress any notice or warning message.",
	},
	{ .name = { "-r", "-resolve" },
	  .parse = ipset_envopt_parse,
	  .has_arg = IPSET_NO_ARG,	.flag = IPSET_ENV_RESOLVE,
	  .help = "\n"
	          "        Try to resolve IP addresses in the output (slow!)",
	},
	{ .name = { "-!", "-exist" },
	  .parse = ipset_envopt_parse,
	  .has_arg = IPSET_NO_ARG,	.flag = IPSET_ENV_EXIST,
	  .help = "\n"
	          "        Ignore errors when creating already created sets,\n"
	          "        when adding already existing elements\n"
		  "        or when deleting non-existing elements.",
	},
	{ },
};

/* Strict option matching */
bool
ipset_match_option(const char *arg, const char * const name[])
{
	assert(arg);
	assert(name && name[0]);
	
	/* Skip two leading dashes */
	if (arg[0] == '-' && arg[1] == '-')
		arg++, arg++;

	return STREQ(arg, name[0])
	       || (name[1] != NULL && STREQ(arg, name[1]));
}

/* Strict envopt matching */
bool
ipset_match_envopt(const char *arg, const char * const name[])
{
	assert(arg);
	assert(name && name[0]);
	
	/* Skip one leading dash */
	if (arg[0] == '-' && arg[1] == '-')
		arg++;

	return STREQ(arg, name[0])
	       || (name[1] != NULL && STREQ(arg, name[1]));
}

/**
 * ipset_shift_argv - shift off an argument
 * @arc: argument count
 * @argv: array of argument strings
 * @from: from where shift off an argument
 *
 * Shift off the argument at "from" from the array of
 * arguments argv of size argc.
 */
void
ipset_shift_argv(int *argc, char *argv[], int from)
{
	int i;
	
	assert(*argc >= from + 1);

	for (i = from + 1; i <= *argc; i++) {
		argv[i-1] = argv[i];
	}
	(*argc)--;
	return;
}

/**
 * ipset_port_usage - prints the usage for the port parameter
 * 
 * Print the usage for the port parameter to stdout.
 */
void
ipset_port_usage(void)
{
	int i;
	const char *name;

	printf("      [PROTO:]PORT is a valid pattern of the following:\n"
	       "           PORTNAME         port name from /etc/services\n"
	       "           PORTNUMBER       port number identifier\n"
	       "           tcp|udp:PORTNAME|PORTNUMBER\n"
	       "           icmp:CODENAME    supported ICMP codename\n"
	       "           icmp:TYPE/CODE   ICMP type/code value\n"
	       "           icmpv6:CODENAME  supported ICMPv6 codename\n"
	       "           icmpv6:TYPE/CODE ICMPv6 type/code value\n"
	       "           PROTO:0          all other protocols\n\n");

	printf("           Supported ICMP codenames:\n");
	i = 0;
	while ((name = id_to_icmp(i++)) != NULL)
		printf("               %s\n", name);
	printf("           Supported ICMPv6 codenames:\n");
	i = 0;
	while ((name = id_to_icmpv6(i++)) != NULL)
		printf("               %s\n", name);
}
