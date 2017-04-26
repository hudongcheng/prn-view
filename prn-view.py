#!/usr/bin/env python

from socket import *
import time

HOST = ''
PORT = 9100
BUFSIZ = 1024
ADDR = (HOST,PORT)
tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(ADDR)
tcpSerSock.listen(5)
#t = ""
while True:
	print 'waiting for connection ...'
	try:
		tcpCliSock, addr = tcpSerSock.accept()
		print '... connected from: ', addr
		t = time.strftime('%Y%m%d%H%M%S', time.localtime())
		prn_file = open('prn_' + t + '.prn', 'w')
		while True:
			data = tcpCliSock.recv(BUFSIZ)
			if not data:
				break
			prn_file.write(data)
		print 'write to ' + prn_file.name
		prn_file.close()
		tcpCliSock.close()
	except KeyboardInterrupt, e:
		tcpSerSock.close()
		exit(-1)

tcpSerSock.close()