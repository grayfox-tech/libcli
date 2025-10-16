#ifndef __BUS_CMDLN_COMMON_INCLUDED_H__
#define __BUS_CMDLN_COMMON_INCLUDED_H__

#include <stdio.h>

struct action {
	/* action name */
	char *name;

	/* user set up argument counter */
	int argc;

	/* cmd action value */
	unsigned int val;
};

struct family {
	/* family name */
	char *name;

	/* array of family cmds */
	struct action *cmds;

	/* size of the cmds array */
	size_t n;

	/* family value */
	unsigned int val;

	/* callback */
	int (*handler)(int action, const char **args, void *usr);

	/* usr data */
	void *usr;
};

struct cmd {
	/* array of families */
	struct family *families;

	/* size of the family array */
	size_t n;
};

#endif // __BUS_CMDLN_COMMON_INCLUDED_H__
