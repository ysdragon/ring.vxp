#pragma once
#include "common.h"

typedef enum Menu {
	MENU_MAIN,
	MENU_FILE_PICKER,
	MENU_ABOUT,
	MENU_FILE_ACTION
} Menu;

typedef struct {
	Menu menu_type;
	int menu_choice;
	char dir_path[FILENAME_MAX];
	char selected_file[FILENAME_MAX];
} MenuState;

void draw_menu();
void set_menu(Menu menu);
void handle_keyevt_menu(VMINT event, VMINT keycode);

void menu_back();
void menu_confirm();
void save_menu_state();
void restore_menu_state();