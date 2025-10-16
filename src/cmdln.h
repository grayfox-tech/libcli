#ifndef __BUS_CMDLN_INCLUDED_H__
#define __BUS_CMDLN_INCLUDED_H__

#include "cmd.h"
#include "info.h"

#define NO_ARG 0
#define REQUIRED_ARG 1
#define OPTIONAL_ARG 2

struct cmdln_option {
	const char *name;
	int has_arg;
	int *flag;
	int val;
};

struct parsed_opts {
	int c;
	int optind;
	char *optarg;
	int sel_optind;
};

int cmdln_handle_opts(int argc, char *const *argv, const char *shortopts,
		      const struct cmdln_option *options,
		      void (*handle_opts)(const struct cmdln_option *opts,
					  const struct parsed_opts *popts,
					  void *usr),
		      void *usr);

int cmdln_handle_acts(const cmd_t *cmds, const struct cli_info *cinfo,
		      int *ret);

#endif // __BUS_CMDLN_INCLUDED_H__
