
#include "stdio.h"
#include <cstring>
#include "parts.h"

int main ()
{
    code = new char[0x348];


    Make_elf_header (0xE0);
    Make_program_header (0xD2);

    auto a = new unsigned char[0x380];

    FILE* in = fopen ("kom", "rb");
    fread (a, sizeof (char), 0x380, in);
    fclose (in);

    Make_executable_code (&a[0xB0], 0xE0 - 0xB0);
    FILE* out = fopen ("try_my_elf", "wb");

    fwrite (code, sizeof (char), code_size, out);
    //fwrite (&a[0xB0], sizeof (char),0xE0 - 0xB0 , out);
    fwrite (&a[0x200 + 0x40], sizeof (char),0x380 - 0x200 - 0x40*4 , out);

    fclose (out);
    delete [] a;
    delete [] code;
    return 0;
}
