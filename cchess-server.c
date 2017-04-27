// int on_connection() {
//   printf("Connection recieved\n");
//   // Send new client_id
// }



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

// int emmit(int event, char * client_id, char const *data[]) {
//   // Data == board status
//   switch (event) {
//     case 0:
//       // SEnd to client_id the data passed thru
//     break
//   }
//   return 0;
// }

int new_thread(void *client_socket) {
  /* If connection is established then start communicating */
  int new_socket_num = *(int *)client_socket;
  int n;
  char buffer[256];
  // Create a new board

  // Set player_waiting true
  // Conditional variable to wait for a player two

  while (1) {
    bzero(buffer, 256);
            /* Read from player 1 */
    n = read(new_socket_num, buffer, 255);

    if (n < 0) {
       perror("ERROR reading from socket");
       exit(1);
    }

    printf("Here is the message from %d: %s\n", new_socket_num, buffer);

    /* TODO process movement and change the board */

              /* Write to player 1 and 2 the board*/
    if (write(new_socket_num, "I got your\nmessage", 18) < 0) {
       perror("ERROR writing to socket");
       exit(1);
    }

    // Now read from player two
  }
}

int main( int argc, char *argv[] ) {
  pthread_t tid [5];

   int sockfd, client_socket, port_number, client_length;
   char buffer[256];
   struct sockaddr_in server_address, client;
   int  n;

   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   /* Initialize socket structure */
   bzero((char *) &server_address, sizeof(server_address));
   port_number = 8000;

   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = INADDR_ANY;
   server_address.sin_port = htons(port_number);

   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }

   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */

          /* MAX_QUEUE */
  listen(sockfd, 5);

   while(1) {
     client_length = sizeof(client);
     // CHECK IF WE'VE A WAITING USER

     /* Accept actual connection from the client */
     client_socket = accept(sockfd, (struct sockaddr *)&client, &client_length);
     printf("Connection accepted from %d at %d.%d.%d.%d:%d\n", client_socket, client.sin_addr.s_addr&0xFF, (client.sin_addr.s_addr&0xFF00)>>8, (client.sin_addr.s_addr&0xFF0000)>>16, (client.sin_addr.s_addr&0xFF000000)>>24, client.sin_port);

     if (client_socket < 0) {
        perror("ERROR on accept");
        exit(1);
     }

     // Check if a user is already waiting
     // If there's no one waiting create a new thread
     pthread_create(&tid[0], NULL, &new_thread, &client_socket);

     // If a user is waiting change user_waiting variable to *client_socket

   }

   close(sockfd);

   return 0;
}
