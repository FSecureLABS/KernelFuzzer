#ifndef BUGHUNT_THREAD_H
#define BUGHUNT_THREAD_H

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "bughunt.h"
#include "handles_database.h"
#include "library_calls.h"
#include "hooking.h"

#ifdef _M_IX86

__declspec(naked) DWORD __stdcall bughunt_syscall (
    
    DWORD _syscall_uid,

    DWORD _dw0x01,
    DWORD _dw0x02,
    DWORD _dw0x03,
    DWORD _dw0x04,
    DWORD _dw0x05,
    DWORD _dw0x06,
    DWORD _dw0x07,
    DWORD _dw0x08,
    DWORD _dw0x09,
    DWORD _dw0x0A,
    DWORD _dw0x0B,
    DWORD _dw0x0C,
    DWORD _dw0x0D,
    DWORD _dw0x0E,
    DWORD _dw0x0F,
    DWORD _dw0x10,
    DWORD _dw0x11,
    DWORD _dw0x12,
    DWORD _dw0x13,
    DWORD _dw0x14,
    DWORD _dw0x15,
    DWORD _dw0x16,
    DWORD _dw0x17,
    DWORD _dw0x18,
    DWORD _dw0x19,
    DWORD _dw0x1A,
    DWORD _dw0x1B,
    DWORD _dw0x1C,
    DWORD _dw0x1D,
    DWORD _dw0x1E,
    DWORD _dw0x1F,
    DWORD _dw0x20
)
{
	__asm{
		pop edx;
		pop eax;	//_syscall_uid
		push edx;	//return
		mov edx, 0x7FFE0300;
		call dword ptr [edx];
		retn 0x80;	
	}
}

#elif _M_IX64

extern DWORD __stdcall bughunt_syscall(

    DWORD _syscall_uid,

    QWORD _dw0x01,
    QWORD _dw0x02,
    QWORD _dw0x03,
    QWORD _dw0x04,
    QWORD _dw0x05,
    QWORD _dw0x06,
    QWORD _dw0x07,
    QWORD _dw0x08,
    QWORD _dw0x09,
    QWORD _dw0x0A,
    QWORD _dw0x0B,
    QWORD _dw0x0C,
    QWORD _dw0x0D,
    QWORD _dw0x0E,
    QWORD _dw0x0F,
    QWORD _dw0x10,
    QWORD _dw0x11,
    QWORD _dw0x12,
    QWORD _dw0x13,
    QWORD _dw0x14,
    QWORD _dw0x15,
    QWORD _dw0x16,
    QWORD _dw0x17,
    QWORD _dw0x18,
    QWORD _dw0x19,
    QWORD _dw0x1A,
    QWORD _dw0x1B,
    QWORD _dw0x1C,
    QWORD _dw0x1D,
    QWORD _dw0x1E,
    QWORD _dw0x1F,
    QWORD _dw0x20
);

#endif

extern DWORD syscall_count;

DWORD bughunt_thread(unsigned int seed)
{
    unsigned int syscall_idx = 0;
    SYSCALL* syscall = NULL;
    unsigned int syscall_argument_datatype_idx = 0;
    unsigned int syscall_arguments[SYSCALL_ARGUMENT_N - 1]; // DWORD syscall_arguments[32];
    FILE* stream; // For logging.

    BH_Handle syscall_handle_argument;
    // The syscall_log_string will hold the string to be logged before the syscall invocation.
    char syscall_log_string[512];
    memset(syscall_log_string, '\0', 512);

    // It turns out rand() is thread-safe after all as its state is kept in a thread-local storage (TLS). This means we have to seed every single state on its own. In this case we choose to use a comination of time(NULL), current process ID, and current thread ID.
	if (seed == 1)
	{
		seed = time(NULL) + GetCurrentProcessId() + GetCurrentThreadId();
		logger("//[PRNG Seed] (0x%08X, 0x%08X, %u)", GetCurrentProcessId(), GetCurrentThreadId(), seed);
		srand(seed);
	}
	else //we have been given a seed to use, so use that.
	{
        logger("//[PRNG Seed] (0x%08X, 0x%08X, %u)", GetCurrentProcessId(), GetCurrentThreadId(), seed);
		srand(seed);
	}

   for (syscall_idx = 0; syscall_idx < syscall_count; syscall_idx += 1)
    {
        
        // Invoke one or more library calls.
        while (TRUE) {
            //fflush(NULL);

            // To hook or not to hook? Hook functions at random.
            if (rand() % 5 == 1) {
                // Uncomment below for hooking.
                // 1. Okay, we'll hook. Proceed with installing hook.
                //BH_SetWindowsHookEx();
                // 2. Make a library call.
                (*random_LIBRARY_CALL())();
                // 3. Uninstall the hook.
                //BH_UnhookWindowsHookEx();
            }
            else {
                (*random_LIBRARY_CALL())();
            }
            if (rand() % 2) {
                break;
		   }
	   }
       // Start cionstructing the syscall invocation log string little by little, i.e. argument by argument.
        
        syscall = random_SYSCALL ();
        syscall_argument_datatype_idx = 0;
        sprintf(syscall_log_string, "bughunt_syscall(0x%08x,", syscall->uid);
    
        while ((syscall_argument_datatype_idx < (SYSCALL_ARGUMENT_N - 1))
            && (syscall->argument_datatypes[syscall_argument_datatype_idx] != NIL))
        {
            //logger("//syscall_argument_datatype_idx = %d\n", syscall_argument_datatype_idx);
        	switch (syscall->argument_datatypes[syscall_argument_datatype_idx])
        	{
                // Something to check is whether the 0x%08x format string specifier is okay in all cases, e.g. 64-bit.
        	case _BOOL:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_bool());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;
        	
			case _CHAR8:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_char8());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;

        	case _CHAR16:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_char16());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;

        	case _INT8:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_int8());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;

        	case _INT16:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_int16());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;

        	case _INT32:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_int32());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;

        	case _INT64:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_int64());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;

        	case _UINT8:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_uint8());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;
  
        	case _UINT16:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_uint16());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;

        	case _UINT32:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_uint32());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;

        	case _UINT64:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_uint64());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;

        	case _REAL32:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_real32());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;

        	case _REAL64:
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)get_fuzzed_real64());
                sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08x,", syscall_arguments[syscall_argument_datatype_idx]);
        		break;

        	case _HANDLE:
                syscall_handle_argument = get_random_HANDLE();
        		syscall_arguments[syscall_argument_datatype_idx] = ((DWORD)syscall_handle_argument.value);
                sprintf(syscall_log_string + strlen(syscall_log_string), "get_specific_HANDLE(%d),", syscall_handle_argument.index);
        		break;

        	} // Switch statement.
        	syscall_argument_datatype_idx += 1;
        } // While loop.
        
        for (; syscall_argument_datatype_idx < 32; ) {
            //logger("//syscall_argument_datatype_idx = %d\n", syscall_argument_datatype_idx);
            sprintf(syscall_log_string + strlen(syscall_log_string), "0x%08X,", 0x4142434445464748);
            syscall_argument_datatype_idx += 1;
        }

        sprintf(syscall_log_string + strlen(syscall_log_string) - 1, ");");
        logger(syscall_log_string);

        /* INVOKE THE SYSCALL... */
        bughunt_syscall (
            syscall->uid,
            syscall_arguments[0],
            syscall_arguments[1],
            syscall_arguments[2],
            syscall_arguments[3],
            syscall_arguments[4],
            syscall_arguments[5],
            syscall_arguments[6],
            syscall_arguments[7],
            syscall_arguments[8],
            syscall_arguments[9],
            syscall_arguments[10],
            syscall_arguments[11],
            syscall_arguments[12],
            syscall_arguments[13],
            syscall_arguments[14],
            syscall_arguments[15],
            syscall_arguments[16],
            syscall_arguments[17],
            syscall_arguments[18],
            syscall_arguments[19],
            syscall_arguments[20],
            syscall_arguments[21],
            syscall_arguments[22],
            syscall_arguments[23],
            syscall_arguments[24],
            syscall_arguments[25],
            syscall_arguments[26],
            syscall_arguments[27],
            syscall_arguments[28],
            syscall_arguments[29],
            syscall_arguments[30],
            syscall_arguments[31]
        );
    } // For loop.
    return (0);
}

#endif /* BUGHUNT_THREAD_H... */
