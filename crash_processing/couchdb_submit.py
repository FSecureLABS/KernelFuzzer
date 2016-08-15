import couchdb
from couchdb.design import ViewDefinition
import sys
import shutil
import argparse
import os
import random
import hashlib

# TODO: Tidy this scipt up!

class CouchDBlib:
	
	def __init__(self,url,dbname,username,password):
		self.couch = couchdb.Server(url)
		self.couch.resource.credentials = (username, password) 
		self.db = self.couch[dbname]
		self.create_views()

	def create_views(self):
		print("++ Creating views")
		view = ViewDefinition('get_crashes', 'all', '''function(doc) {
			emit(doc.crashhash,doc.crashcount);
		}''')
		view.get_doc(self.db)
		view.sync(self.db)

	def zip_crashdata(self,crashpath,fn):
		print("++ Zipping ", crashpath)
		output_filename = crashpath + "/../" + fn
		shutil.make_archive(output_filename, 'zip', crashpath)
		return output_filename

	def add_crash_to_db_windbg(self,crashhash,path):

		# Generate a FN for the crash
		# Change to a timestamp-based ID.
		fn = str(random.randint(0, 0xffffffff))

		# First zip up the crash directory. 
		self.zip_crashdata(path,fn)

		# Read the testcases for uploading
		testcase_zip = path + "\\../" + fn + ".zip"
		testcase = open(testcase_zip,"rb").read()
		

		# Change this so the log filename doesn't have to be hardcoded. 
		debug_log_path = os.path.join(path +"\\windbg.log")
		# Open the log file and pull useful information.
		debug_log = open(debug_log_path).read()

		startstack = debug_log.find("STACK_TEXT:")
		endstack = debug_log.find("THREAD_SHA1_HASH_MOD_FUNC:")
		parserstack = debug_log[startstack:endstack]
		parserstack = parserstack.encode('utf-8')

		crashhash = hashlib.sha224(parserstack).hexdigest()
		
		startip = debug_log.find("FAULTING_IP: ")
		endip = debug_log.find("CONTEXT:  ")
		parserip = debug_log[startip:endip]
		#parserip = unicode(parserip, 'utf-8')

		startreg = debug_log.find("CONTEXT:  ")
		endreg = debug_log.find("efl=")
		parserreg = debug_log[startreg:endreg]
		#parserreg = unicode(parserreg, 'utf-8')

		if self.is_crash_in_db(crashhash):
			print("++ Crash already in DB skipping!")
		else:
			print("++ Adding new crash to DB ++")
			doc = {
				'crashhash' 	: str(crashhash),
				'faultingip' 	: str(parserip),
				'callstack' 	: str(parserstack),
				'registers'		: str(parserreg),
				'crashcount' 	: 1,
				'analyzed'		: False,
				'reduced'		: False
			}

			self.db.save(doc)
			self.db.put_attachment(doc,testcase, filename=fn+".zip") 	
			self.db.put_attachment(doc,debug_log,filename=fn+".log") 	# crash log

		# Cleanup the zip file data
		os.remove(testcase_zip)

	def add_crash_to_db(self,crashhash,faultingip,stackframe,testcase_path,debug_log_path,fn):
		self.zip_crashdata(testcase_path)
		testcase = open(testcase_path + "\\testcases.zip","rb").read()
		debug_log = open(debug_log_path).read()

		# First determine if the crash is already in DB.
		if self.is_crash_in_db(crashhash):
			print("++ Crash already in DB skipping!")
		else:
			print("++ Adding new crash to DB ++")
			doc = {
				'crashhash' 	: crashhash,
				'faultingip' 	: faultingip,
				'callstack' 	: stackframe,
				'crashcount' 	: 1,
				'analyzed'		: False,
				'reduced'		: False
			}
			self.db.save(doc)
			self.db.put_attachment(doc,testcase, filename=fn+".zip") 	
			self.db.put_attachment(doc,debug_log,filename=fn+".log") 	


	def is_crash_in_db(self,crashhash):
		crashes = self.db.view('get_crashes/all')
		
		found = False
		for crash in crashes:
			if crash.key == crashhash:
				print("++ Crash hash already in DB")
				self.increment_crash_count(crash.id)
				found = True
				return found
		return found


	def increment_crash_count(self,id):
		doc = self.db[id]
		doc['crashcount'] = int(doc['crashcount']) + 1

		self.db.save(doc)



if __name__ == "__main__":
	parser = argparse.ArgumentParser(description='Handle couchdb arguments')
	parser.add_argument("--server",help='Couchdb Server Address')
	parser.add_argument("--databasename",help='Couchdb Database Name')
	parser.add_argument("--username",help='Couchdb Username')
	parser.add_argument("--password",help='Couchdb Password')

	subparsers = parser.add_subparsers(dest='mode')
	addcrash_parser = subparsers.add_parser('add-crash',help='Call to add crash to database')
	addcrash_parser.add_argument("--crash-path",help='The path to the crash to store')
	addcrash_parser.add_argument("--crash-hash",help='The hash of the crash to store')

	idcrash_parser = subparsers.add_parser('id-crash',help="Call to id a crash")
	idcrash_parser.add_argument("--crash-hash",help="The hash of the crash to identify")

	args = parser.parse_args()

	cdblib = CouchDBlib(args.server, args.databasename, args.username,args.password)

	if (args.mode == "id-crash"):
		print("++ Identifying crash")
	else:
		print("++ Adding crash")
		print(args.crash_hash)
		print(args.crash_path)

		# Get the crash path from the logfile path
		cdblib.add_crash_to_db_windbg(args.crash_hash, args.crash_path)
