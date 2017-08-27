#!/usr/bin/env python

import time
import serial

LOGFILE = "dump.bin"
SERIAL = "/dev/ttyUSB0"
SPEED = 115200
TIMEOUT = 1000	# ms
BUFSIZE = 128

with serial.Serial (SERIAL, SPEED, timeout = TIMEOUT / 1000.0) as ser, open (LOGFILE, "w") as fp:
	# Wait for Arduino to settle down after reset consequent to serial port opening
	time.sleep (3)

	# Tell sketch to start dumping (Any char will do)
	ser.write ("A");

	recv = 0;
	while True:
		buf = ser.read (BUFSIZE)
		if buf:
			recv += len (buf)
			fp.write (buf)
			print recv
		else:
			print "<TIMEOUT>"
			break
