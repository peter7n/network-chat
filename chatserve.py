from socket import *
serverPort = 31234
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))
serverSocket.listen(1)
print("Server is ready to receive")

while True:
  connectionSocket, addr = serverSocket.accept()
  sentence = connectionSocket.recv(1024).decode()
  print("From Client: {}".format(sentence.decode()))

  serveSentence = raw_input('Input sentence: ')
  connectionSocket.send(serveSentence.encode())
  connectionSocket.close()
