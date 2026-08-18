#include "setup.h"

IMAGE_DOS_HEADER* get_dos_header(FILE* fp) {
    fseek(fp, 0, SEEK_SET);
    IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*) malloc(sizeof(IMAGE_DOS_HEADER));
    printf("%d\n", sizeof(IMAGE_DOS_HEADER));

    if (dos_header == NULL) {
        return NULL;
    }

    int read_amnt = fread(dos_header, sizeof(IMAGE_DOS_HEADER), 1, fp);
    printf("amount read: %d\n", read_amnt);
    if (read_amnt == 0) {
        return NULL;
    }
    return dos_header;
}

// TODO: need to work on this and figure out why i used the malloc stuff
IMAGE_NT_HEADERS* get_nt_headers(FILE* fp) {

    int magic_offset = g_dos_header->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER);
    if (fseek(fp, magic_offset, SEEK_SET)) {
        strerror(errno);
        exit(1);
    };

    if (!fread(&g_version, sizeof(WORD), 1, fp)) {
        strerror(errno);
        exit(1);
    };

    printf("Magic Offset 1: %x\n", magic_offset);
    printf("Version: %x\n", g_version);
    if (fseek(fp, g_dos_header->e_lfanew, SEEK_SET)) {
        strerror(errno);
        exit(1);
    }

    IMAGE_NT_HEADERS* nt_header = (IMAGE_NT_HEADERS*)malloc(sizeof(IMAGE_NT_HEADERS));
    if (nt_header == NULL) {
        strerror(errno);
        exit(1);
    }

    IMAGE_FILE_HEADER* file_header = (IMAGE_FILE_HEADER*)malloc(sizeof(IMAGE_FILE_HEADER));
    if (file_header == NULL) {
        strerror(errno);
        exit(1);
    }

    // TODO: need to double check this, i think there is IMAGE_OPTIONAL_HEADER, IMAGE_OPTIONAL_HEADER64, and IMAGE_OPTIONAL_HEADER32
    IMAGE_OPTIONAL_HEADER* optional_header = (IMAGE_OPTIONAL_HEADER64*)malloc(sizeof(IMAGE_OPTIONAL_HEADER64));
    if (optional_header == NULL) {
        return NULL;
    }

    fread(nt_header, sizeof(IMAGE_NT_HEADERS), 1, fp);
    return nt_header;
}

IMAGE_SECTION_HEADER* get_section_arr(FILE* fp, unsigned int num_sections) {
    int magic_offset = g_dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS); 
    unsigned int mem_size = sizeof(IMAGE_SECTION_HEADER) * num_sections;
    // inside of a .obj file -> use physical addy
    // inside of a .exe / .dll -> use v size
    IMAGE_SECTION_HEADER* image_section_header_arr = (IMAGE_SECTION_HEADER*) malloc(mem_size);
    printf("%u\n", mem_size);
    fread(image_section_header_arr, mem_size, 1, fp);
    return image_section_header_arr;

}

void init_globals(FILE* fp){
    g_dos_header = get_dos_header(fp);     
    g_nt_headers = get_nt_headers(fp);
    if (g_nt_headers == NULL) {
        puts("Null NT Headers returned");
    }
    unsigned short num_sections = g_nt_headers->FileHeader.NumberOfSections;
    printf("%d\n", num_sections);
    g_section_arr = get_section_arr(fp, num_sections);
}

int main(int argc, char** argv) {

    if (argv[1] == NULL) {
        printf("No file was provided\n");
        return -1;
    }
    
    FILE* fp = fopen(argv[1], "r+");
    if (fp == NULL) {
        printf("File open failed\n");
        return -1;
    }

    init_globals(fp);
}

