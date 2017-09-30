project('openav_avtka', ['c','cpp'])

conf_data = configuration_data()
conf_data.set('version', '0.1')

add_project_arguments('-std=c++11', language : 'cpp')

cc  = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

subdir('src')

install_headers('include/avtka.h')

avtka_includes = include_directories('include')

pkg = import('pkgconfig')
pkg.generate(name: 'openav_avtka',
              description: 'OpenAV Avtka Library',
              version: conf_data.get('version'),
              requires: ['cairo', 'x11', 'gl'],
              libraries: avtka)

# dep to make it easy to use Avtka as a Meson subproject
avtka_dep = declare_dependency(
  include_directories : 'include',
  link_with : avtka)

avtka_demo = files('demo.c')

executable('avtka_demo', avtka_demo,
            include_directories: avtka_includes,
            link_with: avtka)

bench = dependency('benchmark', required: false)
if bench.found()
  subdir('tests')
  threads = dependency('threads')
  tests = executable('avtka_tests',test_files,
                      link_with: avtka,
                      dependencies: [bench, threads])
  #test('Performance Tests', tests)

  test_create = executable('test_create', test_create, link_with: avtka)
  test('Create', test_create)
  test('Create Envvar', test_create, env : ['AVTKA_DEBUG=3'])
endif
