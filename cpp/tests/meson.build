test_sources = ['simplify_algorithm_test.cpp']

test_lib_dep = declare_dependency(link_with: lib, include_directories: includes)
test_catch_dep = dependency(
    'catch2-with-main',
    fallback: ['catch2', 'catch2_with_main_dep'],
    required: true,
)

test_exe = executable(
    'simplify_algorithm_test',
    test_sources,
    dependencies: [test_lib_dep, test_catch_dep],
)
test('simplify_algorithm_test', test_exe)