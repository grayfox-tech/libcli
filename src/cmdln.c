#include "cmdln.h"
#include "info.h"
#include "common.h"
#include "cmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#define BUF_MAX 512

extern struct cmd *cmd;

struct cmd_line {
	/* family name */
	char *family;

	/* action name */
	char *action;

	/* parsed arguments */
	char **args;

	/* arguments passed to the cmd action */
	int argc;

	/* family val */
	int fam_val;

	/* action val */
	int act_val;

	/* family and action val */
	int raw_val;
};

static char *strtrim(char *dest, size_t n, const char *src)
{
	char *_dest = dest;
	const char *ptr = src, *tmp = NULL;

	if (src[0] == ' ')
		while (*++ptr == ' ')
			;

	while (*ptr != '\0') {
		*_dest++ = *ptr++;
		if (*ptr == ' ') {
			tmp = ptr;
			while (*++tmp == ' ')
				;
			if (*tmp == '\0')
				break;
		}
	}
	*_dest = '\0';

	return dest;
}

static char *strquot(char *dest, size_t n, char **src)
{
	char *_dest = dest;
	char *_src = *src;

	while (1)
		if (*_src++ == '\"') {
			for (int i = 0; (*_src != '\"') && (i < n);
			     ++_src, ++i) {
				*_dest++ = *_src;
				if ((*_src == '\\') && (*(_src + 1) == '\"'))
					*_dest++ = *_src++;
			}
			break;
		}

	return dest;
}

static char **cmdln_alloc_args(const char *argp, int argc)
{
	char **args = NULL;
	char *ptr = (char *)argp;
	char *argp_tmp = (char *)argp;
	char buf[BUF_MAX];
	char *token = NULL;
	int i = 0;

	args = malloc((argc + 1) * sizeof(char *));
	if (!args)
		return NULL;

	while (*ptr != '\0') {
		if (*ptr == '"')
			token = strquot(buf, BUF_MAX, &ptr);
		else {
			// TODO review those lines
			token = strtok_r(argp_tmp, " ", &ptr);
			argp_tmp = NULL;
		}
		args[i++] = strdup(token);
	}

	args[i] = NULL;

	return args;
}

static void cmdln_free_args(char **argp)
{
	if (argp) {
		int i = 0;
		while (argp[i])
			free(argp[i++]);
		free(argp);
	}
}

static void cmdln_init(struct cmd_line **cmdln)
{
	(*cmdln)->family = NULL;
	(*cmdln)->action = NULL;
	(*cmdln)->args = NULL;
	(*cmdln)->argc = 0;
	(*cmdln)->fam_val = -1;
	(*cmdln)->act_val = -1;
	(*cmdln)->raw_val = -1;
}

static void cmdln_free(struct cmd_line *cmdln)
{
	if (cmdln) {
		free(cmdln->family);
		free(cmdln->action);
		cmdln_free_args(cmdln->args);
	}
}

static int cmdln_parse(const cmd_t *cmds, const char *cmdline,
		       struct cmd_line *cmdln)
{
	char *ptr = (char *)cmdline;
	char *token;
	int fam_off;
	int cmd_off;

	cmdln_init(&cmdln);

	token = strtok_r((char *)cmdline, " ", &ptr);
	cmdln->fam_val = family_val(cmds, token, &fam_off);

	if (cmdln->fam_val < 0)
		goto error;

	cmdln->family = strdup(token);

	int hascmds = has_cmds(cmds, fam_off);
	if (!ptr && hascmds)
		goto error;

	if (!hascmds)
		goto done;

	token = strtok_r(NULL, " ", &ptr);

	cmdln->act_val = cmd_val(cmds, fam_off, token, &cmd_off);

	if (cmdln->act_val < 0)
		goto error;

	cmdln->action = strdup(token);

	int argn = has_args(cmds, fam_off, cmd_off);
	if (!ptr && argn)
		goto error;

	if (!argn)
		goto done;

	int argc = count_cmd_args(ptr);

	if (argc != argn)
		goto error;

	if (argc) {
		cmdln->argc = argc;
		cmdln->args = cmdln_alloc_args(ptr, argc);
		if (!cmdln->args)
			goto error;
	} else
		cmdln->args = NULL;

done:
	return 0;

error:
	return -1;
}

static int cmdln_get_str(char *dest, size_t n)
{
	char *_dest = dest;
	char c;

	while ((c = getchar()) != '\n')
		*_dest++ = c;
	*_dest = '\0';

	strtrim(dest, n, dest);

	return strlen(dest);
}

int cmdln_handle_acts(const cmd_t *cmds, const struct cli_info *cinfo, int *ret)
{
	struct cmd_line cmd_line = { 0 };
	int cmdln_len = 0;

	printf("%s%s", cinfo->name ? cinfo->name : "",
	       cinfo->prompt ? cinfo->prompt : " > ");

	if ((cmdln_len = cmdln_get_str(cinfo->buf, cinfo->n)) < 0)
		return -1;

	if (cmdln_len == 0)
		return 0;

	int _ret = cmdln_parse(cmds, cinfo->buf, &cmd_line);

#ifdef __DEBUG
	printf("RET: %d\n", ret);
#endif
	if (_ret >= 0) {
#ifdef __DEBUG
		printf("Family: %s\n", cmd_line.family);
		printf("Action: %s\n", cmd_line.action);
		puts("Args: ");
		if (cmd_line.args)
			for (int i = 0; cmd_line.args[i]; ++i)
				printf("\t%s\n", (cmd_line.args[i]));
		printf("Argc: %d\n", cmd_line.argc);
		printf("Family value: %d\n", cmd_line.fam_val);
		printf("Action value: %d\n", cmd_line.act_val);
#endif
		int i, act_ret;
		for (i = 0; i < cmds->families->n; ++i) {
			if (strcmp(cmd_line.family,
				   (cmds->families + i)->name) == 0) {
				act_ret =
					(cmds->families + i)
						->handler(cmd_line.act_val,
							  (const char **)
								  cmd_line.args,
							  (cmds->families + i)
								  ->usr);
			}
		}

		if (ret)
			*ret = act_ret;
	}

	cmdln_free(&cmd_line);

	return _ret;
}

int cmdln_handle_opts(int argc, char *const *argv, const char *shortopts,
		      const struct cmdln_option *options,
		      void (*handle_opts)(const struct cmdln_option *,
					  const struct parsed_opts *, void *),
		      void *usr)
{
	while (1) {
		int option_index = 0;
		int c = getopt_long(argc, argv, shortopts,
				    (const struct option *)options,
				    &option_index);

		if (c == -1)
			break;

		const struct parsed_opts popts = { .c = c,
						   .optarg = optarg,
						   .optind = optind,
						   .sel_optind = option_index };

		handle_opts(options, &popts, usr);
	}

	return 0;
}
