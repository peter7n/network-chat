from socket import *
serverPort = 32082
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))

while True:
	serverSocket.listen(1)
	print("Server is ready to receive")

	connectionSocket, address = serverSocket.accept()

	while True:
		clientMessage = connectionSocket.recv(1024).decode()
		if clientMessage == "quit":
			print("CONNECTION CLOSED BY CLIENT")
			break
		else:
			print("From Client: {}".format(clientMessage.decode()))
			serverMessage = raw_input('Input Message: ')
			connectionSocket.send(serverMessage.encode())
		if serverMessage == "quit":
			break

	connectionSocket.close()
