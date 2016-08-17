#ifndef HELPERS_H
#define HELPERS_H

#include <Shellapi.h> // ShellExecuteA();
#include <Wingdi.h>
#include <Winuser.h>

/* Prototypes */
// Helpers returning various structs.
MENUITEMINFO get_MENUITEMINFO();
// This is used for getting current time in milliseconds. The function is used for appending unique identifiers to variable names.
//WORD get_time_in_ms(void);
// Various callbacks.


/* Implementations */


POINT get_POINT(char *vid) {

    POINT p;

    logger("POINT p%s;", vid);

    /*
    typedef struct tagPOINT {
        LONG x;
        LONG y;
    } POINT, *PPOINT;
    */

    p.x = get_fuzzed_int32();
    p.y = get_fuzzed_int32();

    logger("p%s.x = %d;", vid, p.x);
    logger("p%s.y = %d;", vid, p.y);

    logger("//[Helper Function] get_POINT");

    return p;
}

WORD get_time_in_ms(void) {
	SYSTEMTIME time;
	WORD millis;
	GetSystemTime(&time);

	millis = (time.wSecond * 1000) + time.wMilliseconds;
	return millis;
}

#endif /* HELPERS_H */