#include "part-3.h"


// i think it would be better if this were called from the outside
int get_latest_section(IMAGE_SECTION_HEADER** section_arr, int num_sections) {

    int cur_hi_va = 0x0;  // current highest virtual address
    int cur_hi_vsz = 0x0;  // current highest size matching the virtual address
    IMAGE_SECTION_HEADER* cur_section;

    for (int i = 0; i < num_sections; i++) {
        cur_section = section_arr[i];
        if (cur_section->VirtualAddress > cur_hi_va) {
            cur_hi_va = cur_section->VirtualAddress;
            cur_hi_vsz = cur_section->VirtualSize;
        }
    }
    return cur_hi_va + cur_hi_vsz;
}

// 
IMAGE_SECTION_HEADER* add_entry_to_section_headers(int va, int sz, int pa) {
    // first, construct new section header 
    // calculate the virtual address 
    
    IMAGE_SECTION_HEADER* new_section = (IMAGE_SECTION_HEADER*) malloc(sizeof(IMAGE_SECTION_HEADER));
    new_section->VirtualSize = sz;
    new_section->VirtualAddress = va;
    new_section->SizeOfRawData = sz;
    new_section->PointerToRawData = pa;
    new_section->PointerToRelocations = 0x0;
    new_section->PointerToLinenumbers = 0x0;
    new_section->NumberOfRelocations = 0x0;
    new_section->NumberOfLinenumbers = 0x0;
    new_section->Charactersitics = IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE;
    return new_section;
}

// TODO: should physical addr be int or unsigned long long or soething?
// TODO add physical addr to the arguments instead of always appending to the end of the file
int add_new_section(int fd) {
    lseek(fd, 0, SEEK_END);
    int file_sz = ftell(file);
    int pa = file_sz;
    int va = get_latest_section();
    add_entry_to_section_headers(va, sz, pa);    
}

int modify_size_of_image(int fd, DWORD new_image_size) {
    int image_size_offset = sizeof(IMAGE_DOS_HEADER) + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + OPTIONAL_HEADER_SIZE_OF_IMAGE_OFFSET;
    lseek(fd, image_size_offset, SEEK_SET);
    int is_ok = write(fd, &new_image_size, sizeof(DWORD));
    if (is_ok == -1) {
        perror("Failed to write size of image");
    }
    return is_ok;
}

int modify_number_of_sections(int fd, int new_num_of_sections) {

    int num_of_sections_offset = sizeof(IMAGE_DOS_HEADER) + sizeof(DWORD) + sizeof(WORD);
    lseek(fd, num_of_sections_offset, SEEK_SET);
    WORD val_to_write = (WORD) new_num_of_sections;
    // does the pointer to the int work??
    // i feel like it shouldnt because the argument is in a register, not a buffer
    int is_ok = write(fd, &val_to_write, sizeof(WORD));
    if (is_ok == -1) {
        perror("Failed to write number of sections offset");
    }
    return is_ok;
}


