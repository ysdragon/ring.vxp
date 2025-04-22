#pragma once

#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "vmtimer.h"
#include "vmmm.h"
#include "vm4res.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "ring.h"
#include "stb_ds.h"
#include "menu.h"

typedef enum State
{
	ST_MENU,
	ST_RUNNING,
} State;

#define RING_DIR "e:\\ring\\"

#define HINT_BAR_HEIGHT 18
#define TITLE_HEIGHT 18
#define SOFTKEY_HEIGHT 18

#define MAIN_COLOR MRE_RGB(45, 84, 203)
#define SEPARATOR_COLOR MRE_RGB(128, 128, 128)

#define SCREEN_WIDTH vm_graphic_get_screen_width()
#define SCREEN_HEIGHT vm_graphic_get_screen_height()

extern State current_state;
extern char currentScript[FILENAME_MAX];
extern char currentDir[FILENAME_MAX];
extern VMWCHAR ucs2_str[128];
extern vm_graphic_color color;
extern VMINT layer_hdl[1];
extern char **menu_list;
extern Menu menu;

void set_state(State new_state);
void handle_sysevt(VMINT message, VMINT param);
void handle_keyevt(VMINT event, VMINT keycode);
static void draw_running_state(void);
