#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <avtka.h>

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#define ROOMY_DSP_URI "http://www.openavproductions.com/artyfx#roomy"
#define ROOMY_UI_URI  "http://www.openavproductions.com/artyfx#roomy/gui_avtka"

/* The "instance" of the UI. A pointer is passed around as the "LV2UI_Widget"
 * to let you get back to the structure itself */
struct demo_t {
	uint32_t dial_item;
	struct avtka_t *avtka;
};


void event_cb(struct avtka_t *avtka, uint32_t item, float v, void *userdata)
{
	struct demo_t *demo = userdata;
	printf("event on item %d, value %f\n", item, v);
}

static LV2UI_Handle
artyfx_instantiate(const struct _LV2UI_Descriptor * descriptor,
		       const char * plugin_uri,
		       const char * bundle_path,
		       LV2UI_Write_Function write_function,
		       LV2UI_Controller controller,
		       LV2UI_Widget * widget,
		       const LV2_Feature * const * features)
{
	LV2UI_Resize* resize = NULL;
	avtka_native_t parent_handle = 0;
	for (int i = 0; features[i]; ++i) {
		printf("Feature %s\n", features[i]->URI );
		if (!strcmp(features[i]->URI, LV2_UI__parent)) {
			parent_handle = (avtka_native_t)features[i]->data;
			printf("\tParent X11 ID %ld\n", parent_handle );
		} else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
			resize = (LV2UI_Resize*)features[i]->data;
			printf("\tResize %p\n", resize);
		}
	}

	if (strcmp(plugin_uri, ROOMY_DSP_URI) != 0)
		return 0;

	printf("matched %s\n", plugin_uri);
	struct demo_t *demo = calloc(1, sizeof(struct demo_t));
	if(!demo)
		return 0;

	struct avtka_opts_t opts = {
		.native_parent = parent_handle,
		.w = 360,
		.h = 240,
		.event_callback = event_cb,
		.event_callback_userdata = &demo,
	};
	demo->avtka = avtka_create("AVTKA v0.1", &opts);
	if(!demo->avtka) {
		printf("failed to create avtka instance, quitting\n");
		return 0;
	}

	struct avtka_item_opts_t item = {
		.name = "Dial 1",
		.x = 10, .y = 10, .w = 50, .h = 50,
		.draw = AVTKA_DRAW_DIAL,
		.interact = AVTKA_INTERACT_DRAG_V,
	};
	uint32_t dial_item_id = avtka_item_create(demo->avtka, &item);

	/* TODO: add write/controller to UI to update state later..
	ui->write_function = write_function;
	ui->controller     = controller;
	*/

	*widget = (void*)avtka_get_native_handle(demo->avtka);

	if (resize) {
		resize->ui_resize(resize->handle, opts.w, opts.h);
	}

	avtka_redraw(demo->avtka);

	return demo;
}

static void artyfx_cleanup(LV2UI_Handle ui)
{
	/* TODO */
}

static void artyfx_port_event(LV2UI_Handle handle,
                              uint32_t port_index,
                              uint32_t buffer_size,
                              uint32_t format,
                              const void * buffer)
{
	struct demo_t *demo = handle;
	avtka_redraw(demo->avtka);
#if 0
	/* TODO */
	ui->lv2PortEvent( port_index, buffer_size, format, buffer);
	ui->redraw();
#endif
}

static int artyfx_idle(LV2UI_Handle handle)
{
	struct demo_t *demo = handle;
	avtka_iterate(demo->avtka);
	return 0;
}

static const LV2UI_Idle_Interface idle_iface = { artyfx_idle };

static const void*
artyfx_extension_data(const char* uri)
{
	if (!strcmp(uri, LV2_UI__idleInterface)) {
		return &idle_iface;
	}
	return NULL;
}

static const LV2UI_Descriptor descriptor[] = {
	{
		ROOMY_UI_URI,
		artyfx_instantiate,
		artyfx_cleanup,
		artyfx_port_event,
		artyfx_extension_data
	}
};

LV2_SYMBOL_EXPORT const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
	if( index == 0 ) {
		return &descriptor[index];
	}
	return NULL;
}

