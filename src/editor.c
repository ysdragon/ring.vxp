#include "common.h"

VMFILE f_read;
VMWCHAR file_pathw[100] = {0, 0};
VMWCHAR title_name[100] = {0, 0};

VMWCHAR *g_edit_buffer = NULL;

void save_text(VMINT state, VMWSTR text) {

	if (g_edit_buffer) {
		vm_free(g_edit_buffer);
		g_edit_buffer = NULL;
	}

	if (state == VM_INPUT_OK) {
		VMINT len = wstrlen(text);
		if (len > 0) {

			VMCHAR *new_data = vm_malloc(len * 3 + 1);
			if (new_data) {
				f_read =
					vm_file_open(file_pathw, MODE_CREATE_ALWAYS_WRITE, FALSE);
				if (f_read >= 0) {
					VMUINT nwrite;
					vm_chset_convert(VM_CHSET_UCS2, VM_CHSET_UTF8,
									 (VMCHAR *)text, new_data, len * 3 + 1);
					vm_file_write(f_read, new_data, strlen(new_data), &nwrite);
					vm_file_close(f_read);
				}
				vm_free(new_data);
			}
		}
	}
	set_state(ST_MENU);
}

void read_file_to_input(VMWSTR path) {
	VMCHAR *text_data = NULL;
	VMUINT nread, file_size;

	f_read = vm_file_open(path, MODE_READ, FALSE);
	if (f_read < 0)
		return;

	if (vm_file_getfilesize(f_read, &file_size) < 0) {
		vm_file_close(f_read);
		return;
	}

	text_data = vm_malloc(file_size + 1);
	if (!text_data) {
		vm_file_close(f_read);
		return;
	}

	vm_file_read(f_read, text_data, file_size, &nread);
	text_data[nread] = '\0';
	vm_file_close(f_read);

	VMINT ucs2_buf_size_bytes = nread * 2 + 2;
	g_edit_buffer = vm_malloc(ucs2_buf_size_bytes);

	if (!g_edit_buffer) {
		vm_free(text_data);
		return;
	}
	memset(g_edit_buffer, 0, ucs2_buf_size_bytes);

	VMINT max_chars = nread;
	vm_chset_convert(VM_CHSET_UTF8, VM_CHSET_UCS2, text_data,
					 (VMCHAR *)g_edit_buffer, ucs2_buf_size_bytes);

	vm_input_text3(g_edit_buffer, 5000, 0, save_text);

	vm_free(text_data);
}

void edit_file(const char *path) {

	if (strlen(path) >= 99) {

		return;
	}

	vm_ascii_to_ucs2(title_name, 100, (char *)path);
	vm_input_set_editor_title(title_name);

	vm_ascii_to_ucs2(file_pathw, 100, (char *)path);
	read_file_to_input(file_pathw);
}