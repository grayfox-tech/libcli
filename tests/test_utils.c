#include "test_utils.h"
#include <glib.h>

/*
 * borrowed from gimp 3
 *
 */

void set_env_to_subdir(const gchar *root_env_var, const gchar *subdir,
		       const gchar *target_env_var)
{
	const gchar *root_dir = NULL;
	gchar *target_dir = NULL;

	/* Get root dir */
	root_dir = g_getenv(root_env_var);
	if (!root_dir)
		g_printerr(
			"*\n"
			"*  The env var %s is not set, you are probably running\n"
			"*  in a debugger. Set it manually, e.g.:\n"
			"*\n"
			"*  set env %s=%s/src/app\n"
			"*\n",
			root_env_var, root_env_var, g_get_home_dir());

	/* Construct path and setup target env var */
	target_dir = g_build_filename(root_dir, subdir, NULL);
	g_setenv(target_env_var, target_dir, TRUE);
	g_free(target_dir);
}

/**
 * set_mapeditor_directory:
 * @root_env_var: Testing directory
 * @subdir:       Subdir, may be %NULL
 *
 * Sets MAPEDITOR_DIRECTORY to the source dir @root_env_var/@subdir. The
 * environment variables is set up by the test runner.
 **/

void set_mapeditor_directory(const char *root_env_var, const char *subdir)
{
	set_env_to_subdir(root_env_var, subdir,
			  "MAPEDITOR_DIRECTORY" /*target_env_var*/);
}
