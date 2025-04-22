#pragma once

#include "vmsys.h"

void log_init();
void log_write(const char *str);
int log_printf(const char *format, ...);

int read_from_file_to_addr(const char *path_, void **addr);

#define MAX(a, b) (((double)a) > ((double)b) ? (a) : (b))