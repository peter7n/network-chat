/*********************************************************************
 ** Program Filename: chatclient.c
 ** Author: Peter Nguyen
 ** Date: 10/29/17
 ** CS 372-400
 ** Description: Project 1 - Client application for chat that connects
 ** to server application using sockets API and TCP protocol.
 *********************************************************************/

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

void prependHandle(char* handle, char* message, char* prependMessage);

int main(int argc, char *argv[])
{
   int serverPort = 0;
   int clientSocket;
   char handle[12];
   char prependMessage[512];
   char clientMessage[BUFFER_SIZE];
   char serverMessage[BUFFER_SIZE];
   struct hostent *server;        // defines a host computer
   struct sockaddr_in serverAddress;

   // Get user's handle name
   do
   {
      printf("Enter your handle name: ");
      fgets(handle, 12, stdin);
      printf("%d\n", strlen(handle));
      handle[strlen(handle) - 1] = '\0';   // remove newline
   }
   while (strlen(handle) > 10);

   // Set server name and port number
   serverPort = atoi(argv[2]);
   server = gethostbyname(argv[1]);
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
      // Get client message to send
      printf("%s> ", handle);
      fgets(clientMessage, BUFFER_SIZE, stdin);
      clientMessage[strlen(clientMessage) - 1] = '\0';   // remove newline

      // Send message with handle prepended to it
      prependHandle(handle, clientMessage, prependMessage);
      send(clientSocket, prependMessage, strlen(prependMessage), 0);

      if (strcmp(clientMessage, "\\quit") != 0)
      {
         // Receive message from server
         memset(serverMessage, 0, BUFFER_SIZE);   // clear serverMessage buffer
         recv(clientSocket, serverMessage, BUFFER_SIZE, 0);
         if (strstr(serverMessage, "\\quit") == NULL)
            printf("%s\n", serverMessage);
      }
   }
   while (strcmp(clientMessage, "\\quit") != 0 &&
         strstr(serverMessage, "\\quit") == NULL);

   printf("CONNECTION CLOSED\n");
   close(clientSocket);

   return 0;
}

void prependHandle(char* handle, char* message, char* prependMessage)
{
   strcpy(prependMessage, handle);
   strcat(prependMessage, "> ");
   strcat(prependMessage, message);
}
