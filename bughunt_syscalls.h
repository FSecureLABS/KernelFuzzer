#ifndef BUGHUNT_SYSCALLS_H
#define BUGHUNT_SYSCALLS_H

// Other Resources
// https://src.chromium.org/svn/trunk/src/tools/traceline/traceline/syscall_map.h
// http://j00ru.vexillium.org/ntapi_64/
// http://j00ru.vexillium.org/ntapi/
// http://j00ru.vexillium.org/win32k_syscalls/
// http://www.cc.gatech.edu/~brendan/volatility/dl/ssdt.py

/*
typedef struct
{
DWORD uid;
DATATYPE argument_datatypes[SYSCALL_ARGUMENT_N];
DATATYPE return_datatype;

} SYSCALL;
*/

/* LAST VALUE IN EACH argument_datatypes[] MEMBER MUST BE NIL... */

SYSCALL SYSCALLS[] =
{
	//Windows 7 x64 user32 syscalls.

	{ ((DWORD)0x12F5), { NIL }, _BOOL },
	{ ((DWORD)0x12D4), { _VOID_PTR, _VOID_PTR, _VOID_PTR, _HANDLE, NIL }, _BOOL },
	
	// End of Windows 7 x64 gdi32 syscalls.
};



#endif /* BUGHUNT_SYSCALLS_H... */


