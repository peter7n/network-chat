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

#define BUFFER_SIZE 500

int serverConnect(char* userPort, char* userHost);
void sendMessage(char* handle, char* clientMessage, char* prependMessage, int clientSocket);
void receiveMessage(char* clientMessage, char* serverMessage, int clientSocket);
void prependHandle(char* handle, char* message, char* prependMessage);

int main(int argc, char *argv[])
{
   int clientSocket = 0;
   char handle[12];
   char prependMessage[512];
   char clientMessage[BUFFER_SIZE];
   char serverMessage[BUFFER_SIZE];

   // Get user's handle name
   printf("Enter your handle name: ");
   fgets(handle, 12, stdin);
   handle[strlen(handle) - 1] = '\0';   // remove newline

   clientSocket = serverConnect(argv[2], argv[1]);

   // Enter into main send/receive message loop
   do
   {
      sendMessage(handle, clientMessage, prependMessage, clientSocket);
      receiveMessage(clientMessage, serverMessage, clientSocket);
   }
   while (strcmp(clientMessage, "\\quit") != 0 &&
         strstr(serverMessage, "\\quit") == NULL);

   printf("CONNECTION CLOSED\n");
   close(clientSocket);

   return 0;
}

/*********************************************************************
 ** Function: serverConnect
 ** Description: Accepts client host name and port from the command
 ** line; configures server address struct, connects to server;
 ** Parameters: char* userPort, char* userHost
 ** Returns: int, the new client socket
 *********************************************************************/

int serverConnect(char* userPort, char* userHost)
{
   int newSocket = 0;
   int serverPort = 0;
   struct hostent *server;        // defines a host computer
   struct sockaddr_in serverAddress;

   // Set server name and port number
   serverPort = atoi(userPort);
   server = gethostbyname(userHost);
   newSocket = socket(AF_INET, SOCK_STREAM, 0);

   // Configure server address struct
   bzero((char *) &serverAddress, sizeof(serverAddress));
   serverAddress.sin_family = AF_INET;
   bcopy((char *)server->h_addr,
         (char *)&serverAddress.sin_addr.s_addr,
         server->h_length);
   serverAddress.sin_port = htons(serverPort);

   // Connect to server
   connect(newSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

   return newSocket;
}

/*********************************************************************
 ** Function: sendMessage
 ** Description: Gets message input from user, prepends the user's
 ** "handle" and sends message to the server
 ** Parameters: char* handle, char* clientMessage, char* prependMessage, int clientSocket
 *********************************************************************/

void sendMessage(char* handle, char* clientMessage, char* prependMessage, int clientSocket)
{
   // Get input for the client message that will be sent
   printf("%s> ", handle);
   fgets(clientMessage, BUFFER_SIZE, stdin);
   clientMessage[strlen(clientMessage) - 1] = '\0';   // remove newline

   // Send message with handle prepended to it
   prependHandle(handle, clientMessage, prependMessage);
   send(clientSocket, prependMessage, strlen(prependMessage), 0);
}

/*********************************************************************
 ** Function: receiveMessage
 ** Description: Receives message from the server and prints it to the
 ** screen if it does not contain "\quit"
 ** Parameters: char* clientMessage, char* serverMessage, int clientSocket
 *********************************************************************/

void receiveMessage(char* clientMessage, char* serverMessage, int clientSocket)
{
   if (strcmp(clientMessage, "\\quit") != 0)
   {
      // Receive message from server
      memset(serverMessage, 0, BUFFER_SIZE);   // clear serverMessage buffer
      recv(clientSocket, serverMessage, BUFFER_SIZE, 0);
      if (strstr(serverMessage, "\\quit") == NULL)
         printf("%s\n", serverMessage);
   }
}

/*********************************************************************
 ** Function: prependHandle
 ** Description: Takes a client message and prepends the user's
 ** handle to the string
 ** Parameters: char* handle, char* message, char* prependMessage
 *********************************************************************/

void prependHandle(char* handle, char* message, char* prependMessage)
{
   strcpy(prependMessage, handle);
   strcat(prependMessage, "> ");
   strcat(prependMessage, message);
}
