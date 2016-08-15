import sys
import subprocess
import os

from ctypes import windll

# This script prepares a worker for fuzzing.

# Execute as administrator.

def install_windbg():
	print("[!] Installing WinDbg...")
	subprocess.call(["C:\\Windows\\System32\\msiexec.exe", "/i", "dbg_amd64_6.12.2.633.msi", "/qn"])

def install_couch_module():
	print("[!] Installing CouchDB Python module...")
	try:
		# Quick and dirty, one of these two should work.
		subprocess.call(["C:\\Python35\\Scripts\\pip.exe", "install", "couchdb"])
		subprocess.call(["C:\\Python35\\python.exe", "-m", "pip", "install", "couchdb"])
	except:
		pass

def change_registry():
	print("[!] Disabling UAC...")
	subprocess.call(["C:\\Windows\\System32\\reg.exe", "import", "disable_uac.reg"])
	print("[!] Disabling Lock Screen...")
	subprocess.call(["C:\\Windows\\System32\\reg.exe", "import", "disable_lockscreen.reg"])
	print("[!] Disabling Windows Error Reporting...")
	subprocess.call(["C:\\Windows\\System32\\reg.exe", "import", "disable_windows_error_reporting.reg"])
	print("[!] Disabling Windows Updates...")
	subprocess.call(["C:\\Windows\\System32\\reg.exe", "import", "disable_windows_updates.reg"])

def enable_kernel_dumps():	
	print("[!] Enabling Small Memory Dumps...")

	dump_dir = "C:\\Dumps"
	#dump_path = dump_dir + "\\MEMORY.DMP"

	# Create dump dir.
	if not os.path.exists(dump_dir):
		os.mkdir(dump_dir)

	# Set DebugInfoType to 3 for small memory dumps.
	# https://support.microsoft.com/en-gb/kb/307973
	#This does not get what we want, so we set to 2 for kernel dumps.
	subprocess.call(["wmic","recoveros","set","DebugInfoType=2"])

	# Set memory dump path.
	#subprocess.call(["wmic","recoveros","set",'DebugFilePath="' + dump_dir + '"'])
	# Set minidump path.	
	subprocess.call(["wmic","recoveros","set","MiniDumpDirectory=%s" % dump_dir])

def schedule_task():
	print("[!] Scheduling Task...")
	# We replace a shortcut to our wrapper in the Startup folder with a scheduled task.
	# C:\Users\munmap>schtasks /create /tn "Bug Hunter" /tr C:\Users\munmap\Desktop\bughunt_subprocess_types\bughunt_loop.py /sc onstart
	# SUCCESS: The scheduled task "Bug Hunter" has successfully been created.
	# 
	# C:\Users\munmap>

	#subprocess.call(["C:\\Windows\\System32\\schtasks", "/create", "/tn", "Bug Hunter", "/tr", "%s\\..\\bughunt_loop.py" % os.getcwd(), "/sc", "onstart"])
	subprocess.call(["C:\\Windows\\System32\\schtasks", "/create", "/tn", "Bug Hunter", "/tr", "%s\\..\\bughunt_loop.py" % os.getcwd(), "/sc", "onlogon"])

	# C:\Users\munmap>schtasks /delete /TN "Bug Hunter"
	# WARNING: Are you sure you want to remove the task "Bug Hunter" (Y/N)? Y
	# SUCCESS: The scheduled task "Bug Hunter" was successfully deleted.
	# 
	# C:\Users\munmap>schtasks

def enable_special_pool():
	print("[!] Enabling Special Pool...")
	# Enable special pool for win32k.sys.
	# https://msdn.microsoft.com/en-gb/library/windows/hardware/ff556083%28v=vs.85%29.aspx
	subprocess.call(["C:\\Windows\\System32\\verifier.exe", "/driver", "win32k.sys", "/flags", "0x00000001"])

def enable_kernel_dbg():
	# Enable kernel debugging.
	print("[!] Enabling Kernel Debugging...")
	subprocess.call(["C:\\Windows\\System32\\bcdedit","/debug","on"])

def reboot_system():
	print("[!] Rebooting in 5 seconds...")
	subprocess.call(['shutdown', '-r', '-t', '5'])

def main():

	#kdll = windll.LoadLibrary("kernel32.dll")
	
	if not windll.shell32.IsUserAnAdmin():
		print("This script must be ran as admin!")
		sys.exit(1)

	install_windbg()
	install_couch_module()
	change_registry()
	enable_kernel_dumps()
	schedule_task()
	enable_special_pool()
	#enable_kernel_dbg() # Not needed anymore.
	reboot_system()

	# Leaving an artefact to know this has been executed.
	# open(".setup",'a').close()

if __name__ == "__main__":
    main()
