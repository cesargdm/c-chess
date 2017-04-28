#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#include "board.c"

#define PORT 8080;

// Waiting player conditional variable
pthread_cond_t player_to_join;
pthread_mutex_t general_mutex;

// Match player
int challenging_player = 0;
int player_is_waiting = 0;

void move_piece(wchar_t ** board, int * move) {
  printf("origin: [%d, %d]\n", *move, *(move+1));
  printf("dest: [%d, %d]\n", *(move+2), *(move+3));
  // Move piece in board from origin to dest
  board[*(move+2)][*(move+3)] = board[*move][*(move+1)];
  board[*move][*(move+1)] = '\0';
}

void translate_to_move(int * move, char * buffer) {

  printf("buffer: %s\n", buffer);

  *(move) = 8-(*(buffer+1)-'0');
  *(move+1) = (*(buffer)-'0')-49;
  *(move+2) = 8-(*(buffer+4)-'0');
  *(move+3) = (*(buffer+3)-'0')-49;

  printf("[%d,%d] to [%d, %d]\n", *(move), *(move+1), *(move+2), *(move+3));

}

bool is_syntax_valid(int player, char * move) {
  // Look for -
  if (move[2] != '-') { write(player, "i-00", 22); return false; }
  //First and 3th should be characters
  if (move[0]-'0' < 10) { write(player, "i-01", 22); return false; }
  if (move[3]-'0' < 10) { write(player, "i-02", 22); return false; }

  //Second and 5th character should be numbers
  if (move[1]-'0' > 10) { write(player, "i-03", 22); return false; }
  if (move[1]-'0' > 8) { write(player, "i-04", 22); return false; }
  if (move[4]-'0' > 10) { write(player, "i-05", 22); return false; }
  if (move[4]-'0' > 8) { write(player, "i-06", 22); return false; }
  // Move out of range
  if (move[0]-'0' > 56 || move[3]-'0' > 56) { write(player, "i-07", 22); return false; }

  return true;
}

void broadcast(wchar_t ** board, int player_one, int player_two) {
  // Convert wchar_t ** to * wchar_t
  write(player_one, "😀😃😀", 256);
  write(player_two, board, 256);
}

bool is_move_valid(int player, int * move) {
  // TODO
  return true;
}

void * game_room(void *client_socket) {
  /* If connection is established then start communicating */
  int player_one = *(int *)client_socket;
  int n, player_two;
  char buffer[256];
  int * move = (int *)malloc(sizeof(int)*4);

  // Create a new board
  wchar_t ** board = create_board();
  initialize_board(board);

  // Set player_to_join true
  // Conditional variable to wait for a player two

  player_is_waiting = 1; // Set user waiting

  pthread_mutex_lock(&general_mutex); // Wait for player two
  pthread_cond_wait(&player_to_join, &general_mutex); // Wait for player wants to join signal

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

  bool syntax_valid = false;
  bool move_valid = false;

  while (1) {
    bzero(buffer, 256);

    // Waiit until syntax and move are valid
    while (!syntax_valid  || !move_valid) {
      printf("Checking syntax and move validation\n");
      // if (read(player_one, buffer, 6) < 0) { perror("ERROR reading from socket"); exit(1); }
      read(player_one, buffer, 6);
      printf("Player one: %s\n", buffer);

      syntax_valid = is_syntax_valid(player_one, buffer);

      translate_to_move(move, buffer); // Convert to move

      // TODO
      move_valid = is_move_valid(player_one, move);
    }

    printf("They're all now valid s(%d) m(%d)\n", syntax_valid, move_valid);

    syntax_valid = false;
    move_valid = false;

    // Apply move to board
    move_piece(board, move);
    broadcast(board, player_one, player_two);

    while (!syntax_valid && !move_valid) {
      // if (read(player_one, buffer, 6) < 0) { perror("ERROR reading from socket"); exit(1); }
      read(player_one, buffer, 6);

      syntax_valid = is_syntax_valid(player_two, buffer);

      translate_to_move(move, buffer); // Convert to move
      move_valid = is_move_valid(player_two, move);
    }

    syntax_valid = false;
    move_valid = false;

    // Apply move to board

    // broadcast(board, player_one, player_two);

  }

  /* delete board */
  free(move);
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
  port_number = PORT;

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
     client_socket = accept(sockfd, (struct sockaddr *)&client, (unsigned int *)&client_length);
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
