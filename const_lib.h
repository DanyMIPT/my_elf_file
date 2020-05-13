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
    IN_     = 0X08,
    OUT_    = 0X09,
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
    MEM     = 3
};

enum registers
{
    ax  = 0, 
    bx  = 1, 
    cx  = 2, 
    dx  = 3,
 
    r8  = 0, 
    r9  = 1, 
    r10 = 2, 
    r11 = 3, 
    r12 = 4, 
    r13 = 5, 
    r14 = 6,
};

constexpr BYTE reg_compare[] = {0, 3, 1, 2};

namespace mov
{
    constexpr BYTE mov_a = 0xB8;
    constexpr BYTE mov_b = 0xBB;
    constexpr BYTE mov_c = 0xB9;
    constexpr BYTE mov_d = 0xBA;
}

namespace prefix
{
    constexpr BYTE REX_B    = 0x41;  //REX prefix, watch intel manual
    constexpr BYTE REX_W    = 0x48;
    constexpr BYTE REX_WB   = 0x49;
    constexpr BYTE REX_WR   = 0x4C;
    constexpr BYTE REX_WRB  = 0x4D;

    constexpr BYTE short_   = 0x0F;
    constexpr BYTE word     = 0x66;
    constexpr BYTE pref_80  = 0x80; 
    constexpr BYTE pref_c6  = 0xC6; 
}

namespace functions
{
    constexpr WORD int80h       = 0x80cd;

    constexpr BYTE mul_div      = 0xF7;
    constexpr BYTE sub          = 0x29;
    constexpr BYTE cmp_mem      = 0x80;
    constexpr BYTE cmp          = 0x39;
    constexpr BYTE call         = 0xE8;
    constexpr BYTE mov_reg      = 0x89;
    constexpr BYTE mov_dig      = 0xB8;
    constexpr BYTE mov_dig_mem  = 0x04;
    constexpr BYTE mov_dig_rm   = 0xC7;
    constexpr BYTE add_r        = 0x83;
    constexpr BYTE sub_al       = 0x2C;
    constexpr BYTE xor_reg      = 0x31;
    constexpr BYTE push_reg     = 0x50;
    constexpr BYTE push_mem     = 0xFF;
    constexpr BYTE pop_reg      = 0x58;
    constexpr BYTE cmp_al       = 0x3C;
    constexpr BYTE inc          = 0xFF;
    constexpr BYTE add_al       = 0x02;
    constexpr BYTE ret          = 0xC3;
    constexpr BYTE push_dword   = 0x68;
    constexpr BYTE add_rsi      = 0x06;
    constexpr BYTE cmp_r_digit  = 0x83;
    constexpr BYTE add_r_r      = 0x01;
    constexpr BYTE bsr          = 0xBD;
    constexpr BYTE and_cl       = 0xE1;
    constexpr BYTE shl          = 0xD3;
    constexpr BYTE shr          = 0xD1;
    constexpr BYTE shr_two      = 0xC1;
    constexpr BYTE cmp_digit    = 0x83;


    constexpr BYTE mov_mem_from_al    = 0x88;
    constexpr BYTE mov_mem_plus_digit = 0xBE;
}

namespace jumps
{
    constexpr BYTE jmp          = 0xE9;

    constexpr BYTE ja           = 0x8F;
    constexpr BYTE jae          = 0x8D;

    constexpr BYTE je           = 0x84;
    constexpr BYTE jne          = 0x85;

    constexpr BYTE jb           = 0x8C;
    constexpr BYTE jbe          = 0x8E;

    constexpr BYTE jne_one_byte = 0x75;
    constexpr BYTE jbe_one_byte = 0x76;
    constexpr BYTE ja_one_byte  = 0x77;
    constexpr BYTE jmp_one_byte = 0xEB;
    constexpr BYTE jge_one_byte = 0x7d;
}

namespace fun_distances
{
    constexpr BYTE not_minus    = 0x04;
    constexpr BYTE skip_mul     = 0x0A;
    constexpr BYTE repeat       = 0xE1;
    constexpr BYTE loop2        = 0xE9;
    constexpr BYTE jmp_shr      = 0x0B;
    constexpr BYTE next         = 0x03;
    constexpr BYTE refine       = 0xE1;
    constexpr BYTE skip_min     = 0x36;
}














namespace rm_byte
{
    constexpr BYTE rax_r14       = 0xC6;
    constexpr BYTE r14_rax       = 0xF0;
    constexpr BYTE mul_r9        = 0xE9;
    constexpr BYTE div_r9        = 0xF9;
    constexpr BYTE mul_r10       = 0xEA;
    constexpr BYTE r9_r8         = 0xC1;
    constexpr BYTE inc_rcx       = 0xC1;
    constexpr BYTE dig_r9        = 0xC1;
    constexpr BYTE rax_r9        = 0xC1;
    constexpr BYTE sib_follow_sm = 0x2C;
    constexpr BYTE sib_follow_p  = 0x34;
    constexpr BYTE r12_r11       = 0xDC;
    constexpr BYTE rax_rax       = 0xC0;
    constexpr BYTE r10_r10       = 0xD2;
    constexpr BYTE rip_plus      = 0x25;
    constexpr BYTE dig_r10       = 0xC2;
    constexpr BYTE mem_plus_r10  = 0x82;   
    constexpr BYTE cmp_mem_rm    = 0xBA;
    constexpr BYTE r13_rax       = 0xE8;
    constexpr BYTE rcx_rcx       = 0xC9;
    constexpr BYTE rdx_rdx       = 0xD2;
    constexpr BYTE dl_rsi        = 0x16;
    constexpr BYTE dec_rsi       = 0xCE;
    constexpr BYTE rax_digit     = 0xF8;   
    constexpr BYTE rcx_rdx       = 0xCA;
    constexpr BYTE rsi_rcx       = 0xF1;
    constexpr BYTE mul_r13       = 0xFD;
    constexpr BYTE imul_r13      = 0xED;
    constexpr BYTE idiv_r10      = 0xFA;
    constexpr BYTE r9_rax        = 0xC8;
    constexpr BYTE rax_rcx       = 0xC8;
    constexpr BYTE rbx_rbx       = 0xDB;
    constexpr BYTE cl_rdx        = 0xE2;
    constexpr BYTE rbx_r8        = 0xD8;
    constexpr BYTE rdx_r8        = 0xD0;
    constexpr BYTE rax_r8        = 0xC0;     
    constexpr BYTE r8_rax        = 0xC0;
    constexpr BYTE rdx_rbx       = 0xD3;
    constexpr BYTE shr_rbx       = 0xEB;
    constexpr BYTE shr_rdx_digit = 0xEA;
    constexpr BYTE rbx_r9        = 0xD9;
    constexpr BYTE dig_r13       = 0xC5;
    constexpr BYTE rax_r13       = 0xC5;
    constexpr BYTE dig_mem       = 0x25;

}

namespace sib_byte
{
    constexpr BYTE subq_my_sib  = 0x25;
}
