from socket import *
serverName = 'localhost'
serverPort = 31234
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))
sentence = raw_input('Input sentence: ')
clientSocket.send(sentence.encode())
modifiedSentence = clientSocket.recv(1024)
print("From Server: {}".format(modifiedSentence.decode()))
clientSocket.close()
