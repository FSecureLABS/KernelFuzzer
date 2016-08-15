/* This is still highly experimental, not to mention logging isn't implemented. */

/* Prototypes */

LRESULT CALLBACK BH_HookCallback(int code, WPARAM wParam, LPARAM lParam);
HHOOK WINAPI BH_SetWindowsHookEx();
BOOL WINAPI BH_UnhookWindowsHookEx();

 /* VARIABLES */
 
 HHOOK hhk; // Variable used for preserving the hook, this is used to unhook after a function call.
 // Remember to declare this variable in the reproducer as either global or scoped to main.

/* Implementations */

// This is still under heavy development, expect problems.

// https://msdn.microsoft.com/en-gb/library/windows/desktop/ms644990%28v=vs.85%29.aspx
// https://github.com/sam-b/CVE-2014-4113/blob/master/Trigger/Trigger/Trigger.cpp


BOOL WINAPI BH_UnhookWindowsHookEx() {
    // Call with the global variable, current design allows only one hook to exist at a given moment.
    //logger("//[Callback] BH_UnhookWindowsHookEx");
	logger("UnhookWindowsHookEx(hhk);");
    UnhookWindowsHookEx(hhk);
    logger("hhk = NULL;");
    hhk = NULL;
}