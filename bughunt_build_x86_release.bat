@echo off
echo:
echo [Build] Setting up environment variables...
echo:
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" >nul 2>&1
echo:
echo [Build] Removing old files...
echo:
del *.log >nul 2>&1
del *.obj >nul 2>&1
del *.exe >nul 2>&1
echo:
echo [Build] Assembling object file...
echo:
ml.exe /c /Cx bughunt_syscall.asm >nul 2>&1
echo:
echo [Build] Final compilation and linking...
echo:
cl.exe /Tc bughunt.c gdi32.lib kernel32.lib User32.lib Advapi32.lib Shell32.lib Msimg32.lib Dxva2.lib Mscms.lib bughunt_syscall.obj >nul 2>&1