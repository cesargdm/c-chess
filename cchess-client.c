#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#include "board.c"

void * on_signal(void * sockfd) {
  char buffer[256];
  int n;
  int socket = *(int *)sockfd;
  while (1) {
    bzero(buffer,256);
    n = read(socket, buffer, 255);

    if (n < 0) {
       perror("ERROR reading from socket");
       exit(1);
    }

    printf("Message: '%s'\n", buffer);
    if (buffer[0] == 'i' || buffer[0] == 'e') {
      printf("Displaying message...\n");
      // Check if it's an informative or error message
    } else {
      // Print the board
      // Convert wchar_t * char to ** wchar_t
      printf("Priting board...\n");

    }
    // Check if { playerMadeMove: true }
    // Signal to read a new message
  }
}

int main(int argc, char *argv[]) {
   int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;

   char buffer[256];

   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }

   portno = atoi(argv[2]);

   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   server = gethostbyname(argv[1]);

   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);

   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }

   /* Now ask for a message from the user, this message
      * will be read by server
   */

   pthread_t tid[1];

   pthread_create(&tid[0], NULL, &on_signal, &sockfd);

   while (1) {
     printf("Please enter the message: ");
     bzero(buffer,256);
     fgets(buffer, 255, stdin);

     /* Send message to the server */
     n = write(sockfd, buffer, strlen(buffer));

     if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
     }

     /* Now read server response */
    //  bzero(buffer,256);
    //  n = read(sockfd, buffer, 255);
     //
    //  if (n < 0) {
    //     perror("ERROR reading from socket");
    //     exit(1);
    //  }
     //
    //  printf("%s\n",buffer);
   }

   return 0;
}
