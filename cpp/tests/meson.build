test_sources = ['simplify_algorithm_test.cpp', '../src/algorithms.cpp']
test_includes = include_directories(
    '.',
    '../src',
)

test_lib_dep = declare_dependency(link_with: lib, include_directories: includes)
test_catch_dep = dependency(
    'catch2-with-main',
    fallback: ['catch2', 'catch2_with_main_dep'],
    required: true,
)
test_json_dep = dependency(
    'nlohmann_json',
    fallback: ['nlohmann_json', 'nlohmann_json_dep'],
    required: true,
)

fs = import('fs')

test_exe = executable(
    'simplify_algorithm_test',
    test_sources,
    include_directories: test_includes,
    dependencies: [test_lib_dep, test_catch_dep, test_json_dep],
)

test(
    'simplify_algorithm_test',
    test_exe,
    args: ['-s', '--reporter JUnit::out=test-results.xml'],
    depends: [
        fs.copyfile('../../test-data/algorithm-simplifications.json'),
        fs.copyfile('../../test-data/douglas-peucker-simplifications.json'),
        fs.copyfile('../../test-data/point-distances.json'),
        fs.copyfile('../../test-data/points.json'),
        fs.copyfile('../../test-data/radial-simplifications.json'),
        fs.copyfile('../../test-data/segment-distances.json'),
    ],
    workdir: meson.current_build_dir(),
)