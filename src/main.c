#include "part-1.h"
#include "part-2.h"
#include "printer.h"

int main(int argc, char** argv) {
    printf("%s\n", argv[0]);
    if (argv[1] == NULL) {
        printf("No file provided\n");        
        return -1;
    }
    printf("%s\n", argv[1]);

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("File not found");
        return -1;
    }

    // TODO: Check if file is a Windows file
    // how do this besides looking at just MZ / PE magic?
    // b/c that can be faked 
    // maybe we can just be naiive for now
    // we might also cause a crash if we do get_dos_header on input which is too small
    
    IMAGE_DOS_HEADER* dos_header = get_dos_header(fd);
    
    // move fd pointer to the start of NT_HEADER
    lseek(fd, dos_header->e_lfanew, SEEK_SET);
    IMAGE_NT_HEADERS* nt_headers = get_nt_headers(fd);
    if ((dos_header->e_magic != 0x5a4d) || (nt_headers->Signature != 0x4550)) {
        printf("Not a windows file\n"); 
        return -1;
    }

    print_dos_header(dos_header);
    print_nt_header(nt_headers);
    int num_sections = nt_headers->FileHeader.NumberOfSections;
    lseek(fd, dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS), SEEK_SET);
    IMAGE_SECTION_HEADER** image_section_arr = get_section_arr(fd, num_sections);
    


    // Get the section containing the import table
    IMAGE_SECTION_HEADER* import_table_section = get_section_with_import_table(nt_headers, image_section_arr, num_sections);
    // printf("%d\n", import_table_section->VirtualAddress);
    printf("%p\n", nt_headers);
    printf("done!\n");
    printf("ret addr: %p\n", import_table_section);
    printf("%x\n", import_table_section->VirtualAddress);
    printf("%x\n", import_table_section->PointerToRawData);
    printf("%x\n", import_table_section->SizeOfRawData);
    printf("image import descriptor size: %x\n", sizeof(IMAGE_IMPORT_DESCRIPTOR));

    lseek(fd, import_table_section->PointerToRawData, SEEK_SET);
    char* import_section = (char*) malloc(import_table_section->SizeOfRawData);
    read(fd, import_section, import_table_section->SizeOfRawData);
    IMAGE_IMPORT_DESCRIPTOR* cur_descriptor;
    puts("Done reading!");
    
    printf("%p\n", import_section);

    
    lseek(fd, 0, SEEK_SET);

    int va_diff;
    int name_loc;
    char* import_name;
    char* import_base = import_section;
    for (; ((IMAGE_IMPORT_DESCRIPTOR*)import_section)->OriginalFirstThunk != 0; import_section += sizeof(IMAGE_IMPORT_DESCRIPTOR)) {
        cur_descriptor = (IMAGE_IMPORT_DESCRIPTOR*)import_section;
        printf("%x\n", cur_descriptor->Name);
        va_diff = cur_descriptor->Name - import_table_section->VirtualAddress;
        printf("%d\n", va_diff);
        import_name = import_base + va_diff;
        printf("%s\n", import_name);
    }

    // printf("oft: %x\n", import_section[0]->OriginalFirstThunk);
    // for (int i = 0; import_section[i]->OriginalFirstThunk != 0; i++) {
    //     printf("i: %d\n", i);
    //     cur_descriptor = import_section[i];
    //     printf("oft: %x\n", cur_descriptor->OriginalFirstThunk);
    //     break;
    // }
   
    // Cleanup
    // free(dos_header);
    // free(nt_headers);

    return 0;
}


