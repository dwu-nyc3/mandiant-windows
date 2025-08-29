#include <windows.h> // TODO: this should be moved into the makefile i think 
#include <fcntl.h>
#include <assert.h>
#include <error.h>
#include <stdio.h>
/*#include "printer.h"*/

IMAGE_SECTION_HEADER* get_section_with_import_table(IMAGE_NT_HEADERS* nt_headers, IMAGE_SECTION_HEADER** section_arr, int num_sections);
