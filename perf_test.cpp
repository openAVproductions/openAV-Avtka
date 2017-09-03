#include <stdlib.h>
#include "benchmark/benchmark.h"

#include <avtka/avtka.h>

static void
event_cb(struct avtka_t *avtka, uint32_t item, float v, void *userdata)
{
}

void create_destroy(benchmark::State& state)
{
	while (state.KeepRunning()) {
		struct avtka_opts_t opts = {
			.w = 360,
			.h = 240,
			.event_callback = event_cb,
		};
		struct avtka_t *a = avtka_create("Avtka Perf Test", &opts);
		avtka_iterate(a);
		avtka_destroy(a);
	}
}

void iterate_basic(benchmark::State& state)
{
	struct avtka_opts_t opts = {
		.w = 360,
		.h = 240,
		.event_callback = event_cb,
	};
	struct avtka_t *a = avtka_create("Avtka Perf Test", &opts);

	struct avtka_item_opts_t item = {
		.name = "Dial 1",
		.x = 10, .y = 10, .w = 50, .h = 50,
		.draw = AVTKA_DRAW_DIAL,
		.interact = AVTKA_INTERACT_DRAG_V,
	};
	uint32_t i1 = avtka_item_create(a, &item);
#warning > 1000 items causes segfault in puglDestroy - investigate
	for(int i = 0; i < 1000; i++) {
		item.draw = i % 3;
		avtka_item_create(a, &item);
	}

	avtka_iterate(a);

	while (state.KeepRunning()) {
		avtka_iterate(a);
	}

	avtka_destroy(a);
}

//BENCHMARK(benchy)->ReportAggregatesOnly(1)->Repetitions(1);
BENCHMARK(create_destroy);
BENCHMARK(iterate_basic);


BENCHMARK_MAIN()
