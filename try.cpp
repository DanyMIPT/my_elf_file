#include <iostream>
#include "stdio.h"
#include <cstring>
#include "map.h"
#include "myLib.h"
#include "parts.h"

void make_executable_code ();
void write_file           ();

int main ()
{
    code = new char[max_size];

    Make_elf_header (section_header_begin);
    Make_program_header ();
    Make_section_header (0x00,0x18);
    make_data ();
    make_executable_code ();
    make_align (SECTION_HEADER_CONST::text_align);
    write_file ();

    delete [] code;
    return 0;
}

void make_executable_code ()
{
    unsigned char* binary_code = BufferMaker ("binaryCode.bin");
    size_t file_size = Sizecount ("binaryCode.bin");
    int* label_binary_code = (int*) calloc (file_size + 8, sizeof (int));

    int old_size = code_size;
    Make_executable_code (binary_code, file_size, label_binary_code);

    set_text_size (code_size - old_size);

    code_size = old_size;
    Make_executable_code (binary_code, file_size, label_binary_code);
}

void write_file ()
{
    FILE* out = fopen ("try_my_elf", "wb");

    fwrite (code, sizeof (char), code_size, out);

    fclose (out);
}

