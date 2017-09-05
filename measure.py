from datetime import datetime
import logging
import re
import serial
import time
import vesna.alh
from videkrestclient import Videk

log = logging.getLogger(__name__)

class SHT21(object):
	def __init__(self):
		f = serial.Serial("/dev/ttyS2", 115200, timeout=10)
		self.node = vesna.alh.ALHTerminal(f)

	def get(self):
		resp = self.node.get("sensors/sht21")

		f = resp.text.split()
		t = float(f[2])
		rh = float(f[6])

		return t, rh

class VidekError(Exception): pass

class MyVidekSensor(object):
	def __init__(self, myvidek, sensor_id):
		self.myvidek = myvidek
		self.sensor_id = sensor_id

	def upload(self, value):

		data = [
				{
					"value": value,
					"ts": datetime.utcnow().isoformat(),
					"latitude": 0.0,
					"longitude": 0.0
				}
		]

		self.myvidek.videk.uploadMesurements(data, self.myvidek.node_id, self.sensor_id)

class MyVidek(object):
	def __init__(self):
		endpoint = "https://videk.ijs.si"
		api_key = open('/etc/videk/api.key').readline().strip()

		log.debug("Videk: api_key  = %r" % (api_key,))

		self.videk = Videk(endpoint, api_key)

		self.hostname = open('/etc/videk/hostname').readline().strip()
		cluster = re.sub("-[0-9]+$", "", self.hostname)

		log.debug("Videk: hostname = %r" % (self.hostname,))
		log.debug("Videk: cluster  = %r" % (cluster,))

		self.cluster_id = self.videk.getClusterID(cluster)
		if self.cluster_id is None:
			raise VidekError("Can't get cluster ID")

		self.node_id = self.videk.getNodeID(self.hostname)
		if self.node_id is None:
			raise VidekError("Can't get node ID")

	def get_sensor(self, type, quantity, unit):
		sensor_id = self.videk.getSensorID(self.hostname, type, quantity)
		if sensor_id is None:
			self.videk.createSensor(self.node_id, type, quantity, unit)
			sensor_id = self.videk.getSensorID(self.hostname, type, quantity)

		return MyVidekSensor(self, sensor_id)

def main():
	logging.basicConfig(level=logging.DEBUG)

	try:
		myvidek = MyVidek()

		myvidek_t = myvidek.get_sensor("SHT21", "Temperature", "C")
		myvidek_rh = myvidek.get_sensor("SHT21", "Relative humidity", "%")
	except Exception as exc:
		log.error("Videk: will not work: %s" % (exc,))
		myvidek = None

	sht21 = SHT21()

	while True:
		t, rh = sht21.get()

		if myvidek is not None:
			myvidek_t.upload(t)
			myvidek_rh.upload(rh)

		time.sleep(60)

if __name__ == "__main__":
	main()
