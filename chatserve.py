#  ==================================================================
#  Program Filename: chatserve.py
#  Author: Peter Nguyen
#  Date: 10/29/17
#  CS 372-400
#  Description: Project 1 - Server application for chat that connects
#  to client application using sockets API and TCP protocol.
#  ==================================================================

import sys
from socket import *

serverPort = int(sys.argv[1])
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))

while True:
	serverSocket.listen(1)
	print("Server is ready to receive")

	connectionSocket, address = serverSocket.accept()

	while True:
		clientMessage = connectionSocket.recv(1024).decode()
		if "\\quit" in clientMessage:
			print("CONNECTION CLOSED BY CLIENT")
			break
		else:
			print(clientMessage.decode())
			serverMessage = raw_input('server> ')
			serverMessage = "server> " + serverMessage
			connectionSocket.send(serverMessage.encode())
		if "\\quit" in serverMessage:
			break

	connectionSocket.close()
