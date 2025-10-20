#include "common.h"
#include "ring.h"

void ring_vm_mre_loadfunctions(RingState *pRingState)
{
	RING_API_REGISTER("draw_text", ring_mre_draw_text);
	RING_API_REGISTER("flush_screen", ring_mre_flush_screen);
	RING_API_REGISTER("clear_screen", ring_mre_clear_screen);
	RING_API_REGISTER("draw_rect", ring_mre_draw_rect);
	RING_API_REGISTER("fill_rect", ring_mre_fill_rect);
	RING_API_REGISTER("draw_line", ring_mre_draw_line);
	RING_API_REGISTER("draw_pixel", ring_mre_draw_pixel);
	RING_API_REGISTER("set_color", ring_mre_set_color);
	RING_API_REGISTER("background_color", ring_mre_background_color);
	RING_API_REGISTER("screen_width", ring_mre_screen_width);
	RING_API_REGISTER("screen_height", ring_mre_screen_height);

	RING_API_REGISTER("sleep", ring_mre_sleep);
	RING_API_REGISTER("memory_size", ring_mre_memory_size);

	RING_API_REGISTER("clock", ring_mre_clock);
	RING_API_REGISTER("clockspersecond", ring_mre_clockspersecond);
	RING_API_REGISTER("time", ring_mre_time);
	RING_API_REGISTER("date", ring_mre_date);
}

void ring_mre_draw_text(void *pPointer)
{
	VMINT x, y;
	char *text;
	int text_len;
	VMUINT8 r, g, b;
	VMBOOL use_custom_color = VM_FALSE;

	if (RING_API_PARACOUNT != 3 && RING_API_PARACOUNT != 6)
	{
		RING_API_ERROR("Usage: draw_text(x, y, text_string) or draw_text(x, y, text_string, r, g, b)");
		return;
	}

	if (!RING_API_ISNUMBER(1) || !RING_API_ISNUMBER(2) || !RING_API_ISSTRING(3))
	{
		RING_API_ERROR("Incorrect base parameter types for draw_text(x, y, text_string, [r, g, b])");
		return;
	}

	x = (VMINT)RING_API_GETNUMBER(1);
	y = (VMINT)RING_API_GETNUMBER(2);
	text = RING_API_GETSTRING(3);
	text_len = RING_API_GETSTRINGSIZE(3);

	if (RING_API_PARACOUNT == 6)
	{
		if (!RING_API_ISNUMBER(4) || !RING_API_ISNUMBER(5) || !RING_API_ISNUMBER(6))
		{
			RING_API_ERROR("Incorrect color parameter types for draw_text(x, y, text_string, r, g, b)");
			return;
		}
		r = (VMUINT8)RING_API_GETNUMBER(4);
		g = (VMUINT8)RING_API_GETNUMBER(5);
		b = (VMUINT8)RING_API_GETNUMBER(6);
		use_custom_color = VM_TRUE;
	}

	if (layer_hdl[0] == -1)
	{
		return;
	}

	vm_ascii_to_ucs2(ucs2_str, sizeof(ucs2_str) / sizeof(VMWCHAR), text);

	if (use_custom_color)
	{
		color.vm_color_565 = MRE_RGB(r, g, b);
	}
	else
	{
		color.vm_color_565 = VM_COLOR_WHITE;
	}
	vm_graphic_setcolor(&color);

	vm_graphic_textout_to_layer(layer_hdl[0], x, y, ucs2_str, vm_wstrlen(ucs2_str));
}

void ring_mre_clear_screen(void *pPointer)
{
	vm_graphic_color clear_color;

	if (RING_API_PARACOUNT != 0)
	{
		RING_API_ERROR("Usage: clear_screen()");
		return;
	}
	if (layer_hdl[0] == -1)
	{
		RING_API_ERROR("clear_screen: Invalid layer handle.");
		return;
	}

	clear_color.vm_color_565 = VM_COLOR_BLACK;
	vm_graphic_setcolor(&clear_color);

	vm_graphic_fill_rect_ex(layer_hdl[0], 0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
}

void ring_mre_set_color(void *pPointer)
{
	VMUINT8 r, g, b;
	if (RING_API_PARACOUNT != 3)
	{
		RING_API_ERROR("Usage: set_color(r, g, b)");
		return;
	}
	if (!RING_API_ISNUMBER(1) || !RING_API_ISNUMBER(2) || !RING_API_ISNUMBER(3))
	{
		RING_API_ERROR("Incorrect parameter types for set_color(r, g, b)");
		return;
	}
	r = (VMUINT8)RING_API_GETNUMBER(1);
	g = (VMUINT8)RING_API_GETNUMBER(2);
	b = (VMUINT8)RING_API_GETNUMBER(3);

	color.vm_color_565 = MRE_RGB(r, g, b);
}

void ring_mre_background_color(void *pPointer)
{
	VMUINT8 r, g, b;
	VMUINT16 bg_color_565;
	vm_graphic_color bg_color_struct;

	if (RING_API_PARACOUNT != 3)
	{
		RING_API_ERROR("Usage: background_color(r, g, b)");
		return;
	}
	if (!RING_API_ISNUMBER(1) || !RING_API_ISNUMBER(2) || !RING_API_ISNUMBER(3))
	{
		RING_API_ERROR("Incorrect parameter types for set_background_color(r, g, b)");
		return;
	}
	if (layer_hdl[0] == -1)
	{
		RING_API_ERROR("background_color: Invalid layer handle.");
		return;
	}

	r = (VMUINT8)RING_API_GETNUMBER(1);
	g = (VMUINT8)RING_API_GETNUMBER(2);
	b = (VMUINT8)RING_API_GETNUMBER(3);

	bg_color_565 = MRE_RGB(r, g, b);
	bg_color_struct.vm_color_565 = bg_color_565;

	vm_graphic_setcolor(&bg_color_struct);

	vm_graphic_fill_rect_ex(layer_hdl[0], 0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
}

void ring_mre_draw_rect(void *pPointer)
{
	VMINT x, y, w, h;
	if (RING_API_PARACOUNT != 4)
	{
		RING_API_ERROR("Usage: draw_rect(x, y, width, height)");
		return;
	}
	if (!RING_API_ISNUMBER(1) || !RING_API_ISNUMBER(2) || !RING_API_ISNUMBER(3) || !RING_API_ISNUMBER(4))
	{
		RING_API_ERROR("Incorrect parameter types for draw_rect(x, y, width, height)");
		return;
	}
	if (layer_hdl[0] == -1)
	{
		RING_API_ERROR("draw_rect: Invalid layer handle.");
		return;
	}
	x = (VMINT)RING_API_GETNUMBER(1);
	y = (VMINT)RING_API_GETNUMBER(2);
	w = (VMINT)RING_API_GETNUMBER(3);
	h = (VMINT)RING_API_GETNUMBER(4);
	vm_graphic_setcolor(&color);
	vm_graphic_rect_ex(layer_hdl[0], x, y, w, h);
}

void ring_mre_fill_rect(void *pPointer)
{
	VMINT x, y, w, h;
	if (RING_API_PARACOUNT != 4)
	{
		RING_API_ERROR("Usage: fill_rect(x, y, width, height)");
		return;
	}
	if (!RING_API_ISNUMBER(1) || !RING_API_ISNUMBER(2) || !RING_API_ISNUMBER(3) || !RING_API_ISNUMBER(4))
	{
		RING_API_ERROR("Incorrect parameter types for fill_rect(x, y, width, height)");
		return;
	}
	if (layer_hdl[0] == -1)
	{
		RING_API_ERROR("fill_rect: Invalid layer handle.");
		return;
	}
	x = (VMINT)RING_API_GETNUMBER(1);
	y = (VMINT)RING_API_GETNUMBER(2);
	w = (VMINT)RING_API_GETNUMBER(3);
	h = (VMINT)RING_API_GETNUMBER(4);
	vm_graphic_setcolor(&color);
	vm_graphic_fill_rect_ex(layer_hdl[0], x, y, w, h);
}

void ring_mre_draw_line(void *pPointer)
{
	VMINT x1, y1, x2, y2;
	if (RING_API_PARACOUNT != 4)
	{
		RING_API_ERROR("Usage: draw_line(x1, y1, x2, y2)");
		return;
	}
	if (!RING_API_ISNUMBER(1) || !RING_API_ISNUMBER(2) || !RING_API_ISNUMBER(3) || !RING_API_ISNUMBER(4))
	{
		RING_API_ERROR("Incorrect parameter types for draw_line(x1, y1, x2, y2)");
		return;
	}
	if (layer_hdl[0] == -1)
	{
		RING_API_ERROR("draw_line: Invalid layer handle.");
		return;
	}
	x1 = (VMINT)RING_API_GETNUMBER(1);
	y1 = (VMINT)RING_API_GETNUMBER(2);
	x2 = (VMINT)RING_API_GETNUMBER(3);
	y2 = (VMINT)RING_API_GETNUMBER(4);
	vm_graphic_setcolor(&color);
	vm_graphic_line_ex(layer_hdl[0], x1, y1, x2, y2);
}

void ring_mre_draw_pixel(void *pPointer)
{
	VMINT x, y;
	if (RING_API_PARACOUNT != 2)
	{
		RING_API_ERROR("Usage: draw_pixel(x, y)");
		return;
	}
	if (!RING_API_ISNUMBER(1) || !RING_API_ISNUMBER(2))
	{
		RING_API_ERROR("Incorrect parameter types for draw_pixel(x, y)");
		return;
	}
	if (layer_hdl[0] == -1)
	{
		RING_API_ERROR("draw_pixel: Invalid layer handle.");
		return;
	}
	x = (VMINT)RING_API_GETNUMBER(1);
	y = (VMINT)RING_API_GETNUMBER(2);
	vm_graphic_setcolor(&color);
	vm_graphic_set_pixel_ex(layer_hdl[0], x, y);
}

void ring_mre_screen_width(void *pPointer)
{
	VMINT screenWidth = vm_graphic_get_screen_width();
	if (RING_API_PARACOUNT != 0)
	{
		RING_API_ERROR("Usage: screen_width()");
		return;
	}
	RING_API_RETNUMBER(screenWidth);
}

void ring_mre_screen_height(void *pPointer)
{
	VMINT screenHeight = vm_graphic_get_screen_height();
	if (RING_API_PARACOUNT != 0)
	{
		RING_API_ERROR("Usage: screen_height()");
		return;
	}
	RING_API_RETNUMBER(screenHeight);
}

void ring_mre_flush_screen(void *pPointer)
{
	if (RING_API_PARACOUNT != 0)
	{
		RING_API_ERROR("Usage: flush_screen()");
		return;
	}
	if (layer_hdl[0] == -1)
	{
		RING_API_ERROR("flush_screen: Invalid layer handle.");
		return;
	}

	vm_graphic_flush_layer(layer_hdl, 1);
}

void ring_mre_sleep(void *pPointer)
{
	VMINT duration_ms;
	VMUINT start_ticks, current_ticks;

	if (RING_API_PARACOUNT != 1)
	{
		RING_API_ERROR("Usage: sleep(milliseconds)");
		return;
	}
	if (!RING_API_ISNUMBER(1))
	{
		RING_API_ERROR("Incorrect parameter type for sleep(number)");
		return;
	}
	duration_ms = (VMINT)RING_API_GETNUMBER(1);

	if (duration_ms <= 0)
	{
		return;
	}

	start_ticks = vm_get_tick_count();

	do
	{
		current_ticks = vm_get_tick_count();
	} while ((current_ticks - start_ticks) < (VMUINT)duration_ms);
}

void ring_mre_clock(void *pPointer)
{
	if (RING_API_PARACOUNT != 0)
	{
		RING_API_ERROR("Usage: clock()");
		return;
	}
	RING_API_RETNUMBER((double)vm_get_tick_count());
}

void ring_mre_clockspersecond(void *pPointer)
{
	if (RING_API_PARACOUNT != 0)
	{
		RING_API_ERROR("Usage: clockspersecond()");
		return;
	}

	RING_API_RETNUMBER(1000.0);
}

void ring_mre_time(void *pPointer)
{
	vm_time_t time_s;
	char time_str[9];

	if (RING_API_PARACOUNT != 0)
	{
		RING_API_ERROR("Usage: time()");
		return;
	}

	vm_get_time(&time_s);
	snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", time_s.hour, time_s.min, time_s.sec);

	RING_API_RETSTRING(time_str);
}

void ring_mre_date(void *pPointer)
{
	vm_time_t time_s;
	char date_str[11];

	if (RING_API_PARACOUNT != 0)
	{
		RING_API_ERROR("Usage: date()");
		return;
	}

	vm_get_time(&time_s);
	snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d", time_s.day, time_s.mon, time_s.year);

	RING_API_RETSTRING(date_str);
}

void ring_mre_memory_size(void *pPointer)
{
	if (RING_API_PARACOUNT != 0)
	{
		RING_API_ERROR("Usage: memory_size()");
		return;
	}
	VMUINT memorySize = vm_get_mre_total_mem_size();
	RING_API_RETNUMBER(memorySize);
}