#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
// #include <arpa/inet.h>

#define BUFFER_SIZE 500

int main(int argc, char *argv[])
{
   int serverPort = 32082;
   int clientSocket;
   char handle[12];
   char msg[512];
   char clientMessage[BUFFER_SIZE];
   char serverMessage[BUFFER_SIZE];
   struct hostent *server;        // defines a host computer
   struct sockaddr_in serverAddress;

   printf("Enter your handle name: ");
   fgets(handle, 12, stdin);
   printf("%d\n", strlen(handle));
   handle[strlen(handle) - 1] = '\0';   // remove newline
   printf("%s\n", handle);
   printf("%d\n", strlen(handle));
   strcpy(msg, handle);
   strcat(msg, "> Test message 123");
   printf("%s", msg);

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

   do
   {
      printf("Input Message: ");
      fgets(clientMessage, BUFFER_SIZE, stdin);
      clientMessage[strlen(clientMessage) - 1] = '\0';   // remove newline
      send(clientSocket, clientMessage, strlen(clientMessage), 0);

      if (strcmp(clientMessage, "quit\n") != 0)
      {
         memset(serverMessage, 0, BUFFER_SIZE);   // clear serverMessage buffer
         recv(clientSocket, serverMessage, BUFFER_SIZE, 0);
         if (strcmp(serverMessage, "quit") != 0)
         printf("From Server: %s\n", serverMessage);
      }
   }
   while (strcmp(clientMessage, "quit") != 0 && strcmp(serverMessage, "quit") != 0);

   printf("CONNECTION CLOSED\n");
   close(clientSocket);

   return 0;
}
