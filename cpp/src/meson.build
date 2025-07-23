simplify_sources = ['simplify_algorithm.cpp']

lib = shared_library(
    'simplify',
    simplify_sources,
    include_directories: includes,
    cpp_args: lib_args,
    gnu_symbol_visibility: 'hidden',
    install: true,
)