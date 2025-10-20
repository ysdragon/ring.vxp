#ifndef ring_mre_h
#define ring_mre_h

void ring_vm_mre_loadfunctions(RingState *pRingState);

void ring_mre_draw_text(void *pPointer);
void ring_mre_flush_screen(void *pPointer);
void ring_mre_clear_screen(void *pPointer);
void ring_mre_set_color(void *pPointer);
void ring_mre_background_color(void *pPointer);
void ring_mre_draw_rect(void *pPointer);
void ring_mre_fill_rect(void *pPointer);
void ring_mre_draw_line(void *pPointer);
void ring_mre_draw_pixel(void *pPointer);
void ring_mre_set_color(void *pPointer);

void ring_mre_screen_width(void *pPointer);
void ring_mre_screen_height(void *pPointer);

void ring_mre_sleep(void *pPointer);
void ring_mre_clock(void *pPointer);
void ring_mre_clockspersecond(void *pPointer);
void ring_mre_time(void *pPointer);
void ring_mre_date(void *pPointer);

void ring_mre_memory_size(void *pPointer);

#endif