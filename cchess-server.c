#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>

#include "board.c"

// Waiting player conditional variable
pthread_cond_t player_to_join;
pthread_mutex_t general_mutex;

// Match player
int challenging_player = 0;
int player_is_waiting = 0;

void *game_room(void *client_socket) {
  /* If connection is established then start communicating */
  int player_one = *(int *)client_socket;
  int n, player_two;
  char buffer[256];

  // Create a new board
  wchar_t ** board = create_board();
  initialize_board(board);

  // Set player_to_join true
  // Conditional variable to wait for a player two

  // Set user waiting
  player_is_waiting = 1;

  // Wait for player two
  pthread_mutex_lock(&general_mutex); // Unecesary?

  // Wait for player wants to join signal
  pthread_cond_wait(&player_to_join, &general_mutex);

  // TODO lock assigning player mutex
  player_two = challenging_player; // Asign the player_two to challenging_player
  player_is_waiting = 0; // Now none is waiting



  pthread_mutex_unlock(&general_mutex); // Unecesary?

  printf("We have a player two! (%d)\n", player_two);
  if (write(player_one, "{player: 1}", 16) < 0) {
     perror("ERROR writing to socket");
     exit(1);
  }
  if (write(player_two, "{player: 2}", 16) < 0) {
     perror("ERROR writing to socket");
     exit(1);
  }

  while (1) {
    bzero(buffer, 256);

          /* Read from player 1 */

        /* Wait read movement from player one */
    if (read(player_one, buffer, 255) < 0) {
       perror("ERROR reading from socket");
       exit(1);
    }

    printf("Player (%d) said: %s\n", player_one, buffer);

    /* Process movement and change the board */

              /* Write to player 1 and 2 the board*/
    if (write(player_one, "Printing board...", 22) < 0) {
       perror("ERROR writing to socket");
       exit(1);
    }
    if (write(player_two, "Printing board...", 22) < 0) {
       perror("ERROR writing to socket");
       exit(1);
    }

    /* Now read from player two */
    if (read(player_two, buffer, 255) < 0) {
       perror("ERROR reading from socket");
       exit(1);
    }

    printf("Player (%d) salid: %s\n", player_two, buffer);

    /* Process movement and change the board */

    if (write(player_one, "Priting board...", 22) < 0) {
       perror("ERROR writing to socket");
       exit(1);
    }
    if (write(player_two, "Priting board...", 22) < 0) {
       perror("ERROR writing to socket");
       exit(1);
    }

  }

  /* delete board */
  free_board(board);

}

int main( int argc, char *argv[] ) {
  pthread_t tid [5];

  int sockfd, client_socket, port_number, client_length;
  char buffer[256];
  struct sockaddr_in server_address, client;
  int  n;

  // Conditional variable
  pthread_cond_init(&player_to_join, NULL);
  pthread_mutex_init(&general_mutex, NULL);

  /* First call to socket() function */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }

  /* Initialize socket structure */
  bzero((char *) &server_address, sizeof(server_address));
  port_number = 8080;

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

     pthread_mutex_lock(&general_mutex); // Unecesary?
     // Create thread if we have no user waiting
     if (player_is_waiting == 0) {
       printf("I'm lonely, creating my game room...\n");
       pthread_create(&tid[0], NULL, &game_room, &client_socket);
       pthread_mutex_unlock(&general_mutex); // Unecesary?
     }
     // If we've a user waiting join that room
     else {
       // Send user two signal
       printf("I'm joining with %d\n", client_socket);
       challenging_player = client_socket;
       pthread_mutex_unlock(&general_mutex); // Unecesary?
       pthread_cond_signal(&player_to_join);
     }


   }

   close(sockfd);

   return 0;
}
