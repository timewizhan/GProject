import sys, os
import pdb
from socket import *
from select import select
from Log import *
from Recorder import *

class JsonGenerator:
	def __init__(self):
		self.jsonDic = {}

	def appendElement(self, key, value):
		self.jsonDic[key] = value

	def toString(self):
		return json.dumps(self.jsonDic)


class Client:
	HOST = '127.0.0.1'
	PORT = 56789
	BUFSIZE = 1024
	ADDR = (HOST, PORT)

	def __init__(self):
		self.clientSocket = socket(AF_INET, SOCK_STREAM)

	def connectToServer(self):
		try:
    		self.clientSocket.connect(ADDR)
		except Exception as e:
    		Log.debug("Cannot connect to server")
    		sys.exit(1)

    def sendDataToServer(self, sendData):
    	self.clientSocket.send(sendData)

    def recvDataFromServer(self):
    	connection_list = [self.clientSocket]
    	BUFSIZE = 4096

    	recvSize = 0
    	while True:
	        read_socket, write_socket, error_socket = select(connection_list, [], [], 10)

	        for sock in read_socket:
	            if sock == clientSocket:
	                data = sock.recv(BUFSIZE)
	                if not data:
	                    self.clientSocket.close()
	                    break
	                else:
	                	recvSize += len(data)
	                    
	        

if __name__ == "__main__":

	Log("test")

	Log.debug("=============================================")
	Log.debug("================= Start Client =================")
	Log.debug("=============================================")

	try:
		client = Client()
		client.connectToServer()

		json = JsonGenerator()
		json.appendElement("size", 123)

		record = CRecorder()
		record.startRecord()

		Log.debug("Send data to server")
		client.sendDataToServer(json.toString())

		Log.debug("Receive data from server")
		recvLen = client.recvDataFromServer()
		if recvLen != 123:
			Log.error("Fail to receive total data : " + str(recvLen))

		record.endRecord()

		responseTime = Recorder.gerResultTime()
		Log.debug("RESPONSE TIME : " + str(responseTime))

	except Exception as e:
		Log.error("Fail to operate user")
		
		Log.debug("=========================================================")
		Log.debug("================= Abnormally Finish Client =================")
		Log.debug("=========================================================")
		sys.exit(1)

	Log.debug("==============================================")
	Log.debug("================= Finish Client =================")
	Log.debug("==============================================")	
	sys.exit(1)
