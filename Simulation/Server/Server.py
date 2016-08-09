import sys, os
import pdb
from socket import *
from select import select
from Log import *
from Recorder import *

class JsonParser:
	def __init__(self, jsonData):
		self.jsonData = jsonData

	def getJsonObject(self):
		return json.loads(self.jsonData)

	def getValue(self, key):
		jsonObject = self.getJsonObject()
		return jsonObject[key]


class Server:
	HOST = '127.0.0.1'
	PORT = 56789
	BUFSIZE = 1024
	ADDR = (HOST, PORT)

	def __init__(self):
		self.serverSocket = socket(AF_INET, SOCK_STREAM)
		self.serverSocket.bind(ADDR)
		self.serverSocket.listen(10)

	def __del__(self):
		self.serverSocket.close()


	def acceptFromClient(self):
    	clientSocket, addrInfo = self.clientSocket.accept()
    	print "Client [%s]" % addrInfo[0]
		

    def sendDataToServer(self, sendData):
    	return self.clientSocket.send(sendData)

    def recvDataFromServer(self):
    	connection_list = [self.serverSocket]
    	BUFSIZE = 4096

    	recvSize = 0
    	while True:
	        read_socket, write_socket, error_socket = select(connection_list, [], [], 10)

	        for sock in read_socket:
	            if sock == clientSocket:
	                data = sock.recv(BUFSIZE)
	                if not data:
	                    clientSocket.close()
	                    return data


if __name__ == "__main__":

	Log("test")

	Log.debug("=============================================")
	Log.debug("================= Start Server =================")
	Log.debug("=============================================")

	try:
		server = Server()
		server.acceptFromClient()

		record = CRecorder()
		record.startRecord()

		Log.debug("Receive data from client")
		recvData = server.recvDataFromServer()

		jsonParser 	= JsonParser(recvData)
		size 		= jsonParser.getValue("size")

		const_filepath = "C:\\test\\test"
		if size == os.path.getsize(const_filepath):
			print "Same size : %s" % size
		else
			print "Not same size : %s" % os.path.getsize(const_filepath)
			return

		Log.debug("Send data to server")	
		f = open(const_filepath, 'r')

		TOTAL_READ_SIZE = 4096

		currentSize = size
		while currentSize < 1:
			readData = f.read(TOTAL_READ_SIZE)
			client.sendDataToServer(readData)
			currentSize -= TOTAL_READ_SIZE

		f.close()

		Log.debug("Success to send all data to client")
		record.endRecord()

		responseTime = Recorder.gerResultTime()
		Log.debug("RESPONSE TIME : " + str(responseTime))

		del server

	except Exception as e:
		Log.error("Fail to operate user")
		
		Log.debug("=========================================================")
		Log.debug("================= Abnormally Finish Server =================")
		Log.debug("=========================================================")
		sys.exit(1)

	Log.debug("==============================================")
	Log.debug("================= Finish Server =================")
	Log.debug("==============================================")	
	sys.exit(1)
