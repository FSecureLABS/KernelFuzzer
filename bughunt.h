#ifndef BUGHUNT_H
#define BUGHUNT_H

/* CONFIGURATION "FLAGS"... */

#define ENABLE_POST_MORTEM
/* #undef ENABLE_POST_MORTEM */


/* TYPEDEFS USED BY THE BUGHUNT APPLICATIONS... */

#if _M_IX86
    typedef unsigned __int32 POINTER;
#elif _M_X64
    typedef unsigned __int64 POINTER;
#endif

typedef enum
{
    _BOOL = 1,
    _BOOL_PTR = 2,
    _CHAR8 = 3,
    _CHAR8_PTR = 4,
    _CHAR16 = 5,
    _CHAR16_PTR = 6,
    _INT8 = 7,
    _INT8_PTR = 8,
    _INT16 = 9,
    _INT16_PTR = 10,
    _INT32 = 11,
    _INT32_PTR = 12,
    _INT64 = 13,
    _INT64_PTR = 14,
    _UINT8 = 15,
    _UINT8_PTR = 16,
    _UINT16 = 17,
    _UINT16_PTR = 18,
    _UINT32 = 19,
    _UINT32_PTR = 20,
    _UINT64 = 21,
    _UINT64_PTR = 22,
    _REAL32 = 23,
    _REAL32_PTR = 24,
    _REAL64 = 25,
    _REAL64_PTR = 26,
	_HANDLE = 28,

    _VOID_PTR = 27,

    NIL = 0

} DATATYPE;

/* SYSCALL DEFINITIONS... */

#define SYSCALL_ARGUMENT_N             ((size_t)33)

typedef struct
{
    DWORD uid;
    DATATYPE argument_datatypes[SYSCALL_ARGUMENT_N];
    DATATYPE return_datatype;
} SYSCALL;

// Struct to hold a handle value and its index in the array of handles.
typedef struct {
    HANDLE value;
    int index;
} BH_Handle;

#include "bughunt_syscalls.h"
//#include "library_calls.h"

/* TYPEDEFS USED BY SYSCALLS... */

typedef unsigned __int8 bool_t;

typedef char char8_t;
typedef wchar_t char16_t;

typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

typedef float real32_t;
typedef double real64_t;

/* FUNCTION PROTOTYPES... */

bool_t get_fuzzed_bool (void);
bool_t* get_fuzzed_bool_ptr (void);
char8_t get_fuzzed_char8 (void);
char8_t* get_fuzzed_char8_ptr (void);
char16_t get_fuzzed_char16 (void);
char16_t* get_fuzzed_char16_ptr (void);
int8_t get_fuzzed_int8 (void);
int8_t* get_fuzzed_int8_ptr (void);
int16_t get_fuzzed_int16 (void);
int16_t* get_fuzzed_int16_ptr (void);
int32_t get_fuzzed_int32 (void);
int32_t* get_fuzzed_int32_ptr (void);
int64_t get_fuzzed_int64 (void);
int64_t* get_fuzzed_int64_ptr (void);
uint8_t get_fuzzed_uint8 (void);
uint8_t* get_fuzzed_uint8_ptr (void);
uint16_t get_fuzzed_uint16 (void);
uint16_t* get_fuzzed_uint16_ptr (void);
uint32_t get_fuzzed_uint32 (void);
uint32_t* get_fuzzed_uint32_ptr (void);
uint64_t get_fuzzed_uint64 (void);
uint64_t* get_fuzzed_uint64_ptr (void);
real32_t get_fuzzed_real32 (void);
real32_t* get_fuzzed_real32_ptr (void);
real64_t get_fuzzed_real64 (void);
real64_t* get_fuzzed_real64_ptr (void);

BYTE* get_fuzzed_block_ptr (const DATATYPE* const, DWORD);

double random_double_0_to_1 (void);

DWORD random_DWORD_0_to_N (DWORD);

SYSCALL* random_SYSCALL (void);

/* FUNCTION IMPLEMENTATIONS... */

bool_t get_fuzzed_bool (void)
{
    bool_t bool_BH[] = {0, 1};
    bool_t n;

    n = bool_BH[rand() % sizeof(bool_BH) / sizeof(bool_BH[0])];
    logger("//[Get Fuzzed Value] get_fuzzed_bool : n = %hd", n);
    return n;
}

char8_t get_fuzzed_char8 (void)
{
    char8_t char8_BH[] = { ' ', '\t', '\n', '\r', '\\', '/', '@', ':', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_', '+', '=', ':' };
    char8_t n;

    n = char8_BH[rand() % sizeof(char8_BH) / sizeof(char8_BH[0])];
    logger("//[Get Fuzzed Value] get_fuzzed_char8 : n = %c", n);
    return n;
}


char16_t get_fuzzed_char16 (void)
{
    char16_t char16_BH[] = { ' ', '\t', '\n', '\r', '\\', '/', '@', ':', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_', '+', '=', ':' };
    char16_t n;

    n = char16_BH[rand() % sizeof(char16_BH) / sizeof(char16_BH[0])];
    logger("//[Get Fuzzed Value] get_fuzzed_char8 : n = %c", n);
    return n;
}

int8_t get_fuzzed_int8 (void)
{
   
    int8_t n;

    switch (rand() % 8) {
        case 0:
            n = 1;                   // one
            break;
        case 1:
            n = 0;
            break;
        case 2:
            n = -1;
            break;
        case 3:
            n = 8;
        case 4:
            n = 16;
        case 5:
            n = 32;
        case 6:
            n = 64;
		case 7:
			switch (rand() % 3) {
				case 0:
					n = 0xff;                // max
					break;
				case 1:
					n = 1UL << (rand() & 7);  // 2^n (1 -> 128)
					break;
				case 2:
					n = rand() & 0xff;         // 0 -> 0xff
					break;

			}

    }

    logger("//[Get Fuzzed Value] get_fuzzed_int8 : n = %hhd", n);
    return n;
}

int16_t get_fuzzed_int16 (void)
{
  
    int16_t n;

    switch (rand() % 9) {
        case 0:
			switch (rand() % 4) {
				case 0:
					n = 0x8000 >> (rand() & 7); // 2^n (0x100 -> 0x8000)
					break;
				case 1:
					n = rand() & 0xffff;        // 0 -> 0xffff
					break;
				case 2:
					n = 0xff00 | rand() & 0xff; // 0xff00 -> 0xffff
					break;
				case 3:
					n = 0xffff;                 // max
					break;
			}
            break; 
        case 1:
            n = 1;
            break;
        case 2:
            n = 0;
            break;
        case 3:
            n = -1;
            break;
        case 4:
            n = 8;
            break;
        case 5:
            n = 16;
            break;
        case 6:
            n = 32;
            break;
        case 7:
            n = 64;
            break;
        case 8:
            n = 128;
            break;
    }
    logger("//[Get Fuzzed Value] get_fuzzed_int16 : n = %hd", n);
    return n;
}

int32_t get_fuzzed_int32 (void)
{
    
    int32_t n;

    switch (rand() % 10) {
        case 0:
			switch (rand() % 11)
			{
				case 0:
					n = 0x80000000 >> (rand() & 0x1f);    // 2^n (1 -> 0x10000)
					break;
				case 1:
					n = rand();                           // 0 -> RAND_MAX (likely 0x7fffffff)
					break;
				case 2:
					n = (unsigned int)0xff << (4 * (rand() % 7));
					break;
				case 3:
					n = 0xffff0000;
					break;
				case 4:
					n = 0xffffe000;
					break;
				case 5:
					n = 0xffffff00 | rand() & 0xff;
					break;
				case 6:
					n = 0xffffffff - 0x1000;
					break;
				case 7:
					n = 0x1000;
					break;
				case 8:
					n = 0x1000 * ((rand() % (0xffffffff / 0x1000)) + 1);
					break;
				case 9:
					n = 0xffffffff;                     // max
					break;
				case 10:
					n = 0x7fffffff;
					break;
			}
            break;  
        case 1:
            n = 1;
            break;
        case 2:
            n = 0;
            break;
        case 3:
            n = -1;
            break;
        case 4:
            n = 8;
            break;
        case 5:
            n = 16;
            break;
        case 6:
            n = 32;
            break;
        case 7:
            n = 64;
            break;
        case 8:
            n = 128;
            break;
        case 9:
            n = 256;
    }
    logger("//[Get Fuzzed Value] get_fuzzed_int32 : n = %ld", n);
    return n;
}

int64_t get_fuzzed_int64 (void)
{

    int64_t n;

    switch(rand() % 10) {
        case 0:
            n = 0;
            break;
        case 1:
            n = 1;
            break;
        case 2:
			switch (rand() % 1) {
				case 0:
					n = 0xffffffffffffffff;
					break;
				case 1:
					n = rand() & 0xffffffffffffffff;
					break;
			}
            break;
        case 3:
            n = -1;
            break;
        case 4:
            n = 8;
            break;
        case 5:
            n = 16;
            break;
        case 6:
            n = 32;
            break;
        case 7:
            n = 64;
            break;
        case 8:
            n = 128;
            break;
        case 9:
            n = 256;
    }
    logger("//[Get Fuzzed Value] get_fuzzed_int64 : n = %lld", n);
    return n;
}

uint8_t get_fuzzed_uint8 (void)
{

    uint8_t n = (uint8_t)get_fuzzed_int8();
    logger("//[Get Fuzzed Value] get_fuzzed_uint8 : n = %hhu", n);
    return n;
}

uint16_t get_fuzzed_uint16 (void)
{

    uint16_t n = get_fuzzed_int16();
    logger("//[Get Fuzzed Value] get_fuzzed_uint16 : n = %hu", n);
    return n;    
}

uint32_t get_fuzzed_uint32 (void)
{

    uint32_t n = get_fuzzed_int32();
    logger("//[Get Fuzzed Value] get_fuzzed_uint32 : n = %lu", n);
    return n;
}

uint64_t get_fuzzed_uint64 (void)
{

    uint64_t n = get_fuzzed_int64();
    logger("//[Get Fuzzed Value] get_fuzzed_uint64 : n = %llu", n);
    return n;
}

real32_t get_fuzzed_real32 (void)
{

    real32_t n = random_double_0_to_1();
    logger("//[Get Fuzzed Value] get_fuzzed_real32 : n = %lf", n);
    return;
}

real64_t get_fuzzed_real64 (void)
{

    real64_t n = random_double_0_to_1();
    logger("//[Get Fuzzed Value] get_fuzzed_real64 : n = %llf", n);
    return n;
}

double random_double_0_to_1 (void)
{

    // http://stackoverflow.com/questions/1340729/how-do-you-generate-a-random-double-uniformly-distributed-between-0-and-1-from-c
    return ((double)rand() / (double)RAND_MAX);
}

DWORD random_DWORD_0_to_N (DWORD n)
{
    /* RETURNS A PSEUDO-RANDOM DWORD IN THE RANGE [0 ... n]... */
    return ((DWORD)(random_double_0_to_1 () * n));
}

SYSCALL* random_SYSCALL (void)
{
    unsigned int n = sizeof (SYSCALLS) / sizeof (SYSCALLS[0]);
    return (&(SYSCALLS[random_DWORD_0_to_N (n)]));
}

#endif /* BUGHUNT_H... */