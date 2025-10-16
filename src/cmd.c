#include "cmd.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int has_cmds(const cmd_t *cmds, int foffset);

static void free_cmds(struct action *cmds, size_t size)
{
	int i;
	for (i = size - 1; i >= 0; --i)
		free((cmds + i)->name);
	free(cmds);
}

static void free_families(struct family *families, size_t size)
{
	int i;
	for (i = size - 1; i >= 0; --i) {
		free((families + i)->name);
		free_cmds((families + i)->cmds, (families + i)->n);
	}
	free(families);
}

static int add_cmdsv2(const struct cmd_action_info **cmds,
		      struct family **family)
{
	struct action *nmem = NULL;
	struct action *tmp = NULL;
	size_t ccount = 0;

	while (cmds[ccount])
		++ccount;

	nmem = malloc(ccount * sizeof(struct action));

	if (!nmem)
		return 1;

	int i;
	for (i = 0; i < ccount; ++i) {
		tmp = nmem + i;

		tmp->name = strdup(cmds[i]->name);
		if (!tmp->name) {
			while (i >= 0) {
				free((nmem + i)->name);
				--i;
			}
			free(nmem);
			return 1;
		}
		tmp->val = cmds[i]->val;
		tmp->argc = cmds[i]->argc;
	}

	(*family)->cmds = nmem;
	(*family)->n += ccount;

	return 0;
}

static int add_familiesv2(const struct cmd_family_info **families, cmd_t **cmd)
{
	struct family *nmem = NULL;
	struct family *tmp = NULL;
	size_t fcount = 0;

	while (families[fcount])
		++fcount;

	if (!(nmem = malloc(fcount * sizeof(struct family))))
		return -1;

	int i;
	for (i = 0; i < fcount; ++i) {
		tmp = nmem + i;

		tmp->name = strdup(families[i]->name);
		if (!tmp->name) {
			while (i >= 0) {
				free((nmem + i)->name);
				--i;
			}
			free(nmem);
			return -1;
		}
		tmp->cmds = NULL;
		tmp->n = 0;
		tmp->handler = families[i]->handler;
		tmp->usr = families[i]->usr;

		if (families[i]->action)
			add_cmdsv2((const struct cmd_action_info **)families[i]
					   ->action,
				   &tmp);
	}

	(*cmd)->families = nmem;
	(*cmd)->n += fcount;

	return 0;
}

int cmd_alloc(const struct cmd_family_info **fam_info, cmd_t **cmd)
{
	struct cmd *_cmd = malloc(sizeof(struct cmd));

	if (!_cmd)
		return -1;

	_cmd->families = NULL;
	_cmd->n = 0;

	if (add_familiesv2(fam_info, &_cmd) < 0)
		return -1;

	*cmd = _cmd;

	return 0;
}

int count_cmd_args(char *argp)
{
	if (!argp)
		return 0;

	int count = 0;
	char *_argp = argp;

	while (*_argp != '\0') {
		while (*_argp == ' ')
			++_argp;
		if (*_argp == '\"') {
			for (_argp++; *_argp != '\"' && *_argp != '\0'; ++_argp)
				if (*_argp == '\\')
					++_argp;
		} else {
			while (*_argp != ' ' && *_argp != '\0')
				++_argp;
			++count;
		}
	}

	return count;
}

int family_offset(const cmd_t *cmds, const char *family)
{
	int i;
	for (i = 0; i < cmds->n; ++i)
		if (strcmp((cmds->families + i)->name, family) == 0)
			return i;
	return -1;
}

int cmd_offset(const cmd_t *cmds, int foffset, const char *cmd)
{
	struct family *f = cmds->families + foffset;

	int i;
	for (i = 0; i < cmds->n; ++i)
		if (strcmp((f->cmds + i)->name, cmd) == 0)
			return i;
	return -1;
}

int family_val(const cmd_t *cmds, const char *family, int *offset)
{
	int i;
	for (i = 0; i < cmds->n; ++i)
		if (strcmp((cmds->families + i)->name, family) == 0) {
			if (offset)
				*offset = i;
			return (cmds->families + i)->val;
		}

	return -1;
}

int cmd_val(const cmd_t *cmds, int foffset, const char *cmd, int *offset)
{
	struct family *f = cmds->families + foffset;

	int i;
	for (i = 0; i < f->n; ++i)
		if (strcmp((f->cmds + i)->name, cmd) == 0) {
			if (offset)
				*offset = i;
			return (f->cmds + i)->val;
		}

	return -1;
}

int cmd_count(const cmd_t *cmds, int foffset)
{
	return (cmds->families + foffset)->n;
}

int family_count(const cmd_t *cmds)
{
	return cmds->n;
}

int has_cmds(const cmd_t *cmds, int foffset)
{
	return (cmds->families + foffset)->cmds ? 1 : 0;
}

int has_args(const cmd_t *cmds, int foffset, int coffset)
{
	return ((cmds->families + foffset)->cmds + coffset)->argc;
}

const char *get_family_name(cmd_t *cmd, int foffset)
{
	return (cmd->families + foffset)->name;
}

void print_families(cmd_t *cmd)
{
	struct family *f = cmd->families;

	int i;
	for (i = 0; i < cmd->n; ++i)
		puts((f + i)->name);
}

void print_cmds(cmd_t *cmd)
{
	if (!cmd)
		return;

	struct family *f = cmd->families;

	int i, j;
	for (i = 0; i < cmd->n; ++i) {
		puts((f + i)->name);
		if ((f + i)->cmds) {
			for (j = 0; j < (f + i)->n; ++j)
				printf("  %s\n", ((f + i)->cmds + j)->name);
		}
	}
}

void free_cmd(struct cmd *cmd)
{
	if (cmd)
		free_families(cmd->families, cmd->n);
	free(cmd);
}
