#define STB_DS_IMPLEMENTATION
#include "common.h"

/* ---------------------------------------------------------------------------
 * global variables
 * ------------------------------------------------------------------------ */

VMINT layer_hdl[1];
State current_state = ST_MENU;
char currentScript[FILENAME_MAX] = {0};
VMWCHAR ucs2_str[128];
vm_graphic_color color;
VMINT SCREEN_WIDTH;
VMINT SCREEN_HEIGHT;
char **menu_list = NULL;
Menu menu = MENU_MAIN;
RingState *pRingState = NULL;

/*
 * entry
 */
void vm_main(void)
{
    /* Create ring dir */
    vm_ascii_to_ucs2(ucs2_str, 128, RING_DIR);
    vm_file_mkdir(ucs2_str);

    /* Initialize logging */
    log_init();
    log_write("Application Started");

    /* Initialize layer handles */
    layer_hdl[0] = -1;

    /* Set initial menu */
    set_menu(MENU_MAIN);

    /* register system events handler */
    vm_reg_sysevt_callback(handle_sysevt);

    /* Set font size for text rendering */
    vm_font_set_font_size(VM_SMALL_FONT);

    /* register keyboard events handler */
    vm_reg_keyboard_callback(handle_keyevt);

    /* Init MRE resource */
    vm_res_init();
}

void set_state(State new_state)
{
    State old_state = current_state;
    current_state = new_state;

    if (layer_hdl[0] != -1)
    {
        if (current_state == ST_MENU)
        {
            if (old_state == ST_RUNNING)
            {
                cleanup_ring_state();
                restore_menu_state();
            }
            draw_menu();
        }
        else if (current_state == ST_RUNNING)
        {
            draw_running_state();
            if (!execute_script(currentScript))
            {
                log_write("Script execution failed.");
            }
        }
    }
}

void handle_sysevt(VMINT message, VMINT param)
{
    switch (message)
    {
    case VM_MSG_CREATE:
        if (layer_hdl[0] == -1)
        {
            layer_hdl[0] = vm_graphic_create_layer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, -1);
            vm_graphic_set_clip(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            vm_switch_power_saving_mode(turn_off_mode);
        }
        break;
    case VM_MSG_PAINT:
        if (layer_hdl[0] != -1)
        {
            if (current_state == ST_MENU)
            {
                draw_menu();
            }
            else if (current_state == ST_RUNNING)
            {
                draw_running_state();
            }
        }
        break;
    case VM_MSG_HIDE:
        break;
    case VM_MSG_ACTIVE:
        if (layer_hdl[0] != -1)
        {
            if (current_state == ST_MENU)
            {
                draw_menu();
            }
            else if (current_state == ST_RUNNING)
            {
                draw_running_state();
            }
        }
        break;
    case VM_MSG_QUIT:
        log_write("Quit message received");
        if (layer_hdl[0] != -1)
        {
            vm_graphic_delete_layer(layer_hdl[0]);
            layer_hdl[0] = -1;
        }

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

        cleanup_ring_state();
        log_close();
        vm_res_deinit();
        break;
    }
}

void handle_keyevt(VMINT event, VMINT keycode)
{
    if (current_state == ST_MENU)
    {
        handle_keyevt_menu(event, keycode);
    }
    else if (current_state == ST_RUNNING)
    {
        if (event == VM_KEY_EVENT_DOWN)
        {
            if (keycode == VM_KEY_BACK || keycode == VM_KEY_RIGHT_SOFTKEY)
            {
                set_state(ST_MENU);
                return;
            }
        }
    }
}

static void draw_running_state(void)
{
    log_printf("Drawing running state UI");

    if (layer_hdl[0] == -1)
    {
        log_write("Cannot draw running state, layer invalid");
        return;
    }

    vm_graphic_fill_rect_ex(layer_hdl[0], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    color.vm_color_565 = MAIN_COLOR;
    vm_graphic_setcolor(&color);
    vm_graphic_fill_rect_ex(layer_hdl[0], 0, SCREEN_HEIGHT - HINT_BAR_HEIGHT, SCREEN_WIDTH, HINT_BAR_HEIGHT);

    color.vm_color_565 = VM_COLOR_WHITE;
    vm_graphic_setcolor(&color);
    vm_ascii_to_ucs2(ucs2_str, sizeof(ucs2_str) / sizeof(VMWCHAR), "Back");
    int hint_text_y = SCREEN_HEIGHT - HINT_BAR_HEIGHT + (HINT_BAR_HEIGHT - vm_graphic_get_character_height()) / 2;
    vm_graphic_textout_to_layer(layer_hdl[0], SCREEN_WIDTH - 5 - vm_graphic_get_string_width(ucs2_str), hint_text_y, ucs2_str, vm_wstrlen(ucs2_str));

    vm_graphic_flush_layer(layer_hdl, 1);
    log_write("Base UI for running state flushed.");
}

static void cleanup_ring_state(void)
{
    if (pRingState)
    {
        log_write("Deleting Ring state...");
        ring_state_delete(pRingState);
        pRingState = NULL;
        log_write("Ring state deleted.");
    }
}

static VMBOOL execute_script(const char *script_path)
{
    log_printf("Executing script: %s", script_path);
    VMBOOL script_ok = VM_FALSE;

    if (strlen(script_path) == 0)
    {
        log_write("Cannot run script: No script selected.");
        vm_ascii_to_ucs2(ucs2_str, 128, (VMSTR) "No script selected.");
        color.vm_color_565 = VM_COLOR_RED;
        vm_graphic_setcolor(&color);
        int text_x = (SCREEN_WIDTH - vm_graphic_get_string_width(ucs2_str)) / 2;
        int text_y = (SCREEN_HEIGHT - HINT_BAR_HEIGHT - vm_graphic_get_character_height()) / 2;
        vm_graphic_textout_to_layer(layer_hdl[0], text_x, text_y, ucs2_str, vm_wstrlen(ucs2_str));
        vm_graphic_flush_layer(layer_hdl, 1);
        return VM_FALSE;
    }

    log_write("Initializing Ring state...");
    cleanup_ring_state();
    pRingState = ring_state_init();

    if (!pRingState)
    {
        log_write("Failed to initialize Ring state!");
        vm_ascii_to_ucs2(ucs2_str, 128, (VMSTR) "Ring init failed!");
        color.vm_color_565 = VM_COLOR_RED;
        vm_graphic_setcolor(&color);
        int text_x = (SCREEN_WIDTH - vm_graphic_get_string_width(ucs2_str)) / 2;
        int text_y = (SCREEN_HEIGHT - HINT_BAR_HEIGHT - vm_graphic_get_character_height()) / 2;
        vm_graphic_textout_to_layer(layer_hdl[0], text_x, text_y, ucs2_str, vm_wstrlen(ucs2_str));
        vm_graphic_flush_layer(layer_hdl, 1);
        return VM_FALSE;
    }

    log_write("Ring state initialized successfully.");

    void *script_content_orig = NULL;
    char *script_content = NULL;

    do
    {
        int script_size = read_from_file_to_addr(script_path, &script_content_orig);

        if (!script_content_orig || script_size < 0)
        {
            log_printf("Error loading script content (size: %d) or script is empty.", script_size);
            vm_ascii_to_ucs2(ucs2_str, 128, (VMSTR) "Script load error");
            break;
        }

        char *temp_ptr = (char *)realloc(script_content_orig, script_size + 1);
        if (!temp_ptr)
        {
            log_write("Error reallocating memory for null-termination.");
            free(script_content_orig);
            script_content_orig = NULL;
            vm_ascii_to_ucs2(ucs2_str, 128, (VMSTR) "Memory error");
            break;
        }

        script_content = temp_ptr;
        script_content_orig = NULL;
        script_content[script_size] = '\0';

        log_write("Executing Ring script from reallocated buffer...");
        ring_state_runcode(pRingState, script_content);
        log_write("Ring script execution finished.");
        script_ok = VM_TRUE;

        log_write("Flushing layer after script execution attempt.");
        vm_graphic_flush_layer(layer_hdl, 1);

    } while (0);

    if (script_content)
    {
        free(script_content);
        script_content = NULL;
    }

    if (script_content_orig)
    {
        free(script_content_orig);
        script_content_orig = NULL;
    }

    if (!script_ok)
    {
        color.vm_color_565 = VM_COLOR_RED;
        vm_graphic_setcolor(&color);
        int text_x = (SCREEN_WIDTH - vm_graphic_get_string_width(ucs2_str)) / 2;
        int text_y = (SCREEN_HEIGHT - HINT_BAR_HEIGHT - vm_graphic_get_character_height()) / 2;
        vm_graphic_textout_to_layer(layer_hdl[0], text_x, text_y, ucs2_str, vm_wstrlen(ucs2_str));
        vm_graphic_flush_layer(layer_hdl, 1);
        return VM_FALSE;
    }

    log_write("Script execution successful (or finished). Ring state remains active.");
    return VM_TRUE;
}