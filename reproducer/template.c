#include "..\logger.h"
#include "..\bughunt_thread.h" //To avoid the pain that is our dependency hell. Fix this!

/* I know it's filthy but we can currently build this as follows.

cl.exe /w /Zi /Tc template.c gdi32.lib kernel32.lib User32.lib Advapi32.lib Shell32.lib Msimg32.lib Dxva2.lib Mscms.lib ..\bughunt_syscall_x64.obj

*/

//Windows specific files required for this to work.
// These may not be needed.
#include <Icm.h>
#include <Wingdi.h>

static unsigned int syscall_count;

int main(int argc, char* argv[])
{
	// Change the following and other variables to plain integers, no one really needs this sloppy DWORDs and all the casting.
	//DWORD exit_code = ((DWORD)0xDEADBEEF);

	make_HANDLES(); //We pray and hope that this works correctly. If it doesn't... well, tough.
	//From here, we paste the log file in. This should be semi valid C... Hopefully!















	
	//return (exit_code);
	return 0;
}