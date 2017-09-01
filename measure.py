import vesna.alh
import serial
import logging
import time

def main():
	logging.basicConfig(level=logging.INFO)

	f = serial.Serial("/dev/ttyUSB0", 115200, timeout=10)
	node = vesna.alh.ALHTerminal(f)

	while True:
		resp = node.get("sensors/sht21")
		time.sleep(60)

if __name__ == "__main__":
	main()
