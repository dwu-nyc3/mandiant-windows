# Task
Part 1
Write a program in C using only the Windows API and C standard libraries to parse some of the headers of a 32-bit PE file. You can optionally parse 64-bit PE files. You will want to make it modular so you can reuse the parsing code for the next stages of the exercise. The program should print the contents of the following structures:


DOS header

NT header

Section headers


Wherever possible, translate the field values into meaningful terms for humans. Print both the offset and RVA values where appropriate (translate from one to the other depending which is present in the structure).


References:


PE spec

PE format diagram

Windows header files such as winnt.h under C:\Program Files (x86)\Windows Kits\10\Include\ (just #include <Windows.h>)



Part 2
This is really two separate exercises. Complete part 1 solidly before attempting part 2. Don't assume that any file has either imports or exports, you should detect and handle both.


Extend your program to print the names of all imported DLLs and the functions imported from each. Print the name or ordinal of each import depending which is used. You can use either the ILT or IAT, but understand the difference. You can ignore delay-load imports.


If present, print the exported functions. Print at least their ordinal, and their name if present. Include the RVA of their code. You can choose to print the names of forwarded exports, or ignore forwarded exports for the purpose of printing, but you should identify these so as not to print bad information.


Compare the output from several test files of varying complexity with a known-good PE parser like CFF Explorer.

# Thoughts
using the following compiler for windows.h  
x86_64-w64-mingw32-g++ 

DOS header is 

https://offwhitesecurity.dev/malware-development/portable-executable-pe/dos-header/#:~:text=e_magic%20is%20the%20'magic%20number,the%20file%20as%20DOS%20executable.
    <-- useful link 
https://stackoverflow.com/questions/28911962/portable-executable-dos-header-length
    <-- useful link for DOS header

# Questions
- What are RVA values?
    -> Relative Virtual Address
- are there any test cases for this?
- Offset of what? 
- For the DOS header:
    -> Use Windows API struct
    -> iterate through each field and print the value
    -> alternative: rewrite the struct in the file
        <- don't think this should be the case, should just cast instead

- Does the size of NTHeader expand based on the number of Data Directories?

91 -> 119

x86_64-w64-mingw32-g++ 


https://dev.to/mcaci/the-meaning-of-the-phony-target-inside-a-makefile-4fj8#:~:text=Inside%20the%20Makefile%20of%20a,a%20target%20for%20the%20Makefile.&text=build%3A%20go%20build-,.,represent%20files%20of%20the%20project.
<-- good explanation of how phony targets work in Makefiles

okay, how is the 32 bit version different from 64 bit version?
can i build a file as 32 bit?
im confused, i think the alerts stopped after i made the file name long??

What is a good testing mechanism?

64 / 32 bit is compiler specific so one test directory with C files that I have written, just compiled differently
    <-- add this to makefile probably

the code should be 32/64 bit agnostic so i need to double check everywhere that it is
i dont think the print is 
also, let me see the difference in vmware 

You seem to only be handling 64-bit files. The idea was to do 32-bit and then optionally 64-bit, but it's your training so up to you. However, you should check that the file matches what you expect. First of all, check that it's even a PE file. If so, make sure it actually contains an Optional Header. Check its bitness. We encounter both sizes frequently. 
    <-- 

Some of the fields are file offsets and some are RVAs. The instructions said to print both. At the least, do this for those that are RVAs, or at the very least some of those that are important, like AddressOfEntryPoint. You will need to be able to translate between offset and RVA in both directions. 
    <-- a little bit confused about this
    <-- how do we determine which is an RVA and which is just a file offset?
    <-- 

There are some interesting values in IMAGE_FILE_HEADER.Characteristics, IMAGE_OPTIONAL_HEADER.Subsystem, and IMAGE_OPTIONAL_HEADER.DllCharacteristics. You might want to parse those.
You should test it on more than one file. Compare the output with CFF Explorer or PE-Bear or the like.
    <--

How to get imports / exports from an executable?

will need IMAGE_IMPORT_DESCRIPTOR object i think 

okay, i get the IMAGE_DATA_DIRECTORY, and then what?

Extend your program to print the names of all imported DLLs and the functions imported from each. Print the name or ordinal of each import depending which is used. You can use either the ILT or IAT, but understand the difference. You can ignore delay-load imports.

does the code need to be loaded into memory or can i just read the contents
okay, so we go into the first section header for some reason 
okay, i think the goal is the find the .idata section
but according to this stack overflow post, it is not always the case that this is in the .idata section
https://stackoverflow.com/questions/22651433/pe-idata-section
seems like the standard (naive?) way to get the .idata section is to iterate through each section
might be better to read the entire thing into memory first?
what is the difference between PointerToRawData and VirtualAddress?
    <- VirtualAddress is the VA of this section
    <- PointerToRawData is the contents / backing store of this information
ok, steps are to get pointertorawdata, and then get the sizeofrawdata
read that and then iterate until failure
it says 0 for terminating null descriptor importer at the union 
okay, now im confused about what image_data_directory->size is used for 
next goal is to print the names of everything 
yea should stop lseeking around and just read everything into memory
not sure what the best way to do this is because we shouldnt read the entire file into memory, so maybe we read only the section headers? not sure, or at least read all of idata section, i.e. size of raw data
maybe read all of one section into memory like just the idata section

part 3:
Extend your program to insert a new section in a PE file containing data provided from another file. Provide an option to set the memory permissions of the new section and the ability to set the entry point of the PE file to code in that section. Test the new file with both inserted code and data of arbitrary size. Insert some code that performs some visible action to confirm it ran, then calls the original entry point of the executable to allow normal function.
    <- looks like an injection i guess
okay, how to insert new section into PE file?
there has to be a bunch of zero's somewhere that i can overwrite?
https://www.sunshine2k.de/reversing/tuts/tut_addsec.htm
okay, seems like we need to add something to the structures in the windows exectuables
can i / should i just use a magic constant? i feel like it would make more sense? cuz the value is always jsut at a static offset right?

okay the steps / info i need:
1) how big is the size of the new section?
2) what do i need to modify in the rest of the binary?
    <- offset b6(?), make 0300 -> 0400 i.e. 3 sections -> 4 sections
    <- section alignment and then increase image size at offset 0x50 
        <- alignment is a value we get from the OptionalHeader
    where do i put the new section of data?
3) code
    <- need to write to new file in new directory, and also modify the Makefile 
    <- im thinking we use write and lseek to modify very specific parts of the binary
4) constructing section header:
    what to fill for each value in the field?
    
    a. virtualsize or physicaladdress?
                        <-- should be equal to size of payload, and aligned
    b. virtualaddress    <-- derived from previous section headers
                        <-- algorithm is to take the largest virtual address, add the raw size, and align the values to size of section header, which in the example case is 0x1000
    c. sizeofrawdata     <-- how much stuff there is in our payload 
                        <-- i think this should be just the size of payload w/o alignment
    d. pointertorawdata  <-- where that stuff is located in the file
                        <--end of file i think
    e. pointertorelocations <-- ???
    f. pointertolinenumbers <-- ???
    g. numberofrelocations <-- ???
    h. numberoflinenumber <-- ???
    i. characteristics <-- permissions

    i think e->h can just be 0
    virtualaddress = largest virtualaddress of all the sections + the corresponding size of that section
    i think vsize and psize should be the same? (virtual vs physical)


I wonder if it would be a better design pattern if I used structs instead 


okay what is the sequence of steps i need to take to test part-3?
1) call add_new_section
2) modify_number_of_sections
3) modify_size_of_image


okay, there is a logic problem 
okay, we fixed the logic problem i think 

another design question is whether or not it's better to have each individual function open a file descriptor and then close it at the end of the function, or if it's better to pass a file descriptor back and forth
calculating the sizeofimage value is more nuanced than finding size of image and then just adding the payload size
we need to think about: 1) alignment of bytes 2) sizeofimage
does this mean that we also need to modify other parts of memory as well like virtualsize? no i dont think so, that is part of the section itself?
so just need to do rounded(size) + imagesize = newimagesize
not sure about the section stuff anymore, i need to double check that and make sure the alignment is also correct i think 

# TODO                                                                                                                              
- [x] DOS Header
- [x] NT Header
- [x] Section headers
- [ ] Format more nicely + add comments
- [ ] Do 32-bit version of the exercise as well 
- [x] Move printing stuff to a different .c/ .h file 
- [x] Add a makefile
    --> https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
- [x] Create an object directory to force the Makefile generation to make a bit more sense
- [ ] Modify the Makefile so that it reflects best practice
- [ ] Probably modify the makefile to use vscode compiler instead of mingw compiler and see if it still works as well
- [ ] Add test files
    - [ ] How to run tests in C projects effectively?
    - [ ] For the tests that I write, create separate directory for those files, add logic to Makefile, and build for both 32 and 64
    - [ ] Make test in the Makefile
- [ ] Add include directory for the part-1.c file
- [ ] How to build out test cases for this?
- [x] Check the file is a PE file naive
- [ ] Check file is PE file with more steps
- [ ] Reduce the redundancy from the include statements
- [ ] Fix code so that instead of using malloc, we use HeapAlloc() maybe?
- [x] Set up github
- [ ] Fix Makefile so that it's less redundant
- [ ] Part 3
    - [x] Modify section size
    - [x] Modify image size
    - [ ] Add new section
    - [ ] Generate some shellcode that spin locks and try to jump to it 
- [ ] Redo the code tomorrow; this is taking up too much bandwidth, good time to go through a redesign now 

# Questions
1) What are delay-load imports?
2) What is the difference between ILT and IAT?
    ->IAT IS import address table. Entries of the IAT are overwritten with the addresses of the functions being imported. 
    ->ILT is import lookup table
        <- Are these two things at the same addresses i.e. is this a union?
        <- in my code, am i using the IAT or the ILT? i am not sure
3) What is FirstThunk vs OriginalFirstThunk?
    OriginalFirstThunk is related to RVA's. Additionally, the OriginalFirstThunk is related to the way that DLL's are loaded into memory. If the imported DLL is not loaded at its preferred address, then the loader needs to examine the OriginalFirstThunk. Also, RVA seems to represent the offset of a function, for example, from the base, which is the VA
4) it seems like there could be the following case: 
    section A contains the import table addr
    section A's VA is not the same as the import_directory[IMAGE_IMPORT_DESCRIPTOR]'s VA
    When does this happen?
    Important because the way I find the import table is by doing an exact comparison
5) What is an ordinal? 
    An ordinal is an index into the export table
6) 

# Resources
- https://stackoverflow.com/questions/7673754/pe-format-iat-questions
    <- link to the file format in question 
- https://stackoverflow.com/questions/5631317/import-table-it-vs-import-address-table-iat
    <- something related to this?
- https://stackoverflow.com/questions/42413937/why-pe-need-original-first-thunkoft
- https://wambui-ngige.medium.com/dynamic-analysis-of-windows-exploit-mitigations-import-address-filtering-16fc28029529
    <- looks useful for explaining the import directory
- adding section to pe header
    <- https://www.sunshine2k.de/reversing/tuts/tut_addsec.htm
    <- https://secanablog.wordpress.com/2020/06/09/how-to-add-a-section-to-a-pe-file/
    <- useful for part 3 on how to add section to PE file
- Why aren't pe size and file size on disk equal?
    <- https://reverseengineering.stackexchange.com/questions/12121/how-can-file-size-and-pe-size-cant-be-equal

# Second Redesign:
1) handle both 32 and 64 bit
2) read the entire thing into memory
3) make the Makefile more streamlined
4) use fp instead of fd consistently
5) answer the question: which windows chunks do i need?
    - IMAGE_NT_HEADERS is def reused
    - IMAGE_SECTION_HEADER array is def used
6) i think the optional header should be a global next time we do this so we grab the data immediately
















