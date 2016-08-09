import sys, os
import pdb
from Log import *
from Cloud import *

def getCurrentDir():
	return os.getcwd()

if __name__ == "__main__":
	# sample id
	if len(sys.argv) < 3:
		print "Usage : [UID] [FileName] [Action]"
		sys.exit(1)

	userID 		= sys.argv[1]
	fileName 	= sys.argv[2]
	action		= sys.argv[3]

	# Cloud Setting >> Cloud Object
	listCloud	= []
	listCloud.append(Cloud(1, "192.168.0.1", 1234, 1))
	listCloud.append(Cloud(2, "192.168.0.1", 1234, 2))
	listCloud.append(Cloud(3, "192.168.0.1", 1234, 3))

	Log(userID)

	Log.debug("=============================================")
	Log.debug("================= Start User =================")
	Log.debug("=============================================")

	try:
		schedulerForUser = Scheduler(userID, fileName, action, listCloud)
		if schedulerForUser == None:
			raise Exception

		schedulerForUser.start()
	except Exception as e:
		Log.error("Fail to operate user")
		
		Log.debug("=========================================================")
		Log.debug("================= Abnormally Finish User =================")
		Log.debug("=========================================================")
		sys.exit(1)

	Log.debug("==============================================")
	Log.debug("================= Finish User =================")
	Log.debug("==============================================")	
