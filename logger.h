#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// Global variable to store the log filename.
char logfilename[128];
// Whether this filename has been set.
int flag = 0;

VOID logger(const char* fmt, ...)
{
	
	FILE *stream = NULL;

	va_list args;
	va_start(args, fmt);

	if (!flag) {
		// Set the log file name.
    	//snprintf(logfilename, 128, "log.%d.txt", time(NULL) + GetCurrentProcessId() + GetCurrentThreadId());
    	sprintf(logfilename, "log.%d.c", time(NULL) + GetCurrentProcessId() + GetCurrentThreadId());
		flag = 1;
	}

	// Open our log file.
	if ((stream = fopen(logfilename, "a+")) == NULL) {
            printf("Error! Cannot open log file, exiting...");
			exit(1);
	}

	// Write to file.
	if (vfprintf(stream, fmt, args) < 0) {
		printf("Error! Cannot write to log file, exiting...");
		exit(1);
	}
	fprintf(stream, "\n");

	// Flush content to log file and close handle.
	fflush(stream);
	fclose(stream);

	// Print to standard out as well.
	// Check for error conditions.
	if (vfprintf(stdout, fmt, args) < 0) {
		printf("Error! Cannot write to standard output, exiting...");
		exit(1);
	}
	fprintf(stdout, "\n");

	va_end(args);
}