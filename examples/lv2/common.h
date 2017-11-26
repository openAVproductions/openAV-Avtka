#pragma once

/* contents of this file are shared between the DSP and the UI */

#define AVTKA_DSP_URI "http://www.openavproductions.com/avtka#example"
#define AVTKA_UI_URI  "http://www.openavproductions.com/avtka#example/gui"

typedef enum {
	AMP_GAIN   = 0,
	AMP_INPUT  = 1,
	AMP_OUTPUT = 2
} PortIndex;

