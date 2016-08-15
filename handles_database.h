#ifndef HANDLES_DATABASE_H
#define HANDLES_DATABASE_H

/*
 * CRUDE HANDLE DATABASE.
 */

// When updating either sizes, remember to adjust the other one.
#define HANDLES_N 128 // Updated value, old and original value was 32.
#define HANDLE_CREATOR_SIZE 128

HANDLE HANDLES[HANDLES_N];
// The handle creator is subject to removal in the future, don't need it anymore.
char* HANDLE_CREATOR[HANDLE_CREATOR_SIZE]; //When debugging, we need to know what created the handle. This will hold the array in the same order of HANDLES of the handle creator. 

// This tells us the first available, i.e. empoty slot in the array of handles.
// This will initially be set to wherever make_HANDLES stops populating. It will increment till the end of the array and then set back to zero to start overwriting from the begging of the array.
unsigned int HANDLES_ARRAY_AVAILABLE_SLOT_INDEX;
// Do we have empy slots in the array of handles.
int HANDLE_ARRAY_FULLY_POPULATED = 0;


/* FUNCTION PROTOTYPES... */


BH_Handle get_random_HANDLE (void);
HANDLE get_specific_HANDLE(int n);
HANDLE put_random_HANDLE (HANDLE handle, char* HandleCreator);
void dump_HANDLES (void);
void make_HANDLES (void);

// Helpers
HWND OpenNotepad(void);


BH_Handle get_random_HANDLE (void)
{

	BH_Handle temp_handle;
	unsigned int n;

	if (HANDLE_ARRAY_FULLY_POPULATED) {
	 	n = sizeof (HANDLES) / sizeof (HANDLES[0]);
	 	n = rand() % n;
	}
	else {
    	n = rand() % HANDLES_ARRAY_AVAILABLE_SLOT_INDEX;
    }
    
    temp_handle.index = n;
    temp_handle.value = HANDLES[n];

	logger("//[Handler_Function]: get_random_HANDLE : n = %u, HANDLES[n] = 0x%08x, HANDLE_CREATOR[n] = %s", n, HANDLES[n], HANDLE_CREATOR[n]);
    return (temp_handle);
}

HANDLE get_specific_HANDLE(int n)
{
	// This is for pulling a handle by its index.
	// Providing a negative index in the array as a means of returning NULL.
	if (n < 0) {
		return NULL;
	}
	else {
		return (HANDLES[n]);
	}
}

HANDLE put_random_HANDLE (HANDLE handle, char *HandleCreator)
{
	// Check if the handle is valid, discard if it isn't.
	if (handle == NULL || handle == -1 || handle == INVALID_HANDLE_VALUE) {
		// Simply return.
		logger("//[Handler_Function]: put_random_HANDLE : Ignoring invalid handle.");
		// Should probably also check against values less than 33.
		// https://cxsecurity.com/issue/WLB-2015090152
		return 0;
	}
	else {
		// Check if we have reached the end of the array and if so, set the first available slot back to zero.
		if (HANDLES_ARRAY_AVAILABLE_SLOT_INDEX + 1 == HANDLES_N) {
			// Reset the available slot index to zero, i.e. start overwriting the handles in the array.
			HANDLES_ARRAY_AVAILABLE_SLOT_INDEX = 0;
			// Make sure we know that we have fully populated the array, this is used by get_random_HANDLE().
			HANDLE_ARRAY_FULLY_POPULATED = 1;

		}

	    HANDLES[HANDLES_ARRAY_AVAILABLE_SLOT_INDEX] = handle;
		HANDLE_CREATOR[HANDLES_ARRAY_AVAILABLE_SLOT_INDEX] = HandleCreator;

	    /* FOR GOOD MEASURE, RETURN THE HANDLE AS WELL - IT DOESN'T HAVE TO BE USED... */
	    return (HANDLES[HANDLES_ARRAY_AVAILABLE_SLOT_INDEX++]);
	}
}

void dump_HANDLES (void) {
	// This is only for debugging purposes.
	int n;
	for (n = 0; n < HANDLES_N; n = n + 1) {
		printf("//[Handler_Function]: dump_HANDLES: n = %d, HANDLES[n] = 0x%08x\n", n, HANDLES[n]);
	}
}

void make_HANDLES (void)
{
	// Improve the code as we see certain functions failing every time, which means we're not calling them the right way.
	// Diversify the handles!

	unsigned int handle_idx = 0;
	const POINT ptZero = { 0, 0 }; //to get a handle to the primary monitor

    BITMAP bmp = { 0, 8, 8, 2, 1, 1 };
    BYTE bits [8][2] = { 0xFF, 0, 0x0C, 0, 0x0C, 0, 0x0C, 0,
    					 0xFF, 0, 0xC0, 0, 0xC0, 0, 0xC0, 0 };

	HKEY keyCurrentUser;

	HANDLE tempHandle;
	unsigned int tempUINT1, tempUINT2;

	INT NumberOfNotepadHandles = 0;

	tempUINT1 = 0;
	tempUINT2 = 0;


	// Initialise the array of handles by setting every handle to 0.
	for (handle_idx = 0; handle_idx < HANDLES_N; handle_idx += 1) {
		HANDLES[handle_idx] = 0x0000000000000000;
	}

	// Populate each one of the handle slots sequentially.
	//for (handle_idx = 0; handle_idx < HANDLES_N; handle_idx += 1) {
	for (handle_idx = 0; handle_idx < 64; handle_idx += 1) {
		
		while(HANDLES[handle_idx] == 0x0000000000000000) {
			
			if (!tempUINT1) {
				tempHandle = GetDesktopWindow();
				if (tempHandle == NULL || tempHandle == -1 || tempHandle == INVALID_HANDLE_VALUE) {
					logger("//[Handler_Function]: make_HANDLES : Ignoring invalid handle.");
				}
				else {
					logger("//[Handler_Function]: make_HANDLES : n = %u, handle = 0x%08X, HANDLE_CREATOR[n] = %s", handle_idx, tempHandle, "GetDesktopWindow");
					HANDLES[handle_idx] = tempHandle;
					HANDLE_CREATOR[handle_idx] = "GetDesktopWindow";
					tempHandle = -1;
					tempUINT1 = 1;
					break;
				}
			}
			if (!tempUINT2) {
				tempHandle = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
				if (tempHandle == NULL || tempHandle == -1 || tempHandle == INVALID_HANDLE_VALUE) {
					logger("//[Handler_Function]: make_HANDLES : Ignoring invalid handle.");
				}
				else {
					logger("//[Handler_Function]: make_HANDLES : n = %u, handle = 0x%08X, HANDLE_CREATOR[n] = %s", handle_idx, tempHandle, "MonitorFromPoint");
					HANDLES[handle_idx] = tempHandle;
					HANDLE_CREATOR[handle_idx] = "MonitorFromPoint";
					tempHandle = -1;
					tempUINT2 = 1;
					break;
				}
			}

			switch(rand() % 8) {
				case 0:
					tempHandle = CreateFile(TEXT("C:\\boot.ini"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (tempHandle == NULL || tempHandle == -1 || tempHandle == INVALID_HANDLE_VALUE) {
						logger("//[Handler_Function]: make_HANDLES : Ignoring invalid handle.");
					}
					else {
						logger("//[Handler_Function]: make_HANDLES : n = %u, handle = 0x%08X, HANDLE_CREATOR[n] = %s", handle_idx, tempHandle, "CreateFile");
						HANDLES[handle_idx] = tempHandle;
						HANDLE_CREATOR[handle_idx] = "CreateFile";
						tempHandle = -1;
					}
					break;
				case 1:
					tempHandle = CreateSolidBrush(RGB(0, 255, 0));
					if (tempHandle == NULL || tempHandle == -1 || tempHandle == INVALID_HANDLE_VALUE) {
						logger("//[Handler_Function]: make_HANDLES : Ignoring invalid handle.");
					}
					else {
						logger("//[Handler_Function]: make_HANDLES : n = %u, handle = 0x%08X, HANDLE_CREATOR[n] = %s", handle_idx, tempHandle, "CreateSolidBrush");
						HANDLES[handle_idx] = tempHandle;
						HANDLE_CREATOR[handle_idx] = "CreateSolidBrush";
						tempHandle = -1;
					}
					break;
				case 2:
					tempHandle = FindWindow(NULL, TEXT("Explorer"));
					if (tempHandle == NULL || tempHandle == -1 || tempHandle == INVALID_HANDLE_VALUE) {
						logger("//[Handler_Function]: make_HANDLES : Ignoring invalid handle.");
					}
					else {
						logger("//[Handler_Function]: make_HANDLES : n = %u, handle = 0x%08X, HANDLE_CREATOR[n] = %s", handle_idx, tempHandle, "FindWindow");
						HANDLES[handle_idx] = tempHandle;
						HANDLE_CREATOR[handle_idx] = "FindWindow";
						tempHandle = -1;
					}
					break;
				case 3:
					tempHandle = CreateFont(46, 28, 215, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Times New Roman");
					if (tempHandle == NULL || tempHandle == -1 || tempHandle == INVALID_HANDLE_VALUE) {
						logger("//[Handler_Function]: make_HANDLES : Ignoring invalid handle.");
					}
					else {
						logger("//[Handler_Function]: make_HANDLES : n = %u, handle = 0x%08X, HANDLE_CREATOR[n] = %s", handle_idx, tempHandle, "CreateFont");
						HANDLES[handle_idx] = tempHandle;
						HANDLE_CREATOR[handle_idx] = "CreateFont";
						tempHandle = -1;
					}
					break;
				case 4:
					tempHandle = CreateBitmapIndirect(&bmp);
					if (tempHandle == NULL || tempHandle == -1 || tempHandle == INVALID_HANDLE_VALUE) {
						logger("//[Handler_Function]: make_HANDLES : Ignoring invalid handle.");
					}
					else {
						logger("//[Handler_Function]: make_HANDLES : n = %u, handle = 0x%08X, HANDLE_CREATOR[n] = %s", handle_idx, tempHandle, "CreateBitmapIndirect");
						HANDLES[handle_idx] = tempHandle;
						HANDLE_CREATOR[handle_idx] = "CreateBitmapIndirect";
						tempHandle = -1;
					}
					break;
				case 5:
					tempHandle = GlobalAlloc(GMEM_FIXED, 10);
					if (tempHandle == NULL || tempHandle == -1 || tempHandle == INVALID_HANDLE_VALUE) {
						logger("//[Handler_Function]: make_HANDLES : Ignoring invalid handle.");
					}
					else {
						logger("//[Handler_Function]: make_HANDLES : n = %u, handle = 0x%08X, HANDLE_CREATOR[n] = %s", handle_idx, tempHandle, "GlobalAlloc");
						HANDLES[handle_idx] = tempHandle;
						HANDLE_CREATOR[handle_idx] = "GlobalAlloc";
						tempHandle = -1;
					}
					break;
				case 6:
					RegOpenCurrentUser(KEY_READ, &tempHandle);
					if (tempHandle == NULL || tempHandle == -1 || tempHandle == INVALID_HANDLE_VALUE) {
						logger("//[Handler_Function]: make_HANDLES : Ignoring invalid handle.");
					}
					else {
						logger("//[Handler_Function]: make_HANDLES : n = %u, handle = 0x%08X, HANDLE_CREATOR[n] = %s", handle_idx, tempHandle, "RegOpenCurrentUser");
						HANDLES[handle_idx] = tempHandle;
						HANDLE_CREATOR[handle_idx] = "RegOpenCurrentUser";
						tempHandle = -1;
					}
					break;
				case 7:
					tempHandle = OpenNotepad();
					if (tempHandle == NULL || tempHandle == -1 || tempHandle == INVALID_HANDLE_VALUE) {
						logger("//[Handler_Function]: make_HANDLES : Ignoring invalid handle.");
					}
					else {
						logger("//[Handler_Function]: make_HANDLES : n = %u, handle = 0x%08X, HANDLE_CREATOR[n] = %s", handle_idx, tempHandle, "OpenNotepad");
						HANDLES[handle_idx] = tempHandle;
						HANDLE_CREATOR[handle_idx] = "OpenNotepad";
						tempHandle = -1;
					} // if
					break;
			} // switch
		} // while
	} // for

	// Alternatively, increment every iteration of the for loop.
	// Array has been populated from 0 to 64, i.e. first available slot is 64.
	HANDLES_ARRAY_AVAILABLE_SLOT_INDEX = 64;


}

/* HELPER FUNCTIONS */

HWND OpenNotepad(void) {


    char filename[1024];
    FILE *f;
    char buf[1024];
    HANDLE tempHandle;
    int tempINT;
    tempINT = rand() % 1000;

    // Generate a random filename.
    sprintf_s(filename, 1023, "f%d.txt", tempINT);

    // Create file.
    fopen_s(&f, filename, "w");
    fclose(f);

    // Open file in notepad.
    tempHandle = ShellExecuteA(0, "open", "notepad.exe", filename, 0, SW_SHOW);
    
    if (tempHandle < 33) {
    	logger("//[Handler Function]: OpenNotepad: Failed to start notepad.");
    	return 0;
    }

    sprintf_s(buf, 1024, "f%d - Notepad", tempINT);


    //Dirty++
	while(1) {
		tempHandle = FindWindow(0, buf);
		if (tempHandle) {
			return tempHandle;
		}

		logger("//[Handler Function]: OpenNotepad: Cannot find handle.");
	}
    return 0;
}


#endif /* HANDLES_DATABASE_H */
