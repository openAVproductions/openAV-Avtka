avtka_hdr = files('avtka.h')
avtka_src = files('avtka.c', 'item.c', 'draw.c', 'interact.c')
avtka_src += files('pugl/pugl_x11.c')

cairo = dependency('cairo')
x11   = dependency('x11')

avtka = static_library('avtka',
    [avtka_src],
    c_args: ['-DPUGL_HAVE_CAIRO', '-Isrc/pugl/'],
    install : true,
    dependencies: [cairo, x11])
