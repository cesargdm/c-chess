#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>

#include "board.c"

#define PORT 8080;

// Waiting player conditional variable
pthread_cond_t player_to_join;
pthread_mutex_t general_mutex;

// Match player
int challenging_player = 0;
int player_is_waiting = 0;




void move_piece(wchar_t ** board, int * move) {
  // printf("origin: [%d, %d]\n", *move, *(move+1));
  // printf("dest: [%d, %d]\n", *(move+2), *(move+3));
  // Move piece in board from origin to dest
  board[*(move+2)][*(move+3)] = board[*move][*(move+1)];
  board[*move][*(move+1)] = '\0';
}

bool emit(int client, char * message, int message_size) {
  return true;
}

void translate_to_move(int * move, char * buffer) {

  printf("buffer: %s\n", buffer);

  *(move) = 8-(*(buffer+1)-'0');
  *(move+1) = (*(buffer)-'0')-49;
  *(move+2) = 8-(*(buffer+4)-'0');
  *(move+3) = (*(buffer+3)-'0')-49;

  // printf("[%d, %d] to [%d, %d]\n", *(move), *(move+1), *(move+2), *(move+3));

}

bool is_syntax_valid(int player, char * move) {
  // Look for -
  if (move[2] != '-') { send(player, "e-00", 4, 0); return false; }
  //First and 3th should be characters
  if (move[0]-'0' < 10) { send(player, "e-01", 4, 0); return false; }
  if (move[3]-'0' < 10) { send(player, "e-02", 4, 0); return false; }

  //Second and 5th character should be numbers
  if (move[1]-'0' > 10) { send(player, "e-03", 4, 0); return false; }
  if (move[1]-'0' > 8) { send(player, "e-04", 4, 0); return false; }
  if (move[4]-'0' > 10) { send(player, "e-05", 4, 0); return false; }
  if (move[4]-'0' > 8) { send(player, "e-06", 4, 0); return false; }
  // Move out of range
  if (move[0]-'0' > 56 || move[3]-'0' > 56) { send(player, "e-07", 4, 0); return false; }

  return true;
}

void broadcast(wchar_t ** board, char * one_dimension_board, int player_one, int player_two) {

  to_one_dimension_char(board, one_dimension_board);

  printf("\tSending board to %d and %d size(%lu)\n", player_one, player_two, sizeof(one_dimension_board));
  send(player_one, one_dimension_board, 64, 0);
  send(player_two, one_dimension_board, 64, 0);
  printf("\tSent board...\n");
}

int get_piece_team(wchar_t ** board, int x, int y) {

  wchar_t piece = board[x][y];

  switch (piece) {
    case white_king: return -1;
    case white_queen: return -1;
    case white_rook: return -1;
    case white_bishop: return -1;
    case white_knight: return -1;
    case white_pawn: return -1;
    case black_king: return 1;
    case black_queen: return 1;
    case black_rook: return 1;
    case black_bishop: return 1;
    case black_knight: return 1;
    case black_pawn: return 1;
  }
  return 0;
}

int get_piece_type(wchar_t piece) {
  switch (piece) {
    case white_king: return 0;
    case white_queen: return 1;
    case white_rook: return 2;
    case white_bishop: return 3;
    case white_knight: return 4;
    case white_pawn: return 5;
    case black_king: return 0;
    case black_queen: return 1;
    case black_rook: return 2;
    case black_bishop: return 3;
    case black_knight: return 4;
    case black_pawn: return 5;
  }
  return -1;
}

bool is_move_valid(wchar_t ** board, int player, int team, int * move) {

  // General errors
  if (board[*(move)][*(move+1)] == 0) {
    send(player, "e-08", 4, 0);
    return false;
  }  // If selected piece == 0 there's nothing selected
  if (get_piece_team(board, *(move), *(move+1)) == get_piece_team(board, *(move+2), *(move+3))) {
    send(player, "e-09", 4, 0);
    return false;
  } // If the origin piece's team == dest piece's team is an invalid move

  switch (get_piece_type(board[*(move)][*(move+1)])) {
    case 0: /* --- ♚ --- */
      if (abs(x_moves) > 1 || abs(y_moves) > 1) return 0;
      break;
    // case 5: /* --- ♟ --- */
    //   if (y_moves != 0) {
    //     if (!is_diagonal(x_moves, y_moves) || (team_in_dest == 0)) return 50; // Check if it's a diagonal move and it's not an empty location
    //     if (eat_piece(dest)) return 99; // Check if there's something to eat
    //   }
    //   if (*origin == 6 || *origin == 1) {
    //     if (x_moves > 2) return 51;
    //   } else {
    //     if (x_moves > 1) return 52;
    //   }
    //   break;
    default:
      break;
  }

  return true;
}

void * game_room(void *client_socket) {
  /* If connection is established then start communicating */
  int player_one = *(int *)client_socket;
  int n, player_two;
  char buffer[64];
  int * move = (int *)malloc(sizeof(int)*4);

  // Create a new board
  wchar_t ** board = create_board();
  char * one_dimension_board = create_od_board();
  initialize_board(board);

  // Set player_to_join true
  // Conditional variable to wait for a player two

  player_is_waiting = 1; // Set user waiting

  // send(player_one, "i-w", 4);

  pthread_mutex_lock(&general_mutex); // Wait for player two
  pthread_cond_wait(&player_to_join, &general_mutex); // Wait for player wants to join signal

  // TODO lock assigning player mutex
  player_two = challenging_player; // Asign the player_two to challenging_player
  player_is_waiting = 0; // Now none is waiting

  pthread_mutex_unlock(&general_mutex); // Unecesary?

  if (send(player_one, "i-p1", 4, 0) < 0) {
     perror("ERROR writing to socket");
     exit(1);
  }
  if (send(player_two, "i-p2", 4, 0) < 0) {
     perror("ERROR writing to socket");
     exit(1);
  }

  sleep(1);

  broadcast(board, one_dimension_board, player_one, player_two);

  sleep(1);

  bool syntax_valid = false;
  bool move_valid = false;

  while (1) {

    send(player_one, "i-tm", 4, 0);
    send(player_two, "i-nm", 4, 0);

    // Wait until syntax and move are valid
    printf("Waiting for move from player one (%d)... sending i\n", player_one);

    while (!syntax_valid  || !move_valid) {
      bzero(buffer, 64);

      printf("Checking syntax and move validation (%d,%d)\n",syntax_valid,move_valid);
      if (read(player_one, buffer, 6) < 0) {
        perror("ERROR reading from socket");
        exit(1);
      }
      printf("Player one (%d) move: %s\n", player_one, buffer);

      syntax_valid = is_syntax_valid(player_one, buffer);

      translate_to_move(move, buffer); // Convert to move

      // TODO
      move_valid = is_move_valid(board, player_one, 1, move);
    }
    printf("Player one (%d) made move\n", player_one);

    syntax_valid = false;
    move_valid = false;

    // Apply move to board
    move_piece(board, move);

    // Send applied move board
    broadcast(board, one_dimension_board, player_one, player_two);
    sleep(1);
    send(player_one, "i-nm", 4, 0);
    send(player_two, "i-tm", 4, 0);

    printf("Waiting for move from player two (%d)\n", player_two);

    while (!syntax_valid  || !move_valid) {
      bzero(buffer, 64);

      if (read(player_two, buffer, 6) < 0) {
        perror("ERROR reading from socket");
        exit(1);
      }

      syntax_valid = is_syntax_valid(player_two, buffer);

      translate_to_move(move, buffer); // Convert to move

      move_valid = is_move_valid(board, player_two, -1, move);
    }
    printf("Player two (%d) made move\n", player_two);

    syntax_valid = false;
    move_valid = false;

    syntax_valid = false;
    move_valid = false;

    // Apply move to board
    move_piece(board, move);

    // Send applied move board
    broadcast(board, one_dimension_board, player_one, player_two);
    sleep(1);

  }

  /* delete board */
  free(move);
  free_board(board);

}

int main( int argc, char *argv[] ) {
  pthread_t tid [5];

  int sockfd, client_socket, port_number, client_length;
  char buffer[64];
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

          /* MAX_QUEUE */
  listen(sockfd, 20);

   while(1) {
     client_length = sizeof(client);
     // CHECK IF WE'VE A WAITING USER

     /* Accept actual connection from the client */
     client_socket = accept(sockfd, (struct sockaddr *)&client, (unsigned int *)&client_length);
     printf("– Connection accepted from %d at %d.%d.%d.%d:%d –\n", client_socket, client.sin_addr.s_addr&0xFF, (client.sin_addr.s_addr&0xFF00)>>8, (client.sin_addr.s_addr&0xFF0000)>>16, (client.sin_addr.s_addr&0xFF000000)>>24, client.sin_port);

     if (client_socket < 0) {
        perror("ERROR on accept");
        exit(1);
     }

     pthread_mutex_lock(&general_mutex); // Unecesary?
     // Create thread if we have no user waiting
     if (player_is_waiting == 0) {
       printf("Connected player, creating new game room...\n");
       pthread_create(&tid[0], NULL, &game_room, &client_socket);
       pthread_mutex_unlock(&general_mutex); // Unecesary?
     }
     // If we've a user waiting join that room
     else {
       // Send user two signal
       printf("Connected player, joining game room... %d\n", client_socket);
       challenging_player = client_socket;
       pthread_mutex_unlock(&general_mutex); // Unecesary?
       pthread_cond_signal(&player_to_join);
     }


   }

   close(sockfd);

   return 0;
}
