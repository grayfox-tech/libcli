#ifndef __MAPEDITOR_TESTS_INCLUDED_H__
#define __MAPEDITOR_TESTS_INCLUDED_H__

#define ADD_TEST(entity, function, fixture, tdata)                    \
	g_test_add("/" #entity "/" #function, TestFixture, tdata,     \
		   test_##entity##_setup, test_##entity##_##function, \
		   test_##entity##_teardown);

#endif // __MAPEDITOR_TESTS_INCLUDED_H__
