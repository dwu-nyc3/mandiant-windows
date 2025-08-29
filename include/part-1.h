#include <windows.h> // TODO: this should be moved into the makefile i think 
#include <fcntl.h>
#include <assert.h>
#include <error.h>
#include "printer.h"

# define DO_PRINT 1

/**
    * get_dos_header - Construct DOS header
    * @fd: file descriptor pointing to file we want info about
    *
    * This function prints out the fields on the dos header and then returns a pointer to memory on the heap containing the header information
    *
    * Return: IMAGE_DOS_HEADER* on success and NULL on failure
    */

IMAGE_DOS_HEADER* get_dos_header(int fd);

/**
    * get_nt_headers - Get NT header
    *
    */
IMAGE_NT_HEADERS* get_nt_headers(int fd);

/**
    * Get section headers
    */
IMAGE_SECTION_HEADER** get_section_arr(int fd, int num_sections);


