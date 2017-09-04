project('openav_avtka', ['c','cpp'])

conf_data = configuration_data()
conf_data.set('version', '0.1')

add_project_arguments('-std=c++11', language : 'cpp')

cc  = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

subdir('src')

install_headers(avtka_hdr, subdir: 'avtka')

pkg = import('pkgconfig')
pkg.generate(name: 'openav_avtka',
              description: 'OpenAV Avtka Library',
              version: conf_data.get('version'),
              requires: ['cairo', 'x11', 'gl'],
              libraries: avtka)

avtka_includes = include_directories('src')

avtka_demo = files('demo.c')

executable('avtka_demo', avtka_demo,
            include_directories: avtka_includes,
            link_with: avtka)


subdir('tests')

bench = dependency('benchmark')
threads = dependency('threads')

tests = executable('avtka_tests',
                    test_files,
                    link_with: avtka,
                    dependencies: [bench, threads])
test('Performance Tests', tests)
