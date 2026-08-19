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
    int read_amnt;
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

    if ((read_amnt = fread(nt_header, sizeof(IMAGE_NT_HEADERS), 1, fp)) == 0) {
        strerror(errno);
        exit(1);
    }
    printf("Read amount%d %d\n", read_amnt, sizeof(IMAGE_NT_HEADERS));
    return nt_header;
}

IMAGE_SECTION_HEADER* get_section_arr(FILE* fp, unsigned int num_sections) {
    int magic_offset = g_dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS); 
    if (fseek(fp, magic_offset, SEEK_SET)) {
        strerror(errno);
        exit(1);
    };

    unsigned int mem_size = sizeof(IMAGE_SECTION_HEADER) * num_sections;
    IMAGE_SECTION_HEADER* image_section_header_arr = (IMAGE_SECTION_HEADER*) malloc(mem_size);
    printf("%u\n", mem_size);
    printf("Number of sections: %u\n", num_sections);
    fread(image_section_header_arr, mem_size, 1, fp);
    return image_section_header_arr;

}

/* Return the raw pointer
    * Need to do a NULL check upon returning 
    */
IMAGE_SECTION_HEADER* find_section(unsigned int findme) {
    IMAGE_SECTION_HEADER* ret;
    for (int i = 0; i < g_nt_headers->FileHeader.NumberOfSections; i++) {
        printf("%x\n", g_section_arr[i].VirtualAddress);
        if (findme >= g_section_arr[i].VirtualAddress && findme < (g_section_arr[i].VirtualAddress + g_section_arr[i].Misc.VirtualSize)) {
            ret = &g_section_arr[i];
            return ret;
        }
    }
    return ret; // TODO: need to handle this error 
}



// okay, need to modify this so that it is reading in the section so that i can use it 
// need image entry point 
// need a search fn 
// need image descriptors
//
void get_dlls(FILE* fp, IMAGE_NT_HEADERS* nt_hdr) {
    IMAGE_DATA_DIRECTORY import_dir = nt_hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    unsigned int id_va = import_dir.VirtualAddress;

    printf("VA of import: %x\n", import_dir.VirtualAddress);
    printf("Sz of import: %x\n", import_dir.Size);

    IMAGE_IMPORT_DESCRIPTOR* cur = (IMAGE_IMPORT_DESCRIPTOR*)malloc(sizeof(IMAGE_IMPORT_DESCRIPTOR));
    IMAGE_SECTION_HEADER* imports = find_section(id_va);
    printf("Pointer to raw data: %x\n", imports->PointerToRawData);
    printf("Size of raw data: %x\n", imports->SizeOfRawData);

    if (fseek(fp, imports->PointerToRawData, SEEK_SET)) {
        strerror(errno);
        exit(1);
    }

    IMAGE_IMPORT_DESCRIPTOR* descriptors = (IMAGE_IMPORT_DESCRIPTOR*) malloc(imports->SizeOfRawData);

    fread(descriptors, imports->SizeOfRawData, 1, fp);
    char* cp = (char*)descriptors;
    unsigned int oft, diff, val;

    // iterate through the image_import_descriptors
    for (int i = 0;;i++) {
        if (descriptors[i].Name == 0x0) {
            break;
        }
        printf("Descriptor Name: %x\n", descriptors[i].Name);
        printf("DLL Name: %s\n", cp + (descriptors[i].Name - id_va));
        printf("Thunk: %x\n", descriptors[i].OriginalFirstThunk);
        oft = descriptors[i].OriginalFirstThunk;
        diff = oft - id_va;

        for (int i = 0;; i+=8) {
            val = *(int*)(cp + diff + i); // i think add 8 to this to get next ptr
            if (val == 0x0) {break;}
            int* ptr = cp + (val - id_va + 2);
            printf("Ptr str: %s\n", ptr);
        }
        // printf("%x\n", val);
        // int* ptr = cp + (val - id_va);
        // for (int i = 0; ; i++) {
        //     if (*ptr == 0x0) {
        //         break;
        //     }
        //     printf("Ptr str: %s\n", ptr);
        //     ptr += 1; 
        // }
    }
    // from this same section that we read, grab the function pointers using thunks 

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
    get_dlls(fp, g_nt_headers);

}

