// Refrences:
// - https://github.com/gtrxAC/peanut.vxp/blob/main/src/util.c

#include "common.h"

VMFILE log_file = -1;

void log_init()
{
    if (log_file >= 0)
    {
        return;
    }
    vm_ascii_to_ucs2(ucs2_str, 128, RING_DIR"log.txt");
    log_file = vm_file_open(ucs2_str, MODE_CREATE_ALWAYS_WRITE, 1);
    log_write("--- Log Initialized ---");
}

void log_close(void)
{
    if (log_file >= 0)
    {
        log_write("--- Log Closed ---");
        vm_file_close(log_file);
        log_file = -1;
    }
}

void log_write(const char *str)
{
    if (log_file < 0)
        return;
    if (!str)
        str = "(null)";
    int unused;
    vm_file_write(log_file, (void *)str, strlen(str), &unused);
    vm_file_write(log_file, "\r\n", 2, &unused);
    vm_file_commit(log_file);
}

int log_printf(const char *format, ...)
{
    if (log_file < 0)
        return 0;
    static char buf[1024 * 4];
    va_list aptr;

    va_start(aptr, format);
    int ret = vsnprintf(buf, sizeof(buf), format, aptr);
    va_end(aptr);

    if (ret >= 0)
    {
        log_write(buf);
    }
    else
    {
        log_write("Error formatting log message.");
    }
    return ret;
}

int read_from_file_to_addr(const char *path_, void **addr)
{
    char path[128];
    VMUINT red = 0, size = 0;
    VMFILE f = -1;

    if (!path_ || !addr)
    {
        log_write("read_from_file_to_addr: Invalid arguments (path or addr is NULL)");
        if (addr)
            *addr = NULL;
        return 0;
    }
    *addr = NULL;

    if (strncmp(path_, RING_DIR, 8) == 0)
    {
        strncpy(path, path_, sizeof(path) - 1);
    }
    else
    {
        snprintf(path, sizeof(path), RING_DIR"%s", path_);
    }
    path[sizeof(path) - 1] = '\0';

    log_printf("Attempting to read file: %s", path);

    vm_ascii_to_ucs2(ucs2_str, 128, (VMSTR)path);
    f = vm_file_open(ucs2_str, MODE_READ, 1);

    if (f < 0)
    {
        log_printf("File not found or cannot open: %s (Error code: %d)", path, f);
        return 0;
    }

    if (vm_file_getfilesize(f, &size) < 0 || size == 0)
    {
        log_printf("Failed to get file size or file is empty: %s", path);
        vm_file_close(f);
        return 0;
    }

    log_printf("File size: %u bytes", size);

    *addr = malloc(size + 1);
    if (*addr == NULL)
    {
        log_printf("Memory allocation failed for file content. Requested size: %u bytes.", size + 1);
        vm_file_close(f);
        return 0;
    }

    if (vm_file_read(f, *addr, size, &red) < 0 || red != size)
    {
        log_printf("File read error or incomplete read (read %u of %u bytes).", red, size);
        free(*addr);
        *addr = NULL;
        vm_file_close(f);
        return 0;
    }

    ((char *)*addr)[size] = '\0';

    log_printf("Successfully read %u bytes from %s", red, path);
    vm_file_close(f);
    return (int)size;
}
