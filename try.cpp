#include <iostream>
#include "stdio.h"
#include <cstring>
#include "map.h"
#include "myLib.h"
#include "parts.h"
#include <unistd.h>

void   make_executable_code (int program);
void   write_file           (int program);
size_t choose               ();

const char* read_pr[]  = {"binaryCode.bin", "sqr_bin.bin"    };
const char* write_pr[] = {"factorial",      "square_equation"};

int main ()
{
    code = new char[max_size];

    Make_elf_header (section_header_begin);
    Make_program_header ();
    Make_section_header (0x00,0x18);
    make_data ();

    int program = choose ();
    make_executable_code (program);

    make_align (SECTION_HEADER_CONST::text_align);
    write_file (program);

    execl (write_pr[program - 1], NULL);

    delete [] code;
    return 0;
}

size_t choose ()
{
    std::cout << "Push 1 - factorial, Push 2 - square_equation:\n";
    
    int t = 0;
    std::cin >> t;

    switch (t)
    {
        case 1: { }
        case 2:
        {
            return t;
        }

        default:
        {
            choose ();
        } 
    }

}

void make_executable_code (int digit)
{
    unsigned char* binary_code = BufferMaker (read_pr[digit - 1]);
    size_t file_size = Sizecount (read_pr[digit - 1]);
    int* label_binary_code = (int*) calloc (file_size + 8, sizeof (int));

    int old_size = code_size;
    Make_executable_code (binary_code, file_size, label_binary_code);

    set_text_size (code_size - old_size);

    code_size = old_size;
    Make_executable_code (binary_code, file_size, label_binary_code);

    free (binary_code);
    free (label_binary_code);
}

void write_file (int program)
{
    FILE* out = fopen (write_pr[program - 1], "wb");

    fwrite (code, sizeof (char), code_size, out);

    fclose (out);
}

