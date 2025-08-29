#include "part-2.h"
// get import table


/**
    * @brief Get the section that contains information about the import table
    * @param nt_headers       copy of nt header values
    * @param section_arr      copy of section array 
    * @param num_sections     number of sections in section_arr
    *
    * TODO: Not sure if this can't be simplified or something
    */
IMAGE_SECTION_HEADER* get_section_with_import_table(IMAGE_NT_HEADERS* nt_headers, IMAGE_SECTION_HEADER** section_arr, int num_sections) {
    IMAGE_OPTIONAL_HEADER opt_header = nt_headers->OptionalHeader;
    IMAGE_DATA_DIRECTORY* data_dir = opt_header.DataDirectory;
    // iterate through IMAGE_IMPORT_DESCRIPTORs inside the data_dir
    // entry export is the 0th index
    // entry import is the 1st index
    DWORD exp_rva= data_dir[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    DWORD exp_sz= data_dir[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

    DWORD imp_rva= data_dir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    DWORD imp_sz= data_dir[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

    printf("import rva: %x\n", imp_rva);
    printf("import size: %x\n", imp_sz);

    // Go to start of the image section header
    // Iterate through each of the section headers
    // Find the .idata / .rdata section, i.e. find the section that contains the import tables i think 
    // printf("%x %x\n", sizeof(IMAGE_NT_HEADERS), (int)nt_headers);

    IMAGE_SECTION_HEADER* ret_val = (IMAGE_SECTION_HEADER*)malloc(sizeof(IMAGE_SECTION_HEADER));
    IMAGE_SECTION_HEADER* cur_section;
    for (int i = 0; i < num_sections; i++) {
        cur_section = section_arr[i];
        if (cur_section->VirtualAddress == imp_rva) {
            memcpy(ret_val, cur_section, sizeof(IMAGE_SECTION_HEADER));
            printf("%x\n", cur_section->VirtualAddress);
            printf("Found!\n");
            break;
        }
    }

    // printf("ret addr: %x\n", ret_val->VirtualAddress);
    
    printf("ret addr: %p\n", ret_val);

    return ret_val;
}




