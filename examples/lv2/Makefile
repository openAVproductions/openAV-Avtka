all:
	mkdir -p avtka_example.lv2
	gcc -g -Wl,--no-undefined -shared -fPIC  avtka_ui.c -o avtka_example.lv2/avtka_example_ui.so -lavtka -lX11 -lcairo -lGL
	gcc -g -Wl,--no-undefined -shared -fPIC  amp.c -o avtka_example.lv2/avtka_example.so -lm
