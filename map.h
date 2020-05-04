#include "my_vec.h"

typedef unsigned long       QWORD;
typedef unsigned int        DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

enum commands
{
    END     = 0x00,
    PUSH    = 0x01,
    POP     = 0x02,
    ADD     = 0x03,
    SUB     = 0X04,
    MUL     = 0X05,
    DIV     = 0X06,
    SQRT    = 0X07,
    IN      = 0X08,
    OUT     = 0X09,
    JMP     = 0X0A,
    JA      = 0X0B,
    JAE     = 0X0C,
    JB      = 0X0D,
    JBE     = 0X0E,
    JE      = 0X0F,
    JNE     = 0X10,
    CALL    = 0X11,
    RET     = 0X12,
};

enum push
{
    INT_    = 0,
    REG     = 1,
    INT_REG = 2,
    MEM = 3
};

enum registers
{
    ax = 0, bx = 1, cx = 2, dx = 3
};

namespace mov
{
    const BYTE mov_a = 0xB8;
    const BYTE mov_b = 0xBB;
    const BYTE mov_c = 0xB9;
    const BYTE mov_d = 0xBA;
}

namespace functions
{
    const WORD int80h = 0x80cd;

    const BYTE mul    = 0xF7;
    const BYTE sub    = 0x29;
    const BYTE cmp    = 0x39;
    const BYTE jmp    = 0xEB;
    const BYTE call    = 0xEB;

    const BYTE push_a = 0x50;
    const BYTE pop_a  = 0x58;

    const BYTE push_b = 0x53;
    const BYTE pop_b  = 0x5b;

    const BYTE push_c = 0x51;
    const BYTE pop_c  = 0x59;

    const BYTE REX_B   = 0x41;  //REX prefix, watch intel manual
    const BYTE REX_WB  = 0x49;
    const BYTE REX_WRB = 0x4D;

    const BYTE pop_r8  = 0x58;
    const BYTE pop_r9  = 0x59;
    const BYTE push_r9 = 0x51;
    const BYTE pop_r10 = 0x5A;
    const BYTE pop_r11 = 0x5B;
    const BYTE pop_r12 = 0x5C;


    const BYTE push_byte = 0x6A;


}
