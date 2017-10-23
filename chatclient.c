#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
// #include <arpa/inet.h>

int main(int argc, char *argv[])
{
  int serverPort = 31234;
  int clientSocket;
  char sentence[1024];
  char modifiedSentence[1024];
  struct hostent *server;        // Defines a host computer
  struct sockaddr_in serverAddress;

  server = gethostbyname("localhost");
  clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  // Configure server address struct
  bzero((char *) &serverAddress, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
        (char *)&serverAddress.sin_addr.s_addr,
        server->h_length);
  serverAddress.sin_port = htons(serverPort);

  // Connect to server
  connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

  printf("Input lowercase sentence: ");
  fgets(sentence, 1024, stdin);

  send(clientSocket, sentence, strlen(sentence), 0);

  recv(clientSocket, modifiedSentence, 1024, 0);

  printf("From Server: %s\n", modifiedSentence);

  close(clientSocket);

  return 0;
}
