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
    IN_      = 0X08,
    OUT_     = 0X09,
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
    ax = 0, bx = 1, cx = 2, dx = 3, r8 = 0, r9 = 1, r10 = 2, r11 =3, r12 = 4, r13 = 5, r14 = 6,
};

const BYTE reg_compare[] = {0, 3, 1, 2};

namespace mov
{
    const BYTE mov_a = 0xB8;
    const BYTE mov_b = 0xBB;
    const BYTE mov_c = 0xB9;
    const BYTE mov_d = 0xBA;
}

namespace prefix
{
    const BYTE REX_B    = 0x41;  //REX prefix, watch intel manual
    const BYTE REX_W    = 0x48;
    const BYTE REX_WB   = 0x49;
    const BYTE REX_WR   = 0x4C;
    const BYTE REX_WRB  = 0x4D;

    const BYTE short_   = 0x0F;
    const BYTE word     = 0x66;
    const BYTE pref_80  = 0x80; 
}

namespace functions
{
    const WORD int80h       = 0x80cd;

    const BYTE mul_div      = 0xF7;
    const BYTE sub          = 0x29;
    const BYTE cmp_mem      = 0x80;
    const BYTE cmp          = 0x39;
    const BYTE call         = 0xE8;
    const BYTE mov_reg      = 0x89;
    const BYTE mov_dig      = 0xB8;
    const BYTE mov_dig_rm   = 0xC7;
    const BYTE add_r        = 0x83;
    const BYTE sub_al       = 0x2C;
    const BYTE xor_reg      = 0x31;
    const BYTE push_reg     = 0x50;
    const BYTE push_mem     = 0xFF;
    const BYTE pop_reg      = 0x58;
    const BYTE cmp_al       = 0x3C;
    const BYTE inc          = 0xFF;
    const BYTE add_al       = 0x02;
    const BYTE ret          = 0xC3;
    const BYTE push_byte    = 0x6A;
    const BYTE add_rsi      = 0x06;
    const BYTE cmp_r_digit  = 0x83;


    const BYTE mov_mem_from_al    = 0x88;
    const BYTE mov_mem_plus_digit = 0xBE;
}

namespace jumps
{
    const BYTE jmp          = 0xE9;
    const BYTE ja           = 0x87;
    const BYTE je           = 0x84;
    const BYTE jne_one_byte = 0x75;
}

namespace fun_distances
{
    const BYTE not_minus    = 0x04;
    const BYTE skip_mul     = 0x0A;
    const BYTE repeat       = 0xE1;
    const BYTE loop2        = 0xE9;
}

namespace rm_byte
{
    const BYTE rax_r14       = 0xC6;
    const BYTE r14_rax       = 0xF0;
    const BYTE mul_r9        = 0xE9;
    const BYTE div_r9        = 0xF9;
    const BYTE mul_r10       = 0xEA;
    const BYTE r9_r8         = 0xC1;
    const BYTE inc_rcx       = 0xC1;
    const BYTE dig_r9        = 0xC1;
    const BYTE rax_r9        = 0xC1;
    const BYTE sib_follow_sm = 0x2C;
    const BYTE sib_follow_p  = 0x34;
    const BYTE r12_r11       = 0xDC;
    const BYTE rax_rax       = 0xC0;
    const BYTE r10_r10       = 0xD2;
    const BYTE rip_plus      = 0x25;
    const BYTE dig_r10       = 0xC2;
    const BYTE mem_plus_r10  = 0x82;   
    const BYTE cmp_mem_rm    = 0xBA;
    const BYTE r13_rax       = 0xe8;
    const BYTE rcx_rcx       = 0xC9;
    const BYTE rdx_rdx       = 0xD2;
    const BYTE dl_rsi        = 0x16;
    const BYTE dec_rsi       = 0xCE;
    const BYTE rax_digit     = 0xF8;   
    const BYTE rcx_rdx       = 0xCA;
    const BYTE rsi_rcx       = 0xF1;

}

namespace sib_byte
{
    const BYTE subq_my_sib  = 0x25;
}
