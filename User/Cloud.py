
class Cloud:

	def __init__(self, number, IPAddr, Port, latency):
		self.number = number
		self.IPAddr = IPAddr
		self.Port = Port
		self.latency = latency

	def getNumber(self):
		return self.number

	def getIPAddr(self):
		return self.IPAddr

	def getPort(self):
		return self.Port

	def getLatency(self):
		return self.latency