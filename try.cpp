#include <iostream>
#include "stdio.h"
#include <cstring>

//#define WINDOWS
#ifdef WINDOWS

    typedef unsigned long       QWORD;
    typedef unsigned int        DWORD;
    typedef unsigned char       BYTE;
    typedef unsigned short      WORD;

#else

    typedef unsigned long       QWORD;
    typedef unsigned int        DWORD;
    typedef unsigned char       BYTE;
    typedef unsigned short      WORD;

#endif

#include "map.h"
#include "myLib.h"
#include "parts.h"



int main ()
{
    code = new char[0x348];


    Make_elf_header (0xB0);
    Make_program_header (0x00);

    Make_section_header (0x138, 0x00, 0x130, 0x8);


    unsigned char* binary_code = BufferMaker ("binaryCode.bin");
    size_t file_size = Sizecount ("binaryCode.bin");
    int* label_binary_code = (int*) calloc (file_size + 8, sizeof (int));

    for (int i = 0; i < 8; i++)
        code[code_size++] = 0x0;

    std::cout << code_size;

    int old_size = code_size;



    Make_executable_code (binary_code, file_size, label_binary_code);


    size_t text_size = code_size - old_size;
    *(QWORD*) &code[0xd0] = text_size;
    *(QWORD*) &code[0x60] = text_size;
    *(QWORD*) &code[0x68] = text_size;

    code_size = old_size;
    
    Make_executable_code (binary_code, file_size, label_binary_code);

    for (int i = 0; i < 0x20; i++)
        code[code_size++] = 0x0;

    FILE* out = fopen ("try_my_elf", "wb");

    fwrite (code, sizeof (char), code_size, out);

    fclose (out);
    delete [] code;
    return 0;
}

