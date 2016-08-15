#ifndef LIBRARY_CALLS_H
#define LIBRARY_CALLS_H

#include "helpers.h"
// Include the actual implementations.
#include "library_calls/brush.h"



// Include any Windows header files in the respective file instead of adding it here.

// Function Prototypes
// grep '^VOID BH_' library_calls.h |grep '{$'|awk '{gsub(" {", ";");print $0}'

// Blacklisted Functions
// BH_SetMapperFlags, BH_LockWindowUpdate, BH_GetMessage, BH_GetCountColorProfileElements, BH_DeleteColorTransform, BH_GetCMMInfo, BH_CreateWindowStation, BH_CreateBrushIndirect, BH_SetClipboardData, BH_SwitchDesktop, BH_LineDDA, BH_CloseColorProfile, BH_WaitForSingleObject, BH_DrawEdge, BH_GetSystemPaletteEntries, BH_DispatchMessage, BH_WcsCreateIccProfile, BH_DrawText, BH_ExtTextOut

void(*LIBRARY_CALLS[]) () = { BH_GetSysColorBrush
							   };

/* Moved here from bughunt.h to get around compilation error. */
void (*random_LIBRARY_CALL()) (void)
{
    unsigned int n = sizeof (LIBRARY_CALLS) / sizeof (LIBRARY_CALLS[0]);
    return LIBRARY_CALLS[rand() % n];
}

// Stuff to Remember and Handy Tips
// When calling a function, pass a handle by its value. When logging, log by its index in the handles database.
// Consider changing the prototypes of the functions so they all take a single VID argument. VID will be the string to append to the variable names. That way, we'll populate VID only once in the code in the bughunt_thread loop, instead of having it initialised within every single function.

#endif
