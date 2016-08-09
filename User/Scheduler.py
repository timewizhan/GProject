
import pdb

class Scheduler:
	ACTION_STORE = 1
	ACTION_LOAD = 2

	def __init__(self, userID, fileName, action, listCloud):
		self.userID = userID
		self.fileName = fileName
		self.action = action
		self.listCloud = listCloud

	def start(self):
		Log.debug("Start to scheduler")

		executeTaskByAction()


	def executeTaskByAction(self):
		BROKER_IP = ""
		BROKER_PORT = 1

		if self.action == ACTION_STORE:

		elif self.action == ACTION_LOAD:
			# broker connection
			broker = Network(BROKER_IP, BROKER_PORT)
			ret = network.connectToServer()
			if ret == 0:
				Log.error("Fail to connect Broker server")
				return

			# make a json
			jsonGenerator = JsonGenerator()
			jsonGenerator.appendElement("LOC", userPlace)
			
			network.sendDataToServer(jsonGenerator.toString())
			recvData = network.recvDataFromServer()

			del jsonGenerator

			# cloud connection
			clouds = []
			for cloudNumber in len(self.listCloud):
				cloudObj = self.listCloud[cloudNumber]
				clouds.append(Network(cloudObj.getIPAddr(), cloudObj.getPort()))
				ret = clouds[cloudNumber].connectToServer()
				if ret == 0:
					Log.error("Fail to connect Clouds")
					return


			for cloudNumber in len(clouds):
				clouds[cloudNumber].sendDataToServer(makeJsonData())
				clouds[cloudNumber].recvDataFromServer()


		else:
			Log.error("Invalid action type")

	def makeJsonData(self):

