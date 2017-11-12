# ==================================================================
# Program Filename: chatserve.py
# Author: Peter Nguyen
# Date: 10/29/17
# CS 372-400
# Description: Project 1 - Server application for chat that connects
# to client application using sockets API and TCP protocol.
# ==================================================================

import sys
from socket import *

# ==================================================================
# Function: serverSetup
# Description: Takes a port number from the command line and binds
# the port to a new socket
# Parameters: userPort
# Returns: newSocket
#  ==================================================================

def serverSetup(userPort):
	# Get server port # from command line
	serverPort = int(userPort)
	newSocket = socket(AF_INET, SOCK_STREAM)
	newSocket.bind(('', serverPort))
	return newSocket

# ==================================================================
# Function: receiveMessage
# Description: Receives message from client and prints it if the
# message does not contain "\quit". Returns True if contains "\quit"
# Parameters: conSocket
# Returns: True or False
#  ==================================================================

def receiveMessage(conSocket):
	clientMessage = conSocket.recv(1024).decode()
	if "\\quit" in clientMessage:
		print("CONNECTION CLOSED BY CLIENT")
		return True
	else:
		print(clientMessage.decode())
		return False

# ==================================================================
# Function: sendMessage
# Description: Accepts user input for the message and sends it.
# Returns True if the message contains "\quit"
# Parameters: conSocket
# Returns: True or False
#  ==================================================================

def sendMessage(conSocket):
	serverMessage = raw_input('server> ')
	serverMessage = "server> " + serverMessage
	conSocket.send(serverMessage.encode())
	if "\\quit" in serverMessage:
		return True
	else:
		return False

# ==================================================================
# Main Program
# ==================================================================

# Initialize "welcome" socket
serverSocket = serverSetup(sys.argv[1])

# Continue to listen for new clients until SIGINT received
while True:
	serverSocket.listen(1)
	print("Server is ready to receive")

	connectionSocket, address = serverSocket.accept()

	# Continue to send/receive messages to/from current client
	# while messages don't contain "\quit"
	while True:
		quitFlag = receiveMessage(connectionSocket)
		if quitFlag:
			break
		else:
			quitFlag = sendMessage(connectionSocket)
		if quitFlag:
			break

	connectionSocket.close()
