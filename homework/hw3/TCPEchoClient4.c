#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

int main(int argc, char *argv[]) {

  if (argc < 3 || argc > 4) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)",
        "<Server Address> <Echo Word> [<Server Port>]");

  char *servIP = argv[2];     // First arg: server IP address (dotted quad)
  char *echoString = argv[1]; // Second arg: string to echo

  // Third arg (optional): server port (numeric).  7 is well-known echo port
  in_port_t servPort = (argc == 4) ? atoi(argv[3]) : 7;

  // Create a reliable, stream socket using TCP
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
    DieWithSystemMessage("socket() failed");

  // Construct the server address structure
  struct sockaddr_in servAddr;            // Server address
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  servAddr.sin_family = AF_INET;          // IPv4 address family
  // Convert address
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if (rtnVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (rtnVal < 0)
    DieWithSystemMessage("inet_pton() failed");
  servAddr.sin_port = htons(servPort);    // Server port

  // Establish the connection to the echo server
  if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("connect() failed");

  ssize_t numBytes=0;

  size_t echoStringLen = strlen(echoString); // Determine input length
  char eachStr[10];
  int loop=1;
  for(loop=1;loop<=4;loop++){
      strncpy(eachStr,echoString,1);
      eachStr[1]='\0';
      numBytes=send(sock,eachStr,strlen(eachStr),0);
      echoString++;
      sleep(3);
  }
  // Send the string to the server
//  char *send1="t";
//  char *send2="i";
//  char *send3="me";
//  ssize_t numBytes = send(sock, send1, strlen(send1), 0);
//  sleep(1);
//  numBytes=send(sock,send2,strlen(send2),0);
//  sleep(1);
//  numBytes=send(sock,send3,strlen(send3),0);
//  if (numBytes < 0)
//    DieWithSystemMessage("send() failed");
//  else if (numBytes != echoStringLen)
//    DieWithUserMessage("send()", "sent unexpected number of bytes");

  // Receive the same string back from the server
  //unsigned int totalBytesRcvd = 0; // Count of total bytes received
  //fputs("Received: ", stdout);     // Setup to print the echoed string
  char buffer[BUFSIZE]; // I/O buffer
  numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
  while (numBytes!=0) {
    /* Receive up to the buffer size (minus 1 to leave space for
     a null terminator) bytes from the sender */
//    if (numBytes < 0)
//      DieWithSystemMessage("recv() failed");
//    else if (numBytes == 0)
//      DieWithUserMessage("recv()", "connection closed prematurely");
//    totalBytesRcvd += numBytes; // Keep tally of total bytes
//    buffer[numBytes] = '\0';    // Terminate the string!
    fputs(buffer, stdout);      // Print the echo buffer
    numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
  }

  fputc('\n', stdout); // Print a final linefeed

  close(sock);
  exit(0);
}
