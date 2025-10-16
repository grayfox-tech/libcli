#include "../src/cmdln.h"
#include "../src/info.h"
#include "../src/cmd.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define APP_NAME "test"
#define APP_VERSION "1.0.0"

static int interrupt = 0;

static void int_handler(int sig)
{
	interrupt = 1;
}

static void handle_cmdln_opt(const struct cmdln_option *opts,
			     const struct parsed_opts *popts, void *usr)
{
	switch (popts->c) {
	case 0:
		printf("option %s", opts[popts->sel_optind].name);
		if (popts->optarg)
			printf(" with arg %s", popts->optarg);
		printf("\n");
		break;
	case 'd':
		printf("option %s", opts[popts->sel_optind].name);
		printf("option d\n");
		break;
	case 'h':
		printf("option h\n");
		break;
	case 'v':
		printf("option v with value '%s'\n", popts->optarg);
		break;
	case '?':
		break;

	default:
		printf("?? getopt returned character code 0%o ??\n", popts->c);
	}
}

static int action_help(int action, const char **args, void *usr)
{
	printf("help\n");
	return 0;
}

static int action_version(int action, const char **args, void *usr)
{
	puts(APP_VERSION);
	return 0;
}

static int action_quit(int action, const char **args, void *usr)
{
	int *exit = (int *)usr;
	*exit = 1;

	return 0;
}

static int action_test_1(int action, const char **args, void *usr)
{
	return 0;
}

static int action_test_2(int action, const char **args, void *usr)
{
	return 0;
}

static void app_info(const struct cli_info *ci)
{
	printf("\n\n"
	       "\t=[%s v%s]"
	       "\n\n",
	       ci->name, ci->version);
}

static int interactive(const cmd_t *cmd, const struct cli_info *ci, int *exit)
{
	int ret = -1;
	while (!*exit && !interrupt) {
		int _ret = cmdln_handle_acts(cmd, ci, &ret);
		printf("HANDLE RET: %d\n", _ret);
	}

	return 0;
}

int main(int argc, char **argv)
{
	char buf[512] = { 0 };
	int exit = 0;
	static struct cmdln_option opts[] = { { "background", NO_ARG, 0, 'd' },
					      { "help", NO_ARG, 0, 'h' },
					      { "version", NO_ARG, 0, 'v' },
					      { 0, 0, 0, 0 } };

	cmdln_handle_opts(argc, argv, "dhv", opts, handle_cmdln_opt, NULL);

	struct cli_info client_info = { .name = APP_NAME,
					.version = APP_VERSION,
					.prompt = " > ",
					.daemon = 0,
					.buf = buf,
					.n = 512 };

	app_info((const struct cli_info *)&client_info);

	signal(SIGINT, int_handler);

	struct cmd_action_info *test_cmds_info_1[] = {
		cmd_action_info_val("action1", 0, 0),
		cmd_action_info_val("action2", 2, 1),
		cmd_action_info_val("action3", 1, 2),
		cmd_action_info_val("action4", 0, 3), NULL
	};

	struct cmd_action_info *test_cmds_info_2[] = {
		cmd_action_info_val("action1", 3, 0),
		cmd_action_info_val("action2", 1, 1),
		cmd_action_info_val("action3", 0, 2),
		cmd_action_info_val("action4", 0, 3), NULL
	};

	const struct cmd_family_info *families_info[] = {
		cmd_family_info_val("test1", test_cmds_info_1, action_test_1,
				    NULL),
		cmd_family_info_val("test1", test_cmds_info_2, action_test_2,
				    NULL),
		cmd_family_info_val("help", NULL, action_help, NULL),
		cmd_family_info_val("version", NULL, action_version, NULL),
		cmd_family_info_val("quit", NULL, action_quit, &exit),
		NULL
	};

	cmd_t *cmd = NULL;

	if (cmd_alloc(families_info, &cmd) < 0)
		return -1;

	interactive(cmd, &client_info, &exit);

	free_cmd(cmd);

	return 0;
}
