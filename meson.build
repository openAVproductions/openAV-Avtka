project('openav_avtka', 'c')

conf_data = configuration_data()
conf_data.set('version', '0.1')

cc  = meson.get_compiler('c')

subdir('src')

install_headers(avtka_hdr, subdir: 'src')

pkg = import('pkgconfig')
pkg.generate(name: 'openav_avtka',
              description: 'OpenAV Ctlra Library',
              version: conf_data.get('version'),
              requires: ['cairo', 'x11'],
              libraries: avtka)

#executable('simple', example_simple, include_directories: avtka_includes, link_with: )
