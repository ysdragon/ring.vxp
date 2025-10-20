#include "common.h"

int menu_choice = 0;
char currentDir[FILENAME_MAX] = RING_DIR;

MenuState last_menu_state = {MENU_MAIN, 0, RING_DIR, ""};
char file_action_path[FILENAME_MAX] = {0};

void draw_menu()
{
    if (layer_hdl[0] == -1)
        return;

    color.vm_color_565 = VM_COLOR_BLACK;
    vm_graphic_setcolor(&color);
    vm_graphic_fill_rect_ex(layer_hdl[0], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    color.vm_color_565 = MAIN_COLOR;
    vm_graphic_setcolor(&color);
    vm_graphic_fill_rect_ex(layer_hdl[0], 0, 0, SCREEN_WIDTH, TITLE_HEIGHT);

    color.vm_color_565 = VM_COLOR_WHITE;
    vm_graphic_setcolor(&color);
    char title_str[32];
    switch (menu)
    {
    case MENU_MAIN:
        strcpy(title_str, "Main Menu");
        break;
    case MENU_FILE_PICKER:
        strcpy(title_str, "Select Script");
        break;
    case MENU_ABOUT:
        strcpy(title_str, "About");
        break;
    case MENU_FILE_ACTION:
        strcpy(title_str, file_action_path);
        break;
    default:
        strcpy(title_str, "Menu");
        break;
    }
    vm_ascii_to_ucs2(ucs2_str, 128, title_str);
    int title_x = (SCREEN_WIDTH - vm_graphic_get_string_width(ucs2_str)) / 2;
    int title_y = (TITLE_HEIGHT - vm_graphic_get_character_height()) / 2;
    vm_graphic_textout_to_layer(layer_hdl[0], title_x, title_y, ucs2_str, vm_wstrlen(ucs2_str));

    color.vm_color_565 = SEPARATOR_COLOR;
    vm_graphic_setcolor(&color);
    vm_graphic_line_ex(layer_hdl[0], 0, TITLE_HEIGHT, SCREEN_WIDTH - 1, TITLE_HEIGHT);

    int menu_area_y = TITLE_HEIGHT + 1;
    int menu_area_height = SCREEN_HEIGHT - TITLE_HEIGHT - 1 - SOFTKEY_HEIGHT - 1;
    int row_height = vm_graphic_get_character_height() + 6;
    int items_on_screen = menu_area_height / row_height;
    if (items_on_screen <= 0)
        items_on_screen = 1;

    int total_items = arrlen(menu_list);
    int first_item_index = 0;

    first_item_index = menu_choice - (items_on_screen / 2);
    if (first_item_index < 0)
    {
        first_item_index = 0;
    }
    if (first_item_index > total_items - items_on_screen)
    {
        first_item_index = total_items - items_on_screen;
        if (first_item_index < 0)
            first_item_index = 0;
    }

    for (int i = 0; i < items_on_screen; i++)
    {
        int item_index = first_item_index + i;
        if (item_index >= total_items)
            break;

        int row_y_start = menu_area_y + i * row_height;
        int text_y = row_y_start + (row_height - vm_graphic_get_character_height()) / 2;

        if (item_index == menu_choice)
        {
            color.vm_color_565 = MAIN_COLOR;
            vm_graphic_setcolor(&color);
            vm_graphic_fill_rect_ex(layer_hdl[0], 0, row_y_start, SCREEN_WIDTH, row_height);
            color.vm_color_565 = VM_COLOR_WHITE;
            vm_graphic_setcolor(&color);
        }
        else
        {
            color.vm_color_565 = VM_COLOR_WHITE;
            vm_graphic_setcolor(&color);
        }

        vm_ascii_to_ucs2(ucs2_str, 128, menu_list[item_index]);
        vm_graphic_textout_to_layer(layer_hdl[0], 10, text_y, ucs2_str, vm_wstrlen(ucs2_str));
    }

    if (total_items > items_on_screen)
    {
        color.vm_color_565 = VM_COLOR_WHITE;
        vm_graphic_setcolor(&color);
        if (first_item_index > 0)
        {
            vm_ascii_to_ucs2(ucs2_str, 128, "^");
            vm_graphic_textout_to_layer(layer_hdl[0], SCREEN_WIDTH - 12, menu_area_y + 2, ucs2_str, 1);
        }
        if (first_item_index + items_on_screen < total_items)
        {
            vm_ascii_to_ucs2(ucs2_str, 128, "v");
            vm_graphic_textout_to_layer(layer_hdl[0], SCREEN_WIDTH - 12, menu_area_y + menu_area_height - vm_graphic_get_character_height() - 2, ucs2_str, 1);
        }
    }

    int softkey_separator_y = SCREEN_HEIGHT - SOFTKEY_HEIGHT - 1;
    color.vm_color_565 = SEPARATOR_COLOR;
    vm_graphic_setcolor(&color);
    vm_graphic_line_ex(layer_hdl[0], 0, softkey_separator_y, SCREEN_WIDTH - 1, softkey_separator_y);

    int softkey_y = SCREEN_HEIGHT - SOFTKEY_HEIGHT;
    color.vm_color_565 = MAIN_COLOR;
    vm_graphic_setcolor(&color);
    vm_graphic_fill_rect_ex(layer_hdl[0], 0, softkey_y, SCREEN_WIDTH, SOFTKEY_HEIGHT);

    color.vm_color_565 = VM_COLOR_WHITE;
    vm_graphic_setcolor(&color);

    VMWCHAR select_text[20], back_text[10];
    vm_ascii_to_ucs2(select_text, 20, "Select");
    vm_ascii_to_ucs2(back_text, 10, "Back");
    int softkey_text_y = softkey_y + (SOFTKEY_HEIGHT - vm_graphic_get_character_height()) / 2;

    if (menu != MENU_ABOUT)
    {
        vm_graphic_textout_to_layer(layer_hdl[0], 5, softkey_text_y, select_text, vm_wstrlen(select_text));
    }

    if (menu != MENU_MAIN)
    {
        vm_graphic_textout_to_layer(
            layer_hdl[0], SCREEN_WIDTH - 5 - vm_graphic_get_string_width(back_text),
            softkey_text_y, back_text, vm_wstrlen(back_text));
    }

    vm_graphic_flush_layer(layer_hdl, 1);
}

void go_up_directory()
{
    int len = strlen(currentDir);
    if (len > 0 && currentDir[len - 1] == '\\')
    {
        len--;
    }

    while (len > 0 && currentDir[len - 1] != '\\')
    {
        len--;
    }

    if (len < 8)
    {
        return;
    }

    currentDir[len] = '\0';
}

void set_menu(Menu new_menu)
{
    if (menu_list != NULL)
    {
        if (menu == MENU_FILE_PICKER)
        {
            for (int i = 0; i < arrlen(menu_list); i++)
            {
                if (menu_list[i])
                    free(menu_list[i]);
            }
        }
        arrfree(menu_list);
        menu_list = NULL;
    }

    menu = new_menu;
    menu_choice = 0;
    switch (menu)
    {
    case MENU_MAIN:
    {
        arrput(menu_list, "Run Ring Script");
        arrput(menu_list, "About Ring");
        arrput(menu_list, "Exit");
        break;
    }
    case MENU_FILE_ACTION:
    {
        arrput(menu_list, "Run");
        arrput(menu_list, "Edit");
        break;
    }

    case MENU_FILE_PICKER:
    {
        struct vm_fileinfo_t fileinfo;
        VMINT find_handle;
        VMINT find_result;
        int count = 0;

        if (strcmp(currentDir, RING_DIR) != 0)
        {
            char *parent_dir = malloc(15);
            if (parent_dir)
            {
                strcpy(parent_dir, "[DIR] ..");
                arrput(menu_list, parent_dir);
                count++;
            }
        }

        VMWCHAR search_all[FILENAME_MAX];
        vm_ascii_to_ucs2(search_all, FILENAME_MAX, currentDir);
        vm_wstrcat(search_all, (VMWSTR)L"*.*");

        find_handle = vm_find_first(search_all, &fileinfo);
        if (find_handle >= 0)
        {
            do
            {
                char ascii_name[FILENAME_MAX];
                vm_ucs2_to_ascii(ascii_name, FILENAME_MAX, fileinfo.filename);

                if (strcmp(ascii_name, ".") != 0 && strcmp(ascii_name, "..") != 0)
                {
                    VMWCHAR full_path[FILENAME_MAX];
                    vm_ascii_to_ucs2(full_path, FILENAME_MAX, currentDir);
                    vm_wstrcat(full_path, fileinfo.filename);

                    VMINT attr = vm_file_get_attributes(full_path);
                    if (attr >= 0 && (attr & VM_FS_ATTR_DIR))
                    {
                        char *dir_item = malloc(strlen(ascii_name) + 7);
                        if (dir_item)
                        {
                            sprintf(dir_item, "[DIR] %s", ascii_name);
                            arrput(menu_list, dir_item);
                            count++;
                        }
                    }
                }
                find_result = vm_find_next(find_handle, &fileinfo);
            } while (find_result >= 0);
            vm_find_close(find_handle);
        }

        VMWCHAR search_ring[FILENAME_MAX];
        vm_ascii_to_ucs2(search_ring, FILENAME_MAX, currentDir);
        vm_wstrcat(search_ring, (VMWSTR)L"*.ring");

        find_handle = vm_find_first(search_ring, &fileinfo);
        if (find_handle >= 0)
        {
            do
            {
                VMWCHAR full_path[FILENAME_MAX];
                vm_ascii_to_ucs2(full_path, FILENAME_MAX, currentDir);
                vm_wstrcat(full_path, fileinfo.filename);

                VMINT attr = vm_file_get_attributes(full_path);
                if (attr >= 0 && !(attr & VM_FS_ATTR_DIR))
                {
                    char ascii_name[FILENAME_MAX];
                    vm_ucs2_to_ascii(ascii_name, FILENAME_MAX, fileinfo.filename);

                    int len = strlen(ascii_name);
                    if (len > 5 && strcmp(ascii_name + len - 5, ".ring") == 0)
                    {
                        char *file_item = malloc(len + 1);
                        if (file_item)
                        {
                            strcpy(file_item, ascii_name);
                            arrput(menu_list, file_item);
                            count++;
                        }
                    }
                }
                find_result = vm_find_next(find_handle, &fileinfo);
            } while (find_result >= 0);
            vm_find_close(find_handle);
        }

        if (count == 0)
        {
            char *no_files_str = malloc(20);
            if (no_files_str)
            {
                strcpy(no_files_str, "Empty directory");
                arrput(menu_list, no_files_str);
            }
        }

        break;
    }

    case MENU_ABOUT:
    {
        arrput(menu_list, "Ring Embedded in MRE");
        arrput(menu_list, "Version: " RING_VXP_VERSION);
        arrput(menu_list, "RingVM: " RING_STATE_VERSION);
        arrput(menu_list, "Copyright (C) 2025");
        arrput(menu_list, "ysdragon");
        arrput(menu_list, "github.com/ysdragon");

        break;
    }
    }
}

void menu_back()
{
    switch (menu)
    {
    case MENU_MAIN:
        vm_exit_app();
        break;

    case MENU_FILE_PICKER:
        if (strcmp(currentDir, RING_DIR) != 0)
        {
            go_up_directory();
            set_menu(MENU_FILE_PICKER);
            draw_menu();
        }
        else
        {
            set_menu(MENU_MAIN);
            draw_menu();
        }
        break;
    case MENU_FILE_ACTION:
        set_menu(MENU_FILE_PICKER);
        draw_menu();
        break;

    case MENU_ABOUT:
        set_menu(MENU_MAIN);
        draw_menu();
        break;
    }
}

void menu_confirm()
{
    if (menu_list == NULL || menu_choice < 0 || menu_choice >= arrlen(menu_list))
    {
        return;
    }

    const char *selected_item = menu_list[menu_choice];

    switch (menu)
    {
    case MENU_MAIN:
    {
        if (!strcmp(selected_item, "Run Ring Script"))
        {
            strcpy(currentDir, RING_DIR);
            set_menu(MENU_FILE_PICKER);
            draw_menu();
        }
        else if (!strcmp(selected_item, "About Ring"))
        {
            set_menu(MENU_ABOUT);
            draw_menu();
        }
        else if (!strcmp(selected_item, "Exit"))
        {
            vm_exit_app();
        }
        break;
    }

    case MENU_FILE_PICKER:
        if (!strcmp(selected_item, "Empty directory"))
        {
            break;
        }

        if (!strcmp(selected_item, "[DIR] .."))
        {
            go_up_directory();
            set_menu(MENU_FILE_PICKER);
            draw_menu();
            break;
        }

        if (strncmp(selected_item, "[DIR] ", 6) == 0)
        {
            const char *dir_name = selected_item + 6;

            strncat(currentDir, dir_name, FILENAME_MAX - strlen(currentDir) - 2);
            strcat(currentDir, "\\");

            set_menu(MENU_FILE_PICKER);
            draw_menu();
            break;
        }

        if (strncmp(selected_item, "[DIR]", 5) != 0 && strcmp(selected_item, "Empty directory") != 0)
        {
            strcpy(file_action_path, selected_item);
            set_menu(MENU_FILE_ACTION);
            draw_menu();
        }
        break;
    case MENU_FILE_ACTION:
    {
        if (!strcmp(selected_item, "Run"))
        {
            save_menu_state();

            strcpy(currentScript, currentDir);
            strncat(currentScript, file_action_path, FILENAME_MAX - strlen(currentScript) - 1);

            set_state(ST_RUNNING);
        }
        else if (!strcmp(selected_item, "Edit"))
        {
            save_menu_state();

            strcpy(currentScript, currentDir);
            strncat(currentScript, file_action_path, FILENAME_MAX - strlen(currentScript) - 1);

            set_state(ST_EDITOR);
        }
        break;
    }

    case MENU_ABOUT:
        set_menu(MENU_MAIN);
        draw_menu();
        break;
    }
}

void menu_up(int count)
{
    if (arrlen(menu_list) <= 0)
    {
        return;
    }
    menu_choice -= count;
    if (menu_choice < 0)
    {
        menu_choice = (count == 1) ? (arrlen(menu_list) - 1) : 0;
    }
}
void menu_down(int count)
{
    if (arrlen(menu_list) <= 0)
    {
        return;
    }

    menu_choice += count;
    if (menu_choice >= arrlen(menu_list))
    {
        menu_choice = (count == 1) ? 0 : (arrlen(menu_list) - 1);
    }
}

void handle_keyevt_menu(VMINT event, VMINT keycode)
{
    if (event != VM_KEY_EVENT_DOWN)
        return;

    if (keycode == VM_KEY_BACK || keycode == VM_KEY_RIGHT_SOFTKEY)
    {
        menu_back();
        return;
    }

    if (keycode == VM_KEY_OK || keycode == VM_KEY_NUM5 || keycode == VM_KEY_LEFT_SOFTKEY)
    {
        menu_confirm();
        return;
    }

    int prev_choice = menu_choice;
    int row_height = vm_graphic_get_character_height() + 6;
    int menu_area_height = SCREEN_HEIGHT - TITLE_HEIGHT - 1 - SOFTKEY_HEIGHT - 1;
    int items_on_screen = menu_area_height / row_height;
    if (items_on_screen <= 0)
        items_on_screen = 1;

    switch (keycode)
    {
    case VM_KEY_UP:
    case VM_KEY_NUM2:
        menu_up(1);
        break;
    case VM_KEY_DOWN:
    case VM_KEY_NUM8:
        menu_down(1);
        break;
    case VM_KEY_LEFT:
    case VM_KEY_NUM4:
        menu_up(MAX(1, items_on_screen - 1));
        break;
    case VM_KEY_RIGHT:
    case VM_KEY_NUM6:
        menu_down(MAX(1, items_on_screen - 1));
        break;
    default:
        break;
    }

    if (menu_choice != prev_choice && layer_hdl[0] != -1 && current_state == ST_MENU)
    {
        draw_menu();
    }
}

void save_menu_state()
{
    last_menu_state.menu_type = menu;
    last_menu_state.menu_choice = menu_choice;
    strcpy(last_menu_state.dir_path, currentDir);
    if (menu == MENU_FILE_ACTION)
    {
        strcpy(last_menu_state.selected_file, file_action_path);
    }
    else
    {
        last_menu_state.selected_file[0] = '\0';
    }
}

void restore_menu_state()
{
    if (last_menu_state.menu_type != MENU_MAIN || last_menu_state.menu_choice != 0)
    {
        strcpy(currentDir, last_menu_state.dir_path);

        int saved_choice = last_menu_state.menu_choice;
        set_menu(last_menu_state.menu_type);
        if (last_menu_state.menu_type == MENU_FILE_ACTION)
        {
            strcpy(file_action_path, last_menu_state.selected_file);
        }

        if (saved_choice >= 0 && saved_choice < arrlen(menu_list))
        {
            menu_choice = saved_choice;
        }
    }
    else
    {
        set_menu(MENU_MAIN);
    }
}