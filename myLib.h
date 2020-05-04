#include <cstdio>
#include "assert.h"
#include "sys/stat.h"
#include <cstring>
/*!
 * Function compares words
 * @param[in] compare1 First word to compare with
 * @param[in] compare2 Second word to compare with
 * @return 0 if compare 1 != compare 2
 * @return 1 if compare 1 == compare 2
 */
bool WordCmp (const char* compare1, const char* compare2 );

/*!
 * If has define ASSEMBLER counts size of massive, which includes machine code,
 * else counts number of words in the text
 * @param[in] str buffer with text
 * @param[in] length length of the text
 * @return size of massive, which includes machine code(ifdef ASSEMBER)
 * @return number of words(undef ASSEMBLER)
 */
int WordCount ( char* str, int length );

/*!
 * Counts size of file
 * @param fname name of file
 * @return size of file
 */
int Sizecount ( const char *fname );

/*! Makes a buffer
 * @param[in] name name of file
 * @param[in] length length of file
 * @return dynamic buffer with text from file
 */
unsigned char* Buf ( char* name, int length );

/*! Makes a buffer
 * @param[in] name name of file
 * @return dynamic buffer with text from file
 */
unsigned char* BufferMaker ( char* name);

bool WordCmp ( const char* compare1, const char* compare2 )
{
    assert ( compare1 );
    assert ( compare2 );
    for ( int i = 0; i < 4; i++ )
    {
        if ( compare1[i] != compare2[i])
        {
            return false;
        }
    }
    return true;
}

#define ASSEMBLER
int WordCount ( char* str, int length )
{
    int i = 0;
    int number = 0;
    int kolofcifra = 0;
    str[length + 1] = '\n';
    for ( i; i < length + 1 ; i++ )
    {
        if ( ( ( str[i] == ' ' ) && ( str[i+1] != ' ' ) ) || ( ( str[i] != ' ' ) && ( str[i+1] == '\n'  ) ) )
        {
            number++;
            kolofcifra++;
            //printf("\n------%d--------\n", number);

        }
        if ( ( str[i] >= '1') &&  ( str[i] <= '9') && ( str[i+1] != ' ' ) )
        {
            kolofcifra+=3;
        }
    }

    if ( ( str[i] != ' ' ) && ( str[i - 1] == ' ' ) )
    {
        //printf("$$-%c-$$", str[i]);
        number++;
        kolofcifra++;
    }
#ifdef ASSEMBLER
    return kolofcifra;
#else
    return number;
#endif
}

int Sizecount ( const char *fname )
{
    assert ( fname );
    //fname[0] = 'u'; /* поднагадить с имечком */
    struct stat sizeknowing = {};

    stat ( fname, &sizeknowing );

    assert ( sizeknowing.st_size >= 0 );

    return sizeknowing.st_size;
}

unsigned char* Buf ( const char* name, int length )
{
    assert ( name );
    assert (length>0 );
    unsigned char* buffer = ( unsigned char* )  calloc ( length + 1, sizeof (unsigned char) );


    FILE *fp = fopen(name, "rb");
    assert ( fp );

    fread ( buffer, sizeof(char), ( length + 1 ), fp);
    assert ( buffer );

    buffer[length] = '\0';

    fclose ( fp );

    return buffer;
}


unsigned char* BufferMaker (const char* name)
{
    int length = Sizecount ( name );

    unsigned char* buffer = Buf ( name, length );

    return buffer;
}

