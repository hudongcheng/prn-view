#!/usr/bin/env python

from socket import *
from time import ctime

HOST = ''
PORT = 9100
BUFSIZ = 1024
ADDR = (HOST,PORT)
tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(ADDR)
tcpSerSock.listen(5)
t = 0
while True:
	print 'waiting for connection ...'
	tcpCliSock, addr = tcpSerSock.accept()
	print '... connected from: ', addr
	prn_file = open('prn_' + str(t) + '.prn', 'w')
	t = t + 1
	while True:
		data = tcpCliSock.recv(BUFSIZ)
		if not data:
			break
		prn_file.write(data)
	print 'write to ' + prn_file.name
	prn_file.close()
	tcpCliSock.close()
tcpSerSock.close()
