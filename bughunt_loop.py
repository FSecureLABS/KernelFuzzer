from subprocess import call, TimeoutExpired
import fnmatch
import sys
import shutil
import os
import time

# Current timeout set to 25 minutes, i.e. 25*60 seconds.
#TIMEOUT=1500
TIMEOUT=600
THREADS=1
# Current number of executions is 1M.
#EXECUTIONS=1000000
EXECUTIONS=350000
SEED=1

COMMAND='bughunt.exe'

# Change working directory to where this wrapper is located.
wrapper_path = os.path.abspath(__file__)
wrapper_path_parent = os.path.dirname(wrapper_path)
os.chdir(wrapper_path_parent)

# Folder name where to preserve any logs and dumps.
#folder = "crashes/%s" % str(tstamp)
folder = "to-be-populated"
crash_found = False

#try:
#        os.makedirs(folder, 777)
#except:
#        print("Error, cannot create folder structure.")
#        pass

# Look for memory dumps and move them to a new folder
for r, d, filenames in os.walk('C:/Dumps/'):
        for filename in fnmatch.filter(filenames, '*.dmp'):

                try:
                        print("Crash found!")
                        try:
                                print("Creating folder...")
                                tstamp = time.time()
                                folder = "crashes/%s" % str(tstamp)
                                os.makedirs(folder, 777)
                        except:
                                print("Error, cannot create folder structure.")
                                pass

                        # Currently copying the memory dump as opposed to just moving it.
                        #shutil.copyfile("C:/Dumps/%s" % filename, "%s/%s" % (folder, filename))
                        # Move instead of copying.
                        print("Moving memory dump to new folder...")
                        os.rename("C:/Dumps/%s" % filename, "%s/%s" % (folder, filename))
                        
                        # Process the memory dump using kd_batch_commands.txt.
                        print("Analysing memory dump... %s" % filename)
                        #kd_log = open("%s/%s.log" % (folder, filename.split('.')[0]),"wb")
                        kd_log = open("%s/windbg.log" % folder, "wb")
                        call(["C:\\Program Files\\Debugging Tools for Windows (x64)\\kd.exe", "-z", "%s\%s" % (folder, filename), "-c", "$$<crash_processing\\kd_batch_commands.txt;Q"], stdout=kd_log)
                        kd_log.close()
                        crash_found = True
                except:
                        print("Error, cannot process memory dump.")
                        pass

if crash_found:
        # Preserve log file(s) left before BSODs and move to same folder.
        for r, d, filenames in os.walk('.'):
                for filename in fnmatch.filter(filenames, 'log.*'):

                        try:
                                os.rename("./%s" % filename, "%s/%s" % (folder, filename))
                        except:
                                print("Error, cannot move log file.")
                                pass

        # Submit crash information to our CouchDB instance.

        try:
                # Invoke couchdb_submit.py with the correct arguments.
                call(["C:\Python35\python.exe", "crash_processing\\couchdb_submit.py", "--server", "IPADDRESS", "--database", "DBNAME", "--username", "USER", "--password", "PW", "add-crash", "--crash-path", "%s" % folder])
        except:
                print("Error, cannot submit crash information to database.")
                raise

try:
	# Run the fuzzer with the specified timeout.
        call(['%s' % COMMAND, '%s' % THREADS, '%s' % EXECUTIONS, '%s' % SEED], timeout=TIMEOUT)
        # We need a way of querying the CPU load while doing a long, e.g. 25 minute run. If the load is below 90% for more than 2-3 minute, we assume our process is dead. Should that be the case, reboot. Sound like 'psutils' is the module we need for the performance check.

except TimeoutExpired:
        print("Timeout of %d seconds expired.\n" % TIMEOUT)

except:
	# Process other exceptions accordingly, to be implemented.
	pass
'''
for i in range(10):
        try:
                call(['%s' % COMMAND, '%s' % THREADS, '%s' % EXECUTIONS, '%s' % SEED], timeout=TIMEOUT)
        except:
                pass
        call(['taskkill', '/f', '/im', 'notepad.exe'])
        call(['del', '*.txt'], shell=True)
        call(['del', 'log.*'], shell=True)
'''

# Kill all instances of Notepad.
call(['taskkill', '/f', '/im', 'notepad.exe'])

# Clean-up any temporary files logs left.
call(['del', '*.txt'], shell=True)
call(['del', 'log.*'], shell=True)

# Reboot and start fresh.
call(['shutdown', '-r'])
