#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "benchmark/benchmark.h"

#include "avtka.h"

static void
event_cb(struct avtka_t *avtka, uint32_t item, float v, void *userdata)
{
}

void create_destroy(benchmark::State& state)
{
	while (state.KeepRunning()) {
		struct avtka_opts_t opts = {0};
		opts.w = 360;
		opts.h = 240;
		opts.event_callback = event_cb;
		struct avtka_t *a = avtka_create("Avtka Perf Test", &opts);
		avtka_iterate(a);
		avtka_destroy(a);
	}
}

void iterate_basic(benchmark::State& state)
{
	struct avtka_opts_t opts = {0};
	opts.w = 360;
	opts.h = 240;
	opts.event_callback = event_cb;
	struct avtka_t *a = avtka_create("Avtka Perf Test", &opts);

	struct avtka_item_opts_t item = {
		.x = 10, .y = 10, .w = 50, .h = 50,
	};
	snprintf(item.name, sizeof(item.name), "item");

	for(int i = 0; i < state.range(0); i++) {
		item.draw = state.range(1);
		item.x = rand() % 360;
		item.y = rand() % 240;
		avtka_item_create(a, &item);
	}

	avtka_iterate(a);

	while (state.KeepRunning()) {
		avtka_iterate(a);
	}

	avtka_destroy(a);
}

#if 0
BENCHMARK(create_destroy)
	->Unit(benchmark::kMicrosecond);
#endif

BENCHMARK(iterate_basic)
	->RangeMultiplier(8)
	->Ranges({{1, 512}, {0, 2}});


BENCHMARK_MAIN()
