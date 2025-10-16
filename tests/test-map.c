#include "../map/tilemap.h"
#include "../map/tilemapmanager.h"
#include "tests.h"
#include "test_utils.h"
#include <glib.h>
#include <stdio.h>

typedef struct {
	struct tilemap_manager *tmm;
} TestFixture;

static void test_tilemapmanager_setup(TestFixture *fixture, gconstpointer data)
{
	fixture->tmm = alloc_tilemap_mngr();
	g_assert_nonnull(fixture->tmm);
}

static void test_tilemapmanager_teardown(TestFixture *fixture,
					 gconstpointer data)
{
	free_tilemap_mngr(fixture->tmm);
	fixture->tmm = NULL;
}

static void test_tilemapmanager_add_tilemap(TestFixture *fixture,
					    gconstpointer data)
{
	struct tilemap_manager *tmm = fixture->tmm;

	add_tilemap(&tmm, "tm1", 10, 10);

	g_assert_nonnull(tmm->tilemaps->layers);
	g_assert_cmpint(tmm->n, ==, 1);
	g_assert_cmpint(tmm->tilemaps->linear_len, ==, 100);

	struct tilemap *tm = tmm->tilemaps;

	g_assert_nonnull(tm);
	g_assert_cmpint(tm->nlayers, ==, 1);
	g_assert_cmpint(tm->linear_len, ==, 100);

	add_tilemap(&tmm, "tm2", 20, 20);

	g_assert_nonnull(tmm->tilemaps->layers + 1);
	g_assert_cmpint(tmm->n, ==, 2);
	g_assert_cmpint((tmm->tilemaps + 1)->linear_len, ==, 400);

	tm = tmm->tilemaps + 1;

	g_assert_nonnull(tm);
	g_assert_cmpint(tm->nlayers, ==, 1);
	g_assert_cmpint(tm->linear_len, ==, 400);
}

static void test_tilemapmanager_add_layer(TestFixture *fixture,
					  gconstpointer data)
{
	struct tilemap_manager *tmm = fixture->tmm;

	add_tilemap(&tmm, "tm1", 10, 10);

	g_assert_nonnull(tmm->tilemaps->layers);
	g_assert_cmpint(tmm->n, ==, 1);
	g_assert_cmpint(tmm->tilemaps->linear_len, ==, 100);

	struct tilemap *tm = tmm->tilemaps;

	g_assert_nonnull(tm);
	g_assert_cmpint(tm->nlayers, ==, 1);
	g_assert_cmpint(tm->linear_len, ==, 100);

	add_tilemap(&tmm, "tm2", 20, 20);

	g_assert_nonnull(tmm->tilemaps->layers + 1);
	g_assert_cmpint(tmm->n, ==, 2);
	g_assert_cmpint((tmm->tilemaps + 1)->linear_len, ==, 400);

	/* add layer to tilemap[0] */
	add_layer(&tmm->tilemaps, "Layer 1", tmm->tilemaps->rows,
		  tmm->tilemaps->cols);

	tm = tmm->tilemaps;

	g_assert_nonnull(tm);
	g_assert_cmpint(tm->nlayers, ==, 2);
	g_assert_cmpint(tm->linear_len, ==, 100);

	/* add layers to tilemap[1] */
	tm = tmm->tilemaps + 1;

	add_layer(&tm, "Layer 1", tm->rows, tm->cols);

	g_assert_nonnull(tm);
	g_assert_cmpint(tm->nlayers, ==, 2);
	g_assert_cmpint(tm->linear_len, ==, 400);

	add_layer(&tm, "Layer 2", tm->rows, tm->cols);

	g_assert_nonnull(tm);
	g_assert_cmpint(tm->nlayers, ==, 3);
	g_assert_cmpint(tm->linear_len, ==, 400);
}

static void test_tilemapmanager_rm_tilemap(TestFixture *fixture,
					   gconstpointer data)
{
	struct tilemap_manager *tmm = fixture->tmm;

	add_tilemap(&tmm, "tm1", 10, 10);

	/* remove the only existing tilemap */
	rm_tilemap(&tmm, 0);

	g_assert_cmpint(tmm->n, ==, 0);
	g_assert_null(tmm->tilemaps);

	add_tilemap(&tmm, "tm1", 10, 10);
	add_tilemap(&tmm, "tm2", 20, 20);
	add_tilemap(&tmm, "tm3", 30, 30);
	add_tilemap(&tmm, "tm4", 40, 40);
	add_tilemap(&tmm, "tm5", 50, 50);
	add_tilemap(&tmm, "tm6", 60, 60);

	/* remove middle tilemap */
	rm_tilemap(&tmm, 2);

	g_assert_cmpint(tmm->n, ==, 5);
	g_assert_nonnull(tmm->tilemaps);

	/* remove middle tilemap */
	rm_tilemap(&tmm, 1);

	g_assert_cmpint(tmm->n, ==, 4);
	g_assert_nonnull(tmm->tilemaps);

	/* remove first element */
	rm_tilemap(&tmm, 0);

	g_assert_cmpint(tmm->n, ==, 3);
	g_assert_nonnull(tmm->tilemaps);

	/* remove last element */
	rm_tilemap(&tmm, tmm->n - 1);

	g_assert_cmpint(tmm->n, ==, 2);
	g_assert_nonnull(tmm->tilemaps);
}

static void test_tilemapmanager_rm_layer(TestFixture *fixture,
					 gconstpointer data)
{
	struct tilemap_manager *tmm = fixture->tmm;

	add_tilemap(&tmm, "tm1", 10, 10);

	add_tilemap(&tmm, "tm2", 20, 20);

	add_tilemap(&tmm, "tm3", 20, 20);

	/* point to first tilemap */
	struct tilemap *tm = tmm->tilemaps;

	/* add layer to tilemap[0] */
	add_layer(&tm, "Layer 1", tm->rows, tm->cols);
	add_layer(&tm, "Layer 2", tm->rows, tm->cols);
	add_layer(&tm, "Layer 3", tm->rows, tm->cols);
	add_layer(&tm, "Layer 4", tm->rows, tm->cols);

	/* point to second tilemap */
	tm = tmm->tilemaps + 1;

	/* add layers to tilemap[1] */
	add_layer(&tm, "Layer 1", tm->rows, tm->cols);
	add_layer(&tm, "Layer 2", tm->rows, tm->cols);
	add_layer(&tm, "Layer 3", tm->rows, tm->cols);
	add_layer(&tm, "Layer 4", tm->rows, tm->cols);
	add_layer(&tm, "Layer 5", tm->rows, tm->cols);
	add_layer(&tm, "Layer 6", tm->rows, tm->cols);

	/* point to last tilemap */
	tm = tmm->tilemaps + tmm->n - 1;

	/* remove layer 0 */
	rm_layer(&tm, 0);

	g_assert_cmpint(tm->nlayers, ==, 0);
	g_assert_null(tm->layers);

	/* point to tilemap 1 */
	tm = tmm->tilemaps + 1;

	/* remove last layer */
	rm_layer(&tm, tm->nlayers - 1);

	g_assert_cmpint(tm->nlayers, ==, 6);
	g_assert_nonnull(tm->layers);

	/* remove last layer */
	rm_layer(&tm, tm->nlayers - 1);

	g_assert_cmpint(tm->nlayers, ==, 5);
	g_assert_nonnull(tm->layers);
}

int main(int argc, char **argv)
{
	int result;

	g_test_init(&argc, &argv, NULL);

	// set_mapeditor_directory("G_TEST_SRCDIR", "src/test/medir");

	ADD_TEST(tilemapmanager, add_tilemap, TestFixture, NULL);
	ADD_TEST(tilemapmanager, add_layer, TestFixture, NULL);
	ADD_TEST(tilemapmanager, rm_tilemap, TestFixture, NULL);
	ADD_TEST(tilemapmanager, rm_layer, TestFixture, NULL);

	/* Run the tests */
	result = g_test_run();

	/* Don't write files to the source dir */
	// set_mapeditor_directory("G_TEST_BUILDDIR", "src/test/medir-output");

	return result;
}
