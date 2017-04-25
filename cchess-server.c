// int on_connection() {
//   printf("Connection recieved\n");
//   // Send new client_id
// }

// int emmit(int event, char * client_id, char const *data[]) {
//   // Data == board status
//   switch (event) {
//     case 0:
//       // SEnd to client_id the data passed thru
//     break
//   }
//   return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int new_thread(void *new_socket) {
  /* If connection is established then start communicating */
  int new_socket_num = *(int *)new_socket;
  int n;
  char buffer[256];

  while (1) {
    bzero(buffer, 256);
    n = read(new_socket_num, buffer, 255);

    if (n < 0) {
       perror("ERROR reading from socket");
       exit(1);
    }

    printf("Here is the message: %s\n", buffer);

    /* Write a response to the client */
    n = write(new_socket_num, "I got your message", 18);

    if (n < 0) {
       perror("ERROR writing to socket");
       exit(1);
    }
  }
}

int main( int argc, char *argv[] ) {
  pthread_t tid [5];

   int sockfd, new_socket, portno, client_length;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;

   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 8080;

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }

   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */

            /* MAX_QUEUE */
  listen(sockfd, 5);

   while(1) {
     printf("While\n");
     client_length = sizeof(cli_addr);

     /* Accept actual connection from the client */
     new_socket = accept(sockfd, (struct sockaddr *)&cli_addr, &client_length);
     printf("Socket fd %d\n", cli_addr.sin_addr.s_addr);

     if (new_socket < 0) {
        perror("ERROR on accept");
        exit(1);
     }

     pthread_create(&tid[0], NULL, &new_thread, &new_socket);

   }

   close(sockfd);

   return 0;
}
