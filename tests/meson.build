tests_links = [
	libmap,
	libtile,
]

libacommontestutils_sources = [
	'test_utils.c',
	'tests.c',
]

libcommontestutils = static_library('commontestutils',
					libacommontestutils_sources,
					dependencies: gtk4,
					link_with: tests_links,
)

tests_links += libcommontestutils

psapi_cflags = [ ]
link_args = []

libapp_c_args = []

app_tests = [
	'map',
]

prio = 1000
foreach test_name : app_tests
	test_exe = executable(
		test_name,
		'test-@0@.c'.format(test_name),
		'tests.c',
		dependencies: [
			gtk4,
		],
		link_with: [
			tests_links
		]
	)

	test(
		test_name,
		test_exe,
		env: [
			'G_TEST_SRCDIR=@0@'.format(meson.current_source_dir()),
			'G_TEST_BUILDDIR=@0@'.format(meson.current_build_dir()),
			'G_SLICE=always-malloc',
			'G_DEBUG=gc-friendly',
		],
		# env: test_env,
		timeout: 60,
		is_parallel : false,
		priority: prio,
	)

	prio = prio - 10

endforeach
