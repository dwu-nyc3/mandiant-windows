#include "printer.h"

/** Print the DOS header of an exe file
    *
    */
void print_dos_header(IMAGE_DOS_HEADER* dos_header) {
    printf("DOS Header\n");
    printf("e_magic: %04x\n", dos_header->e_magic);
    printf("e_cblp: %04x\n", dos_header->e_cblp);
    printf("e_cp: %04x\n", dos_header->e_cp);
    printf("e_crlc: %04x\n", dos_header->e_crlc);
    printf("e_cparhdr: %04x\n", dos_header->e_cparhdr);
    printf("e_minalloc: %04x\n", dos_header->e_minalloc);
    printf("e_maxalloc: %04x\n", dos_header->e_maxalloc);
    printf("e_ss: %04x\n", dos_header->e_ss);
    printf("e_sp: %04x\n", dos_header->e_sp);
    printf("e_csum: %04x\n", dos_header->e_csum);
    printf("e_ip: %04x\n", dos_header->e_ip);
    printf("e_cs: %04x\n", dos_header->e_cs);
    printf("e_lfarlc: %04x\n", dos_header->e_lfarlc);
    printf("e_ovno: %04x\n", dos_header->e_ovno);
    
    for (int i = 0; i < 4; i++) {
        printf("e_res[%d]: %04x\n", i, dos_header->e_res[i]);
    }
    
    printf("e_oemid: %04x\n", dos_header->e_oemid);
    printf("e_oeminfo: %04x\n", dos_header->e_oeminfo);
    
    for (int i = 0; i < 10; i++) {
        printf("e_res2[%d]: %04x\n", i, dos_header->e_res2[i]);
    }
    
    printf("e_lfanew: %08x\n", dos_header->e_lfanew);
}

/** Print the NT header of an exe file
    *
    */
void print_nt_header(IMAGE_NT_HEADERS* nt_header) {
    printf("NT Header\n");
    printf("Signature: %08x\n", nt_header->Signature);
    printf("FileHeader.Machine: %04x\n", nt_header->FileHeader.Machine); 
    printf("FileHeader.NumberOfSections: %08x\n", nt_header->FileHeader.NumberOfSections); 
    printf("FileHeader.TimeDateStamp: %08x\n", nt_header->FileHeader.TimeDateStamp); 
    printf("FileHeader.PointerToSymbolTable: %08x\n", nt_header->FileHeader.PointerToSymbolTable); 
    printf("FileHeader.NumberOfSymbols: %08x\n", nt_header->FileHeader.NumberOfSymbols); 
    printf("FileHeader.SizeOfOptionalHeader: %04x\n", nt_header->FileHeader.SizeOfOptionalHeader); 
    printf("FileHeader.Characteristics: %04x\n", nt_header->FileHeader.Characteristics); 
    
    printf("OptionalHeader.Magic: %04x\n", nt_header->OptionalHeader.Magic);
    printf("OptionalHeader.MajorLinkerVersion: %02x\n", nt_header->OptionalHeader.MajorLinkerVersion);
    printf("OptionalHeader.MinorLinkerVersion: %02x\n", nt_header->OptionalHeader.MinorLinkerVersion);
    printf("OptionalHeader.SizeOfCode: %08x\n", nt_header->OptionalHeader.SizeOfCode);
    printf("OptionalHeader.SizeOfInitializedData: %08x\n", nt_header->OptionalHeader.SizeOfInitializedData);
    printf("OptionalHeader.SizeOfUninitializedData: %08x\n", nt_header->OptionalHeader.SizeOfUninitializedData);
    printf("OptionalHeader.AddressOfEntryPoint: %08x\n", nt_header->OptionalHeader.AddressOfEntryPoint);
    printf("OptionalHeader.BaseOfCode: %08x\n", nt_header->OptionalHeader.BaseOfCode);
    printf("OptionalHeader.ImageBase: %016llx\n", nt_header->OptionalHeader.ImageBase);
    printf("OptionalHeader.SectionAlignment: %08x\n", nt_header->OptionalHeader.SectionAlignment);
    printf("OptionalHeader.FileAlignment: %08x\n", nt_header->OptionalHeader.FileAlignment);
    printf("OptionalHeader.MajorOperatingSystemVersion: %04x\n", nt_header->OptionalHeader.MajorOperatingSystemVersion);
    printf("OptionalHeader.MinorOperatingSystemVersion: %04x\n", nt_header->OptionalHeader.MinorOperatingSystemVersion);
    printf("OptionalHeader.MajorImageVersion: %04x\n", nt_header->OptionalHeader.MajorImageVersion);
    printf("OptionalHeader.MinorImageVersion: %04x\n", nt_header->OptionalHeader.MinorImageVersion);
    printf("OptionalHeader.MajorSubsystemVersion: %04x\n", nt_header->OptionalHeader.MajorSubsystemVersion);
    printf("OptionalHeader.MinorSubsystemVersion: %04x\n", nt_header->OptionalHeader.MinorSubsystemVersion);
    printf("OptionalHeader.Win32VersionValue: %08x\n", nt_header->OptionalHeader.Win32VersionValue);
    printf("OptionalHeader.SizeOfImage: %08x\n", nt_header->OptionalHeader.SizeOfImage);
    printf("OptionalHeader.SizeOfHeaders: %08x\n", nt_header->OptionalHeader.SizeOfHeaders);
    printf("OptionalHeader.CheckSum: %08x\n", nt_header->OptionalHeader.CheckSum);
    printf("OptionalHeader.Subsystem: %04x\n", nt_header->OptionalHeader.Subsystem);
    printf("OptionalHeader.DllCharacteristics: %04x\n", nt_header->OptionalHeader.DllCharacteristics);
    printf("OptionalHeader.SizeOfStackReserve: %016llx\n", nt_header->OptionalHeader.SizeOfStackReserve);
    printf("OptionalHeader.SizeOfStackCommit: %016llx\n", nt_header->OptionalHeader.SizeOfStackCommit);
    printf("OptionalHeader.SizeOfHeapReserve: %016llx\n", nt_header->OptionalHeader.SizeOfHeapReserve);
    printf("OptionalHeader.SizeOfHeapCommit: %016llx\n", nt_header->OptionalHeader.SizeOfHeapCommit);
    printf("OptionalHeader.LoaderFlags: %08x\n", nt_header->OptionalHeader.LoaderFlags);
    printf("OptionalHeader.NumberOfRvaAndSizes: %08x\n", nt_header->OptionalHeader.NumberOfRvaAndSizes);
    for (int i = 0; i < 16; i++) {
        printf("OptionHeader.DataDirectory virtual address at position %d: %08x\n", i, nt_header->OptionalHeader.DataDirectory[i].VirtualAddress);
        printf("OptionHeader.DataDirectory size at position %d: %08x\n", i, nt_header->OptionalHeader.DataDirectory[i].Size);
    }
}

/** Print information about the image section headers
    *
    *
    */
void print_section_arr(IMAGE_SECTION_HEADER** image_section_arr, int num_sections) {
    for (int i = 0; i < num_sections; i++) {
        puts("-----");
        printf("Index: %d\n", i);
        printf("PhysicalAddress (Misc.PhysicalAddress): %08x\n", image_section_arr[i]->Misc.PhysicalAddress);
        printf("VirtualSize (Misc.VirtualSize): %08x\n", image_section_arr[i]->Misc.VirtualSize);
        printf("VirtualAddress: %08x\n", image_section_arr[i]->VirtualAddress);
        printf("SizeOfRawData: %08x\n", image_section_arr[i]->SizeOfRawData);
        printf("PointerToRawData: %08x\n", image_section_arr[i]->PointerToRawData);
        printf("PointerToRelocations: %08x\n", image_section_arr[i]->PointerToRelocations);
        printf("PointerToLinenumbers: %08x\n", image_section_arr[i]->PointerToLinenumbers);
        printf("NumberOfRelocations: %04x\n", image_section_arr[i]->NumberOfRelocations);
        printf("NumberOfLinenumbers: %04x\n", image_section_arr[i]->NumberOfLinenumbers);
        printf("Characteristics: %08x\n", image_section_arr[i]->Characteristics);
        puts("-----");
    }
}

