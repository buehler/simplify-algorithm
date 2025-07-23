project(
  'simplify_algorithm',
  'cpp',
  version: '0.0.0-development',
  license: 'AGPL-3.0-or-later',
  meson_version: '>= 1.3.0',
  default_options: ['warning_level=3', 'cpp_std=c++23'],
)

dependencies = []

# These arguments are only used to build the shared library
# not the executables that use the library.
lib_args = ['-DBUILDING_SIMPLIFY_ALGORITHM']

includes = include_directories('include')

subdir('include')
subdir('src')
subdir('tests')

# # Make this library usable as a Meson subproject.
simplify_algorithm_dep = declare_dependency(
  include_directories: include_directories('.'),
  dependencies: dependencies,
  link_with: lib,
)
meson.override_dependency('simplify_algorithm', simplify_algorithm_dep)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  lib,
  version: '0.0.0-development',
  name: 'libsimplify',
  description: 'Implementation of the simplify algorithm.',
)