#include <windows.h> // TODO: this should be moved into the makefile i think 
#include <fcntl.h>
#include <assert.h>
#include <error.h>
#include <stdio.h>
#include <unistd.h>

// write to file

#define OPTIONAL_HEADER_SIZE_OF_IMAGE_OFFSET 0x38

// TODO: the new section, and the new section header stuff should be specified / abstracted away i think 
// i.e. if we add a new section at the end of the file, then we need the section headers to also reflect that
// so i think add_new_section calls add_new_section_headers 

/* 
 * @brief add new section content to end of file
 *
 *
 */
int add_new_section();

/*
 * @brief add a new entry to the section header
 * @param va        new virtual address 
 * @param sz        new size
 * @param pa        location of section in the file
 *
 */
int add_entry_to_section_headers();

/*
 * @brief modify size of image in IMAGE_OPTIONAL_HEADERS
 *
 */
int modify_size_of_image(int fd, DWORD new_image_size);

/*
 * @brief nodify the number of sections in file
 *
 * @param new_num_of_sections      new number
 * @return success 0, -1 fail
 */
int modify_number_of_sections(int fd, int new_num_of_sections);


