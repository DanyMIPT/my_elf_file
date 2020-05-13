

char*  code      = nullptr;
size_t code_size = 0;

constexpr size_t max_size             = 0x100000;
constexpr size_t section_header_begin = 0xB0;

void pass_push     (const unsigned char* binary_code, size_t& i);
void pass_pop      (const unsigned char* binary_code, size_t& i);
void pass_mul      ();
void pass_div      ();
void pass_add      ();
void pass_sub      ();
void pass_in       ();
void pass_out      ();
void pass_jumps    (const unsigned char* binary_code, size_t& i, const int* label_binary_code);
void pass_call     (const unsigned char* binary_code, size_t& i, const int* label_binary_code);
void pass_ret      ();
void pass_sqrt     ();
void pass_end      ();
void make_data     ();  
void set_text_size (const size_t& size);
void make_align    (const size_t& align);
void safe_abcd     ();
void restore_abcd  ();


namespace ELF_HEADER
{
    constexpr DWORD signature              = 0x464C457F;   //Сигнатура elf файла
    constexpr BYTE  elf_class              = 0x2;          //elf64
    constexpr BYTE  endian_type            = 0x1;          //little endian
    constexpr WORD  format_version         = 0x1;          //

    constexpr BYTE  one_byte               = 8;            //8 zeros must be set

    constexpr WORD  file_type              = 0x2;          //file type
    constexpr WORD  machine_type           = 0x3E;         //machine type
    constexpr DWORD version                = 0x1;          //version

    constexpr DWORD virtual_memory_start   = 0x400000;     //entry_point
    constexpr DWORD entry                  = 0x200; 
    constexpr DWORD _0                     = 0x0;          //...

    constexpr DWORD program_header_offset  = 0x40;

    constexpr WORD elf_header_size         = 0x40;
    constexpr WORD size_of_program_header  = 0x38;

    constexpr WORD number_of_prog_entr     = 0x2;

    constexpr WORD section_header_size     = 0x40;
    constexpr WORD section_header_num_entr = 0x2;

    constexpr WORD index_of_shstrtab       = 0x5;

    constexpr QWORD memory_place           = 0x600130; 
}

namespace PROGRAM_HEADER_CONST
{
    constexpr DWORD head_type         = 0x1;                //Loadable segment
    constexpr DWORD RE                = 0x5;                //Read and execute
    constexpr DWORD RW                = 0x6;                //Read and write

    constexpr QWORD size_memory       = 0xD0;               // size of memory
    constexpr QWORD align             = 0x200000;
}

struct PROGRAM_HEADER
{
    DWORD flags;
    QWORD offset;
    QWORD virtual_address;
    QWORD memory_size;       //number of elements in memory

    constexpr PROGRAM_HEADER (const DWORD& flags,           const QWORD& offset,
                              const QWORD& virtual_address, const QWORD& memory_size):
            flags (flags),
            offset (offset),
            virtual_address (virtual_address),
            memory_size (memory_size)
    { set_program_header (); }

private:
    constexpr void set_program_header ();

};

namespace SECTION_HEADER_CONST
{
    constexpr DWORD head_type         = 0x1;
    constexpr QWORD alloc_and_execute = 0x6;
    constexpr QWORD alloc_and_write   = 0x3;

    constexpr QWORD text_offset       = 0x200;
    constexpr QWORD memory_offset     = 0x600000;
    constexpr DWORD mem_size          = 0xD0;
    constexpr BYTE  text_align        = 0x10;
}

struct SECTION_HEADER
{
    QWORD flags;
    QWORD virtual_address;
    QWORD real_address;
    QWORD size;
    QWORD align;

    constexpr SECTION_HEADER (const QWORD& flags, const QWORD& virtual_address, const QWORD& real_adress,
                              const QWORD& size,  const QWORD& align):
            flags           (flags),
            virtual_address (virtual_address),
            real_address    (real_adress),
            size            (size),
            align           (align)
    { set_section_header (); }

private:
    constexpr void set_section_header ();
};

inline void set_zero_byte (const size_t& bytes)
{
    for (size_t i = 0; i < bytes; i++)
        code[code_size++] = 0x00;
}

inline void set_zero_byte_until (const size_t& until)
{
    for (; code_size < until;)
        code[code_size++] = 0x00;
}


template <class TypeValue>
constexpr void set_val (const TypeValue& value)
{
    *( (TypeValue*) &code[code_size]) = value;
    code_size += sizeof (TypeValue);
}

template<typename FirstValue, typename... Others>
constexpr void set_val (const FirstValue& first, const Others&... value)
{
    *( (FirstValue*) &code[code_size]) = first;
    code_size += sizeof (FirstValue);

    set_val (value...);
}




constexpr void PROGRAM_HEADER::set_program_header ()
{
    set_val (PROGRAM_HEADER_CONST::head_type, flags, offset, virtual_address, virtual_address, 
             memory_size, memory_size,        PROGRAM_HEADER_CONST::align);
}

constexpr void SECTION_HEADER::set_section_header ()
{
    set_val ( (DWORD) 0x0,  SECTION_HEADER_CONST::head_type, flags, virtual_address, 
              real_address, size,   (QWORD) 0x0, align,     (QWORD) 0x0);
}

constexpr void Make_elf_header (const DWORD& section_header_offset)
{
    using namespace ELF_HEADER;

    set_val (signature,               elf_class, endian_type, format_version,  (QWORD) 0x0, file_type,
             machine_type,            version,   virtual_memory_start + entry, (DWORD) 0x0, program_header_offset,
             (DWORD) 0x0,             section_header_offset,  (QWORD) 0x0,     elf_header_size,             
             size_of_program_header, number_of_prog_entr, section_header_size, section_header_num_entr,         
             index_of_shstrtab);
}

constexpr void Make_program_header () //size_of_excutable_code
{
    using namespace PROGRAM_HEADER_CONST;

    PROGRAM_HEADER elf_program_headers[2] = {
            PROGRAM_HEADER (RE, 0x0,   ELF_HEADER::virtual_memory_start,    0X0),
            PROGRAM_HEADER (RW, 0x0,   SECTION_HEADER_CONST::memory_offset, size_memory) };
}


constexpr void Make_section_header ()
{
    using namespace SECTION_HEADER_CONST;

    SECTION_HEADER elf_section_headers[2] = {
            SECTION_HEADER (alloc_and_execute, ELF_HEADER::virtual_memory_start + text_offset, text_offset,   0x0,      0x10),
            SECTION_HEADER (alloc_and_write,   0x0,                                            memory_offset, mem_size, 0x4) };

}

void Make_executable_code (unsigned char* binary_code, size_t file_size, int* label_binary_code)
{

    for (size_t i = 8; i < file_size; i++) // skip signature
    {
        label_binary_code[i] = code_size;
        
        switch (binary_code[i])
        {
            case PUSH:
            {
                pass_push (binary_code, i);
                break;
            }

            case POP:
            {
                pass_pop (binary_code, i);
                break;
            }

            case MUL:
            {
                pass_mul ();
                break;
            }

            case DIV:
            {
                pass_div ();
                break;
            }

            case SUB:
            {
                pass_sub ();
                break;
            }

            case ADD:
            {
                pass_add ();
                break;
            }

            case IN_:
            {
                pass_in ();
                break;
            }

            case OUT_:
            {
                pass_out ();
                break;
            }

            case JA:  { }
            case JAE: { }
            case JB:  { }
            case JBE: { }
            case JE:  { }
            case JNE: { }

            case JMP:
            {
                pass_jumps (binary_code, i, label_binary_code);
                break;
            }
    
            case SQRT:
            {
                pass_sqrt ();
                break;
            }

            case CALL:
            {
                pass_call (binary_code, i, label_binary_code);
                break;
            }

            case RET:
            {
                pass_ret ();
                break;
            }

            case END:
            {
                pass_end ();
                break;
            }

            default:
                std::cout << "ERROR";


        }
    }
}

void pass_push (const unsigned char* binary_code, size_t& i)
{
    switch (binary_code[i + 1])
    {
        case INT_:
        {
            set_val (functions::push_dword);
            set_val ( (DWORD) (*(int*)(&binary_code[i + 2]) * PREC) );
            i += sizeof (char) + sizeof (int);
            break;
        }
    
        case REG:
        {
            set_val ( (BYTE) (functions::push_reg + reg_compare[binary_code[i + 2]]));
            i += 2 * sizeof (char);
            break;
        }

        default:
        {
            std::cout << "ERROR";
        }

    }
}

void pass_pop (const unsigned char* binary_code, size_t& i)
{
    switch (binary_code[i + 1])
    {
    
        case REG:
        {
            set_val ( (BYTE) (functions::pop_reg + reg_compare[binary_code[i + 2]]));
            i += 2*sizeof (char);
            break;
        }

        default:
        {
            std::cout << "ERROR";
        }

    }
}



void pass_mul ()
{
    set_val (prefix::REX_WB, functions::mov_reg, rm_byte::rax_r14,     // mov r14, rax
             (BYTE) (functions::pop_reg + reg_compare[ax]),            // pop rax
             prefix::REX_B, (BYTE) (functions::pop_reg + r10),         // pop r10
             (BYTE) (functions::push_reg + reg_compare[dx]),           // push rdx
             prefix::REX_W, functions::xor_reg, rm_byte::rdx_rdx,      // xor rdx, rdx
             prefix::REX_WB, functions::mul_div, rm_byte::mul_r10,     // imul r10

//поддержка  точности
             prefix::REX_B, (BYTE) (functions::mov_dig + r13),         // mov r13, 100
             (DWORD) 100,
             prefix::REX_WB, functions::mul_div, rm_byte::mul_r13,     // idiv r13
//  
             (BYTE) (functions::pop_reg + reg_compare[dx]),            // pop rdx           
             (BYTE) (functions::push_reg + reg_compare[ax]),           // push rax
             prefix::REX_WR, functions::mov_reg, rm_byte::r14_rax);    // mov rax, r14

}

void pass_sub ()
{
    set_val (prefix::REX_B,   (BYTE) (functions::pop_reg + r8),              // pop r8
             prefix::REX_B,   (BYTE) (functions::pop_reg + r9),              // pop r9
             prefix::REX_WRB, functions::sub, rm_byte::r9_r8,                // sub r9, r8
             prefix::REX_B,   (BYTE) (functions::push_reg + r9));            // push r9
} 

void safe_abcd ()
{
    set_val ( (BYTE) (functions::push_reg + reg_compare[ax]),                           // push rax
              (BYTE) (functions::push_reg + reg_compare[bx]),                           // push rbx
              (BYTE) (functions::push_reg + reg_compare[cx]),                           // push rcx
              (BYTE) (functions::push_reg + reg_compare[dx]));                          // push rdx
}  

void restore_abcd ()
{
    set_val ( (BYTE) (functions::pop_reg + reg_compare[dx]),                            // pop rdx
              (BYTE) (functions::pop_reg + reg_compare[cx]),                            // pop rcx
              (BYTE) (functions::pop_reg + reg_compare[bx]),                            // pop rbx
              (BYTE) (functions::pop_reg + reg_compare[ax]));                           // pop rax
}


void pass_sqrt ()
{
    set_val (prefix::REX_B,   (BYTE) (functions::pop_reg + r9));                         // pop r9
    safe_abcd ();

    set_val (prefix::REX_WR, functions::mov_reg, rm_byte::r9_rax,                        // mov rax, r9

//поддержка точности
             prefix::REX_B, (BYTE) (functions::mov_dig + r9), (DWORD) 100,               // mov r9, 100
             prefix::REX_W, functions::xor_reg, rm_byte::rdx_rdx,                        // xor rdx, rdx
             prefix::REX_WB, functions::mul_div, rm_byte::mul_r9,                        // imul r9
//

             prefix::REX_W,  functions::xor_reg, rm_byte::rbx_rbx,                       // xor rbx, rbx
             prefix::REX_W,  prefix::short_, functions::bsr, rm_byte::rax_rcx,           // bsr rcx, rax
             prefix::pref_80, functions::and_cl, (BYTE) 0xfe,                            // and cl, 0feh
             (BYTE) (functions::mov_dig  + reg_compare[dx]), (DWORD) 0x1,                // mov edx, 1
             prefix::REX_W, functions::shl, rm_byte::cl_rdx,                             // shl rdx, cl
                                                                                         // refine:
             prefix::REX_WB, functions::mov_reg, rm_byte::rbx_r8,                        // mov r8, rbx    
             prefix::REX_WB, functions::add_r_r, rm_byte::rdx_r8,                        // add r8, rdx
             prefix::REX_WB, functions::cmp, rm_byte::rax_r8,                            // cmp r8, rax
             jumps::ja_one_byte, fun_distances::jmp_shr,                                 // ja jmp_shr
             prefix::REX_WR, functions::sub, rm_byte::r8_rax,                            // sub rax, r8   
             prefix::REX_W, functions::shr, rm_byte::shr_rbx,                            // shr rbx
             prefix::REX_W, functions::add_r_r, rm_byte::rdx_rbx,                        // add rbx, rdx
             jumps::jmp_one_byte, fun_distances::next,                                   // jmp next
                                                                                         // jmp_shr:
             prefix::REX_W, functions::shr, rm_byte::shr_rbx,                            // shr rbx
                                                                                         // next:
             prefix::REX_W, functions::shr_two, rm_byte::shr_rdx_digit, (BYTE) 0x2,      // shr rdx, 2
             jumps::jne_one_byte, fun_distances::refine,                                 // jne refine
             prefix::REX_WB, functions::mov_reg, rm_byte::rbx_r9);                       // mov r9, rbx
                                      
    restore_abcd ();
    set_val (prefix::REX_B, (BYTE) (functions::push_reg + r9));                          // push r9
}

const BYTE minus = 0x2D;

void pass_in ()
{
    safe_abcd ();

    set_val ( (BYTE) (functions::mov_dig  + reg_compare[ax]), (DWORD) 0x3,              // mov eax, 3
              (BYTE) (functions::mov_dig  + reg_compare[bx]), (DWORD) 0x2,              // mov ebx, 2

              (BYTE) (functions::mov_dig  + reg_compare[cx]), 
                      (DWORD) ELF_HEADER::memory_place,                                 // mov ecx, virtual_memory

              (BYTE) (functions::mov_dig  + reg_compare[dx]), (DWORD) 0x8,              // mov rdx, 1
              functions::int80h,                                                        // int 80h
              prefix::REX_W,   functions::xor_reg, rm_byte::rax_rax,                    // xor rax, rax
              prefix::REX_WRB, functions::xor_reg, rm_byte::r10_r10,                    // xor r10, r10

              prefix::pref_80, functions::cmp_al, rm_byte::rip_plus,                    // cmp byte [value], '-'
              (DWORD) ELF_HEADER::memory_place, minus,

              jumps::jne_one_byte, fun_distances::not_minus,                            // jne loop
              prefix::REX_WB, functions::add_r, rm_byte::dig_r10, (BYTE) 0x1,           // add r10, 1   
                                                                                        // loop:
              prefix::REX_B, functions::add_al, rm_byte::mem_plus_r10,                  // add al, [value + r10]
              (DWORD) ELF_HEADER::memory_place, 

              functions::sub_al, (BYTE) 0x30,                                           // sub al, 30h
              prefix::REX_B, (BYTE) (functions::mov_dig + r9), (DWORD) 0xA,             // mov r9, 10
              prefix::REX_WB, functions::mul_div, rm_byte::mul_r9,                      // imul r9
              prefix::REX_WB, functions::inc, rm_byte::dig_r10,                         // inc r10

              prefix::REX_B,  functions::cmp_mem, rm_byte::cmp_mem_rm,                  // cmp byte [value + r10], 0ah
              (DWORD) ELF_HEADER::memory_place, (BYTE) 0xA, 

              jumps::jne_one_byte, fun_distances::repeat,                               // jne loop
              prefix::REX_WB, functions::mul_div, rm_byte::div_r9,                      // idiv r9

              prefix::pref_80, functions::cmp_al, rm_byte::rip_plus,                    // cmp byte [value], '-'
              (DWORD) ELF_HEADER::memory_place, minus,

              jumps::jne_one_byte, fun_distances::skip_mul,                             // jne skip_mul
              prefix::REX_WB, functions::mov_dig_rm, rm_byte::dig_r9, (DWORD)0xFFFFFFFF,// mov r9, -1
              prefix::REX_WB, functions::mul_div, rm_byte::mul_r9,                      // mul r9
                                                                                        // skip_mul:
//поддержка точности
              prefix::REX_B, (BYTE) (functions::mov_dig + r9), (DWORD) 100,             // mov r9, 100
              prefix::REX_W, functions::xor_reg, rm_byte::rdx_rdx,                       // xor rdx, rdx
              prefix::REX_WB, functions::mul_div, rm_byte::mul_r9,                      // imul r9
//

              prefix::REX_WB, (BYTE) (functions::mov_reg + reg_compare[ax]),            // mov r9, rax
              rm_byte::rax_r9);
            

    restore_abcd ();

    set_val (prefix::REX_B, (BYTE) (functions::push_reg + r9));                          // push r9

    
}

void pass_out ()
{
    set_val (prefix::REX_B, (BYTE) (functions::pop_reg + r13));                         // pop r13
    safe_abcd ();

    set_val (prefix::REX_WR, (BYTE) (functions::mov_reg  + reg_compare[ax]),            // mov rax, r13 
             rm_byte::r13_rax,

             prefix::REX_W, functions::cmp_digit, rm_byte::rax_digit, (BYTE) 0x0,       // cmp rax, 0
             jumps::jge_one_byte, fun_distances::skip_min,                              // jge skip_min
             prefix::REX_WB, functions::mov_dig_rm, rm_byte::dig_r13, (DWORD) -1,       // mov r13, -1
             prefix::REX_W, functions::xor_reg, rm_byte::rdx_rdx,                       // xor rdx, rdx
             prefix::REX_WB, functions::mul_div, rm_byte::imul_r13,                     // imul r13
             prefix::REX_WB, functions::mov_reg, rm_byte::rax_r13,                      // mov r13, rax
             (BYTE) (functions::mov_dig  + reg_compare[ax]), (DWORD) 0x4,               // mov eax, 4
             (BYTE) (functions::mov_dig  + reg_compare[bx]), (DWORD) 0x1,               // mov ebx, 1

             prefix::pref_c6, functions::mov_dig_mem, rm_byte::dig_mem,                 // mov byte [value], '-'
             (DWORD) (ELF_HEADER::memory_place), (BYTE) '-',
             
             prefix::REX_W, (BYTE) (functions::mov_dig  + reg_compare[cx]),             // mov rcx, value
             (QWORD) (ELF_HEADER::memory_place),

             (BYTE) (functions::mov_dig  + reg_compare[dx]), (DWORD) 0x1,               // mov edx, 1
             functions::int80h,
             prefix::REX_WR, functions::mov_reg, rm_byte::r13_rax,                      // mov rax, r13 

//поддержка  точности
             prefix::REX_B, (BYTE) (functions::mov_dig + r13),                          // mov r13, 100
             (DWORD) 100,
             prefix::REX_W, functions::xor_reg, rm_byte::rdx_rdx,                       // xor rdx, rdx
             prefix::REX_WB, functions::mul_div, rm_byte::mul_r13,                      // idiv r13
//  

            

             prefix::REX_W, functions::mov_mem_plus_digit, 
             (QWORD) (ELF_HEADER::memory_place + PROGRAM_HEADER_CONST::size_memory),   // mov rsi, value + 15h
    
    
             prefix::REX_B, (BYTE) (functions::mov_dig + r9), (DWORD) 0xA,              //mov r9, 0xa
             prefix::REX_W, functions::xor_reg, rm_byte::rcx_rcx,                       // xor rcx, rcx
                                                                                        // loop2
             prefix::REX_W, functions::xor_reg, rm_byte::rdx_rdx,                       // xor rdx, rdx
             prefix::REX_W, functions::inc, rm_byte::inc_rcx,                           // inc rcx         
             prefix::REX_WB, functions::mul_div, rm_byte::div_r9,                       // idiv r9
             functions::mov_mem_from_al, rm_byte::dl_rsi,                               // mov byte [rsi], dl 
             prefix::pref_80, functions::add_rsi, (BYTE) 0x30,                          // add byte [rsi], 48       
             prefix::REX_W, functions::inc, rm_byte::dec_rsi,                           // dec rsi
             prefix::REX_W, functions::cmp_r_digit, rm_byte::rax_digit, (BYTE) 0x0,     // cmp rax, 0 
             jumps::jne_one_byte, fun_distances::loop2,                                 // jne loop2
             (BYTE) (functions::mov_dig  + reg_compare[ax]), (DWORD) 0x4,               // mov eax, 4
             (BYTE) (functions::mov_dig  + reg_compare[bx]), (DWORD) 0x1,               // mov ebx, 1
             prefix::REX_W, functions::mov_reg, rm_byte::rcx_rdx,                       // mov rdx, rcx 
             prefix::REX_W, functions::mov_reg, rm_byte::rsi_rcx,                       // mov rcx, rsi
             prefix::REX_W, functions::inc, rm_byte::inc_rcx,                           // inc rcx                    
             functions::int80h,
             (BYTE) (functions::mov_dig  + reg_compare[ax]), (DWORD) 0x4,               // mov eax, 4
             prefix::pref_c6, functions::add_r_r, (BYTE) 0xA,                           // mov [rcx], 0xA
             (BYTE) (functions::mov_dig  + reg_compare[dx]), (DWORD) 0x1,               // mov rdx, 1
             functions::int80h);

    restore_abcd ();

}

void pass_jumps (const unsigned char* binary_code, size_t& i, const int* label_binary_code)
{

    switch (binary_code[i])
    {
        case JMP:
        {
            set_val (jumps::jmp);
            break;
        }

        default:
        {
            set_val (prefix::REX_B,   (BYTE) (functions::pop_reg + r11),         // pop r11
                     prefix::REX_B,   (BYTE) (functions::pop_reg + r12),         // pop r12
                     prefix::REX_WRB, functions::cmp, rm_byte::r12_r11);         // cmp r12, r11
            
            switch (binary_code[i])
            {

                case JA:
                {
                    set_val (prefix::short_, jumps::ja);
                    break;
                }

                case JAE:
                {
                    set_val (prefix::short_, jumps::jae);
                    break;
                }

                case JB:
                {
                    set_val (prefix::short_, jumps::jb);
                    break;
                }

                case JBE: 
                {
                    set_val (prefix::short_, jumps::jbe);
                    break;
                }

                case JE:
                {
                    set_val (prefix::short_, jumps::je);
                    break;
                }

                case JNE: 
                {
                    set_val (prefix::short_, jumps::jne);
                    break;
                }
             
                default:
                {
                    std::cout << "ERROR";
                }

            }
                                                
        }
    }


    set_val ( (DWORD) (label_binary_code[*(int*)(&binary_code[i + 1]) + 8] - code_size - sizeof (int)));
    i += sizeof (int);
}

void pass_call (const unsigned char* binary_code, size_t& i, const int* label_binary_code)
{
    set_val (functions::call, (DWORD) (label_binary_code[*(int*)(&binary_code[i + 1]) + 8] - code_size - sizeof (int) - 1));

    i += 2*sizeof (int);
}

void pass_ret ()
{
    set_val (functions::ret);
}

void pass_end ()
{
    set_val ( (BYTE) (functions::mov_dig  + reg_compare[ax]), (DWORD) 0x1,              // mov eax, 3
              (BYTE) (functions::mov_dig  + reg_compare[bx]), (DWORD) 0x0,              // mov ebx, 2
              functions::int80h);
}

void make_data ()
{
    set_zero_byte (SECTION_HEADER_CONST::mem_size);
}

void set_text_size (const size_t& size)
{
    *(QWORD*) &code[0xd0] = size;
    *(QWORD*) &code[0x60] = size;
    *(QWORD*) &code[0x68] = size;
}

void make_align (const size_t& align)
{
    set_zero_byte (align);
}

void pass_add ()
{
    set_val (prefix::REX_B,   (BYTE) (functions::pop_reg + r8),              // pop r8
             prefix::REX_B,   (BYTE) (functions::pop_reg + r9),              // pop r9
             prefix::REX_WRB, functions::add_r_r, rm_byte::r9_r8,            // add r9, r8
             prefix::REX_B,   (BYTE) (functions::push_reg + r9));            // push r9
} 

void pass_div ()
{
    set_val (prefix::REX_WB, functions::mov_reg, rm_byte::rax_r14,     // mov r14, rax
             prefix::REX_B, (BYTE) (functions::pop_reg + r10),         // pop r10
             (BYTE) (functions::pop_reg + reg_compare[ax]),            // pop rax
             (BYTE) (functions::push_reg + reg_compare[dx]),           // push rdx
             prefix::REX_W, functions::xor_reg, rm_byte::rdx_rdx,      // xor rdx, rdx

//поддержка  точности
             prefix::REX_B, (BYTE) (functions::mov_dig + r13),         // mov r13, 100
             (DWORD) 100,
             prefix::REX_WB, functions::mul_div, rm_byte::imul_r13,    // imul r13
// 
            
             prefix::REX_WB, functions::mul_div, rm_byte::idiv_r10,    // idiv r10
             (BYTE) (functions::pop_reg + reg_compare[dx]),            // pop rdx
             (BYTE) (functions::push_reg + reg_compare[ax]),           // push rax
             prefix::REX_WR, functions::mov_reg, rm_byte::r14_rax);    // mov rax, r14

}









