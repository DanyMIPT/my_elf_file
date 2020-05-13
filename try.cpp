#include <iostream>
#include "stdio.h"
#include <cstring>
#include "const_lib.h"
#include "parts.h"
#include <unistd.h>
#include "trans.h"


size_t choose ();

constexpr char* read_pr[]  = {"binaryCode.bin", "sqr_bin.bin"    };
constexpr char* write_pr[] = {"factorial",      "square_equation"};

int main ()
{
    code = new char[max_size];


    int program = choose ();
    trans (read_pr[program - 1], write_pr[program - 1]);

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



