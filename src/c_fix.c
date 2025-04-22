#ifndef WIN32
#include "vmstdlib.h"
#include "vmsys.h"

typedef VMINT (*vm_get_sym_entry_t)(char *symbol);
vm_get_sym_entry_t vm_get_sym_entry;
typedef void (**__init_array)(void);
extern unsigned int __init_array_start;
extern unsigned int __init_array_end;

void _sbrk() {}
void _write(int fd, const void *buffer, unsigned int count) {}
void _close() {}
void _lseek() {}
void _open() {}
void _read() {}
void _exit(int status)
{
    vm_exit_app();
    while(1);
}
void _getpid() {}
void _kill() {}
void _fstat() {}
void _isatty() {}

int fiprintf(FILE *fd, const char *format, ...)
{
	char buffer[200];
	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vm_vsprintf(buffer, format, aptr);
	va_end(aptr);

	return (ret);
}
int sprintf(char *buffer, const char *format, ...)
{
	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vm_vsprintf(buffer, format, aptr);
	va_end(aptr);

	return (ret);
}
int snprintf(char *buffer, size_t n, const char *format, ...)
{
	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vm_vsprintf(buffer, format, aptr);
	va_end(aptr);

	return (ret);
}

void *malloc(size_t size)
{
	return vm_malloc(size);
}

void *calloc(size_t number, size_t size)
{
	return vm_calloc(size * number);
}

void *realloc(void *ptr, size_t newsize)
{
	return vm_realloc(ptr, newsize);
}

void free(void *ptr)
{
	vm_free(ptr);
}

void *_malloc_r(struct _reent *unused, size_t size)
{
	(void)unused;
	void *ret = malloc(size);
	return ret;
}

void *__real__malloc_r(struct _reent *unused, size_t size)
{
	(void)unused;
	void *ret = malloc(size);
	return ret;
}
void *__wrap__malloc_r(struct _reent *unused, size_t size)
{
	(void)unused;
	void *ret = malloc(size);
	return ret;
}

void *_calloc_r(struct _reent *unused, size_t count, size_t size)
{
	(void)unused;
	void *ret = calloc(count, size);
	return ret;
}

void *_realloc_r(struct _reent *unused, void *ptr, size_t newsize)
{
	(void)unused;
	void *ret = realloc(ptr, newsize);
	return ret;
}
void _free_r(struct _reent *unused, void *ptr)
{
	(void)unused;
	free(ptr);
}

void gcc_entry(unsigned int entry, unsigned int init_array_start, unsigned int count)
{
	unsigned int i;
	__init_array ptr;
	vm_get_sym_entry = (vm_get_sym_entry_t)entry;

	if (init_array_start == 0)
		init_array_start = (unsigned int)&__init_array_start,
		count = ((unsigned int)&__init_array_end - (unsigned int)&__init_array_start) / 4;

	ptr = (__init_array)init_array_start;
	for (i = 1; i < count; i++)
	{
		ptr[i]();
	}
	vm_main();
}

#endif