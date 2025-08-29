#include "part-1.h"

IMAGE_DOS_HEADER* get_dos_header(int fd) {
    IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*) malloc(sizeof(struct _IMAGE_DOS_HEADER));
    if (dos_header == NULL) {
        return NULL;
    }

    int read_amnt = read(fd, dos_header, sizeof(struct _IMAGE_DOS_HEADER));
    if (read_amnt == -1) {
        return NULL;
    }

    // if (DO_PRINT) {
    //     print_dos_header(dos_header);
    // }
    //
    return dos_header;
}

IMAGE_NT_HEADERS* get_nt_headers(int fd) {
         
    IMAGE_NT_HEADERS* nt_header = (IMAGE_NT_HEADERS*)malloc(sizeof(IMAGE_NT_HEADERS));
    if (nt_header == NULL) {
        return NULL;
    }

    IMAGE_FILE_HEADER* file_header = (IMAGE_FILE_HEADER*)malloc(sizeof(IMAGE_FILE_HEADER));
    if (file_header == NULL) {
        return NULL;
    }

    IMAGE_OPTIONAL_HEADER64* optional_header = (IMAGE_OPTIONAL_HEADER64*)malloc(sizeof(IMAGE_OPTIONAL_HEADER64));
    if (optional_header == NULL) {
        return NULL;
    }

    // TODO: break this up into their own individual functions should be better I think 
    read(fd, nt_header, sizeof(IMAGE_NT_HEADERS));
    // if (DO_PRINT) {
    //     print_nt_header(nt_header);
    // }

    // printf("%d\n", sizeof(IMAGE_NT_HEADERS));
    // printf("%d\n", sizeof(IMAGE_FILE_HEADER));
    // printf("%d\n", sizeof(IMAGE_OPTIONAL_HEADER64));
    // printf("%d\n", sizeof(IMAGE_DATA_DIRECTORY));
    return nt_header;
}

IMAGE_SECTION_HEADER** get_section_arr(int fd, int num_sections) {
    printf("Number of sections: %d\n", num_sections);
    IMAGE_SECTION_HEADER** image_section_arr = (IMAGE_SECTION_HEADER**) malloc(sizeof(IMAGE_SECTION_HEADER*) * num_sections);

    for (int i = 0; i < num_sections; i++) {
        printf("Section number: %d\n", i);
        IMAGE_SECTION_HEADER* image_section = (IMAGE_SECTION_HEADER*)malloc(sizeof(IMAGE_SECTION_HEADER));
        read(fd, image_section, sizeof(IMAGE_SECTION_HEADER));
        image_section_arr[i] = image_section;
    }
    return image_section_arr;
}
