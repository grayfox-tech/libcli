#ifndef __BUS_CMD_INCLUDED_H__
#define __BUS_CMD_INCLUDED_H__

#define cmd_family_info_val(nam, act, cb, us)                        \
	&(struct cmd_family_info)                                    \
	{                                                            \
		.name = nam, .action = act, .handler = cb, .usr = us \
	}

#define cmd_action_info_val(nam, argcnt, v)           \
	&(struct cmd_action_info)                     \
	{                                             \
		.name = nam, .argc = argcnt, .val = v \
	}

typedef struct cmd cmd_t;

struct cmd_action_info {
	char *name;
	char argc;
	int val;
};

struct cmd_family_info {
	char *name;
	struct cmd_action_info **action;
	int (*handler)(int action, const char **args, void *usr);
	void *usr;
};

int cmd_alloc(const struct cmd_family_info **fam_info, cmd_t **cmd);
int count_cmd_args(char *argp);
int family_offset(const cmd_t *cmds, const char *family);
int cmd_offset(const cmd_t *cmds, int foffset, const char *cmd);
int family_val(const cmd_t *cmds, const char *family, int *offset);
int cmd_val(const cmd_t *cmds, int foffset, const char *cmd, int *offset);
int family_count(const cmd_t *cmds);
int cmd_count(const cmd_t *cmds, int foffset);
int has_args(const cmd_t *cmds, int foffset, int coffset);
const char *get_family_name(cmd_t *cmd, int foffset);
void print_families(cmd_t *cmd);
void print_cmds(cmd_t *cmd);
void get_cmds(void);
void free_cmd(cmd_t *cmd);

#endif // __BUS_CMD_INCLUDED_H__
