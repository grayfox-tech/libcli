#include "tests.h"
#include "test_utils.h"
#include <glib.h>
#include <stdio.h>

typedef struct {
    struct cmd_ln *cmdln;
} TestFixture;

static void test_cmdln_setup(TestFixture *fixture, gconstpointer data)
{
    fixture->tmm = alloc_tilemap_mngr();
    g_assert_nonnull(fixture->tmm);
}

static void test_cmdln_teardown(TestFixture *fixture,
                                gconstpointer data)
{
    free_tilemap_mngr(fixture->tmm);
    fixture->tmm = NULL;
}

static void test_cmdln(TestFixture *fixture,
                        gconstpointer data)
{

}

int main(int argc, char **argv)
{
    int result;

    g_test_init(&argc, &argv, NULL);

    // set_mapeditor_directory("G_TEST_SRCDIR", "src/test/medir");

    ADD_TEST(cmdln, add_tilemap, TestFixture, NULL);


    /* Run the tests */
    result = g_test_run();

    /* Don't write files to the source dir */
    // set_mapeditor_directory("G_TEST_BUILDDIR", "src/test/medir-output");

    return result;
}
