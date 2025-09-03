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
            // cur_hi_vsz = cur_section->VirtualSize;
            cur_hi_vsz = cur_section->Misc.VirtualSize;
        }
    }
    return cur_hi_va + cur_hi_vsz;

}
 
IMAGE_SECTION_HEADER* add_entry_to_section_headers(int sz, int va, int pa) {
    // first, construct new section header 
    // calculate the virtual address 
    
    IMAGE_SECTION_HEADER* new_section = (IMAGE_SECTION_HEADER*) malloc(sizeof(IMAGE_SECTION_HEADER));
    new_section->Misc.VirtualSize = sz + alignment;
    new_section->VirtualAddress = va;
    new_section->SizeOfRawData = sz;
    new_section->PointerToRawData = pa;
    new_section->PointerToRelocations = 0x0;
    new_section->PointerToLinenumbers = 0x0;
    new_section->NumberOfRelocations = 0x0;
    new_section->NumberOfLinenumbers = 0x0;
    new_section->Characteristics = IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE;
    return new_section;
}

// TODO: should physical addr be int or unsigned long long or soething?
// TODO add physical addr to the arguments instead of always appending to the end of the file
// TODO there is an error here; it should be size of the section
int add_new_section(int fd, IMAGE_SECTION_HEADER** section_arr, int num_sections) {
    FILE* fp = fdopen(fd, "w"); // writing section to the end of the file
    fseek(fp, 0, SEEK_END);
    int file_sz = ftell(fp); // this is pointer to raw data
    int va = get_latest_section(section_arr, num_sections); // virtual address, which should be autoaligned?
    // align the virtual address now, which should be in the optional header
    IMAGE_SECTION_HEADER* new_section = add_entry_to_section_headers(payload_sz, va, file_sz);    
    // TODO: add the new section to the file with a write
    // need section alignment
    
    fwrite();
    
}

// ERR: need to jump to elfa_new value
//   <- resolved
int modify_size_of_image(IMAGE_NT_HEADERS* nt_headers, int fd, DWORD payload_size) {
    DWORD file_alignment = nt_headers->OptionalHeader.FileAlignment;
    DWORD size_of_image = nt_headers->OptionalHeader.SizeOfImage;
    DWORD new_size = ((size_of_image + payload_size) / file_alignment + 1) * file_alignment;
    printf("FDinside: %d\n", fd);
    int image_size_offset = sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + OPTIONAL_HEADER_SIZE_OF_IMAGE_OFFSET;
    lseek(fd, image_size_offset, SEEK_CUR); // move from current position instead
    // lseek(fd, image_size_offset, SEEK_SET);
    printf("new size of image: %x\n", new_size);
    int is_ok = write(fd, &new_size, sizeof(DWORD));
    if (is_ok == -1) {
        perror("Failed to write size of image");
    }
    return is_ok;
}

int modify_number_of_sections(int fd, int new_num_of_sections) {
    
        // ERR: need to get SectionAlignment
    printf("FDinside: %d\n", fd);
    int num_of_sections_offset = sizeof(DWORD) + sizeof(WORD);
    printf("offset: %d %d\n", num_of_sections_offset, new_num_of_sections);
    // lseek(fd, num_of_sections_offset, SEEK_SET);
    lseek(fd, num_of_sections_offset, SEEK_CUR);
    WORD val_to_write = (WORD) new_num_of_sections;
    // does the pointer to the int work??
    // i feel like it shouldnt because the argument is in a register, not a buffer
    int is_ok = write(fd, &val_to_write, sizeof(WORD));
    if (is_ok == -1) {
        perror("Failed to write number of sections offset");
    }
    return is_ok;
}


