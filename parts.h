typedef unsigned long        QWORD;
typedef unsigned int        DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

char*  code      = nullptr;
size_t code_size = 0;

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

    constexpr DWORD entry                  = 0x4000B0;     //entry_point
    constexpr DWORD _0                     = 0x0;          //...

    constexpr DWORD program_header_offset  = 0x40;
//    constexpr DWORD section_header_offset  = 0x200;

    constexpr WORD elf_header_size         = 0x40;
    constexpr WORD size_of_program_header  = 0x38;

    constexpr WORD number_of_prog_entr     = 0x2;

    constexpr WORD section_header_size     = 0x40;
    constexpr WORD section_header_num_entr = 0x2;

    constexpr WORD index_of_shstrtab       = 0x5;
}

namespace PROGRAM_HEADER_CONST
{
    constexpr DWORD head_type         = 0x1;                //Loadable segment
    constexpr DWORD RE                = 0x5;                //Read and execute
    constexpr DWORD RW                = 0x6;                //Read and write
    constexpr QWORD virtual_address1  = 0x400000;
    constexpr QWORD virtual_address2  = 0x6000D4;
//    constexpr QWORD size_address1     = 0x00000000000000D2;
    constexpr QWORD size_address2     = 0x1;
    constexpr QWORD align             = 0x200000;
}    

struct PROGRAM_HEADER
{
    DWORD flags;
    QWORD offset;
    QWORD virtual_address; 
    QWORD memory_size;       //number of elements in memory
    
    PROGRAM_HEADER (const DWORD& flags,           const QWORD& offset, 
                    const QWORD& virtual_address, const QWORD& memory_size):
        flags (flags),
        offset (offset),
        virtual_address (virtual_address),
        memory_size (memory_size) 
        { set_program_header (); }

private:
    void set_program_header ();
        
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

template<typename TypeValue>
inline void set_val (const TypeValue& value)
{
    *( (TypeValue*) &code[code_size]) = value;
    code_size += sizeof (TypeValue);
}


void PROGRAM_HEADER::set_program_header ()
{
    set_val (PROGRAM_HEADER_CONST::head_type);
    set_val (flags);
    set_val (offset);
    set_val (virtual_address);
    set_val (virtual_address);
    set_val (memory_size);
    set_val (memory_size);
    set_val (PROGRAM_HEADER_CONST::align);
}

namespace SECTION_TABLE
{
}

void Make_elf_header (const DWORD& section_header_offset)
{
    using namespace ELF_HEADER;

    set_val (signature);

    set_val (elf_class);
    set_val (endian_type);
    set_val (format_version);
    
    set_zero_byte (one_byte);

    set_val (file_type);
    set_val (machine_type);
    set_val (version);
    set_val (entry);

    set_zero_byte (one_byte >> 1);
    
    set_val (program_header_offset);
    
    set_zero_byte (one_byte >> 1);

    set_val (section_header_offset);

    set_zero_byte (one_byte >> 1);

    set_zero_byte (one_byte >> 1);

    set_val (elf_header_size);



    set_val (size_of_program_header);
    set_val (number_of_prog_entr);
    set_val (section_header_size);
    set_val (section_header_num_entr);
    set_val (index_of_shstrtab);
}

void Make_program_header (const QWORD& size_address1) //size_of_excutable_code
{
using namespace PROGRAM_HEADER_CONST; 

    PROGRAM_HEADER elf_program_headers[2] = { 
        PROGRAM_HEADER (RE, 0x0,  virtual_address1, size_address1), 
        PROGRAM_HEADER (RW, 0xD4, virtual_address2, size_address2) };
}


void Make_section_table ()
{
    using namespace SECTION_TABLE;
//
//    for (size_t i = 0; i < 3; i++)
//    {
//        for (size_t j = 0; j < 2; j++)
//            set_val (name[i][j]);
//
//        set_val (virtual_size);
//        set_val (virtual_address[i]);
//        set_val (size_of_raw_data);
//        set_val (pointer_to_raw_data[i]);
//
//        set_zero_byte (unused_bytes12);
//
//        set_val (characteristics[i]);
//    }
}

void Make_executable_code (const unsigned char* execut_code, size_t size)
{
    memcpy (&code[code_size], execut_code, size);
    code_size += size;
}


