#ifndef __CLI_INFO_INCLUDED_H__
#define __CLI_INFO_INCLUDED_H__

#include <stdio.h>

struct cli_info {
	/* client application name */
	char *name;

	/* client application version */
	char *version;

	/* prompt style */
	char *prompt;

	/* is interactive console or daemon */
	int daemon;

	/* cmd line buffer */
	char *buf;

	/* buffer capacity */
	size_t n;
};

#endif // __CLI_INFO_INCLUDED_H__
