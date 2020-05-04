#include <iostream>
#include "stdio.h"
#include <cstring>
#include "map.h"
#include "myLib.h"
#include "parts_new.h"


int main ()
{
    code = new char[0x348];


    Make_elf_header (0xB0);
    Make_program_header (0xD2);

    auto a = new unsigned char[0x380];

    FILE* in = fopen ("kom", "rb");
    fread (a, sizeof (char), 0x380, in);
    fclose (in);

    Make_section_header (0x134, 0x22, 0x130, 0x4);
    //Make_executable_code (&a[0xB0], 0xF0 - 0xB0);
    memcpy (&code[code_size], &a[0xD4], 4);
    memcpy (&code[code_size + 4], &a[0xB0], 0xD4 - 0xB0);
        code_size += 0xD4 - 0xB0 + 4;
    for (int i = code_size; i < 0x170; i++)
        code[code_size++] = 0;
    //code_size += 0xF0 - 0xB0;
    *(DWORD*) &code[0x13b + 0x4] = 0x600130;
    *(DWORD*) &code[0x80]  += 0x130;
    *(DWORD*) &code[0x88]  += 0x130;
    *(DWORD*) &code[0x90]  += 0x130;
    *(DWORD*) &code[0x100] += 0x130;
    *(DWORD*) &code[0x108] += 0x130;

    FILE* out = fopen ("try_my_elf", "wb");

    fwrite (code, sizeof (char), code_size, out);

    Make_executable_code ();

    fclose (out);
    delete [] a;
    delete [] code;
    return 0;
}

