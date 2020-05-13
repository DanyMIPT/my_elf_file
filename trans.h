#include "myLib.h"

void trans                (const char* in, const char* out);
void make_executable_code (const char* program);
void write_file           (const char* program);

void trans (const char* in, const char* out)
{
    Make_elf_header (section_header_begin);
    Make_program_header ();
    Make_section_header ();
    make_data ();

    make_executable_code (in);

    make_align (SECTION_HEADER_CONST::text_align);
    write_file (out);
}

void make_executable_code (const char* in)
{
    unsigned char* binary_code = BufferMaker (in);
    size_t file_size = Sizecount (in);

    int* label_binary_code = (int*) calloc (file_size + 8, sizeof (int));

    int old_size = code_size;
    Make_executable_code (binary_code, file_size, label_binary_code);

    set_text_size (code_size - old_size);

    code_size = old_size;
    Make_executable_code (binary_code, file_size, label_binary_code);

    free (binary_code);
    free (label_binary_code);
}

void write_file (const char* out_file)
{
    FILE* out = fopen (out_file, "wb");

    fwrite (code, sizeof (char), code_size, out);

    fclose (out);
}
