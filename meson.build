project('openav_avtka', 'c')

conf_data = configuration_data()
conf_data.set('version', '0.1')

cc  = meson.get_compiler('c')

subdir('src')

install_headers(avtka_hdr, subdir: 'src')

pkg = import('pkgconfig')
pkg.generate(name: 'openav_avtka',
              description: 'OpenAV Avtka Library',
              version: conf_data.get('version'),
              requires: ['cairo', 'x11'],
              libraries: avtka)

avtka_includes = include_directories('src')

avtka_demo = files('main.c')

executable('avtka_demo', avtka_demo,
            include_directories: avtka_includes,
            link_with: avtka)
