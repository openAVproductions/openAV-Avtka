all:
	gcc -DPUGL_HAVE_CAIRO `pkg-config --cflags cairo` -Isrc/ -Isrc/pugl/ src/*.c src/pugl/pugl_x11.c main.c -lcairo -lX11
