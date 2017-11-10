project('openav_avtka', ['c','cpp'])

conf_data = configuration_data()
conf_data.set('version', '0.1')

add_project_arguments('-std=c99', language : 'c')
add_project_arguments('-std=c++11', language : 'cpp')

cc  = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

dep_names = ['cairo', 'x11', 'GL']
deps = []

foreach dep : dep_names
  deps += dependency(dep)
endforeach

avtka_includes = []
avtka_includes += include_directories('include')
avtka_includes += include_directories('src')
avtka_src = []

src_files  = ['avtka.c', 'screen.c', 'item.c', 'draw.c', 'interact.c']
src_files += ['pugl/pugl_x11.c']

foreach file : src_files
  avtka_src += join_paths('src', file)
endforeach

install = true
if meson.is_subproject()
  install = false
endif

avtka = static_library('avtka',
    [avtka_src],
    c_args: ['-DPUGL_HAVE_CAIRO',
             '-DPUGL_HAVE_GL',
             '-Isrc/pugl/'],
    install : install,
    include_directories: avtka_includes,
    dependencies: deps)

if meson.is_subproject()
  # dep to make it easy to use Avtka as a Meson subproject
  avtka_dep = declare_dependency(
      include_directories : avtka_includes,
      link_with : avtka)
else
  # standalone install
  install_headers('include/avtka.h')

  pkg = import('pkgconfig')
  pkg.generate(name: 'openav_avtka',
                description: 'OpenAV Avtka Library',
                version: conf_data.get('version'),
                requires: dep_names,
                libraries: avtka)

  # compile the demo project
  avtka_demo = files('demo.c')
  executable('avtka_demo', avtka_demo,
              include_directories: avtka_includes,
              link_with: avtka)

  # grab test files
  subdir('tests')

  # compile benchmarks
  bench = dependency('benchmark', required: false)
  if bench.found()
    threads = dependency('threads')
    tests = executable('avtka_tests',test_files,
                        link_with: avtka,
                        include_directories: avtka_includes,
                        dependencies: [bench, threads])
    #test('Performance Tests', tests)
  endif

  # compile tests
  test_create = executable('test_create',
                            test_create,
                            include_directories: avtka_includes,
                            link_with: avtka)
  test('Create', test_create)
  test('Create Envvar', test_create, env : ['AVTKA_DEBUG=3'])
endif
