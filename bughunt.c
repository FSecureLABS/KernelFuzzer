#include "logger.h"
#include "bughunt_thread.h"

#define MAX_THREADS 64

// Moved this in the global scope.
// Number of system calls to execute as part of each thread.
static unsigned int syscall_count;

int main (int argc, char* argv[])
{
    // Number of threads to create.
    unsigned int subprocess_count = 0;
    
    // Thread index.
    unsigned int subprocess_idx = 0;

    // Thread data structures.
    unsigned int dwThreadIdArray[MAX_THREADS];
    HANDLE hThreadArray[MAX_THREADS];

    // PRNG Seed.
	unsigned int seed = NULL;
    unsigned int control_seed = NULL;

    if (argc != 4)
    {
        printf ("USAGE : %s <subprocess_count> <syscall_count> <seed>\n", argv[0]);
        printf ("WHERE : subprocess_count   : is a base 10 DWORD\n");
        printf ("        syscall_count      : is a base 10 DWORD\n");
		printf ("        seed               : Seed to use. Use 1 if you want to create a new seed.\n");

        return (0xDEADBEEF); // Error.
    } 
	
    // Populate our object store, i.e. handles database in the Windows case.
    make_HANDLES();

    subprocess_count = strtol (argv[1], NULL, 10);
    syscall_count = strtol (argv[2], NULL, 10);
	seed = strtol (argv[3], NULL, 10);

    // Also check if syscall_count is less then MAX_THREADS.
    for (subprocess_idx = 0; subprocess_idx < subprocess_count; subprocess_idx += 1)
    {

        hThreadArray[subprocess_idx] = CreateThread(NULL,                   // default security attributes
                                       0,                      // use default stack size  
                                       (LPTHREAD_START_ROUTINE) bughunt_thread,         // thread function name
                                       seed,          // argument to thread function 
                                       0,                      // use default creation flags 
                                       &dwThreadIdArray[subprocess_idx]);   // returns the thread identifier
        
        // Check if thread was successfully created. Bail out should we fail to create a new thread.
        if (hThreadArray[subprocess_idx] == NULL) 
        {
            printf ("Error creating thread, exiting.");
            return (0xDEADBEEF); // Error.
        }
    } // For loop.
            
    // Comment out the lines above and uncomment the following one for no threads.
    //bughunt_thread(syscall_count, seed);

    // Wait until all threads have terminated.
    WaitForMultipleObjects(subprocess_count, hThreadArray, TRUE, INFINITE);
    
    // Close all thread handles and free memory allocations.
    for(subprocess_idx = 0; subprocess_idx < subprocess_count; subprocess_idx += 1)
    {
        CloseHandle(hThreadArray[subprocess_idx]);
    }
    return (0);
}