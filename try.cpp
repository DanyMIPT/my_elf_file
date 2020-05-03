
#include "stdio.h"
#include <cstring>
#include "parts.h"

int main ()
{
    code = new char[0x348];


    Make_elf_header (0x40);
    Make_program_header (0xD2);

    auto a = new unsigned char[0x380];

    FILE* in = fopen ("kom", "rb");
    fread (a, sizeof (char), 0x380, in);
    fclose (in);

    Make_section_header (0x130, 0x22, 0x154, 0x1);
    Make_executable_code (&a[0xB0], 0xF0 - 0xB0);
    *(DWORD*) &code[0x13b] = 0x600154;
    *(DWORD*) &code[0x80]  += 0x154;
    *(DWORD*) &code[0x88]  += 0x154;
    *(DWORD*) &code[0x90]  += 0x154;
    *(DWORD*) &code[0x100] += 0x154;
    *(DWORD*) &code[0x108] += 0x154;

    FILE* out = fopen ("try_my_elf", "wb");

    fwrite (code, sizeof (char), code_size, out);

    fclose (out);
    delete [] a;
    delete [] code;
    return 0;
}

