#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>
#include <pthread.h>

#include "board.c"
#include "piece.c"

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define MAX_MOVE_LENGTH 256

#define TEAM 0

char move[MAX_MOVE_LENGTH];

char * cpmove_from;
char * cpmove_to;

int imove_from[2];
int imove_to[2];

char move_from[2];
char move_to[2];

void move_piece(int *, int *);
void translate_move(char *, int *);
bool is_diagonal(int, int);
int is_diagonal_clear(int *, int *);
bool is_rect(int, int);
int is_rect_clear(int *, int *);
bool eat_piece(int *);
int check_move(int *, int *);
int make_move();
void debug_print_board();
int ask_move();

void * socket_io();
void socket_io_emit(char *);
void * graphical_interface();

char * signal_data;

pthread_cond_t on_connect;
pthread_cond_t on_match;
pthread_cond_t on_signal;
pthread_mutex_t general_mutex;

int main(int argc, char const *argv[]) {

  // Initialize 3 threads
    // UI - moves
    // Web changes

  pthread_t tid[3];

  // Print the board

  // Init mutex
  // pthread_mutex_init(&general_mutex, NULL);

  // Init conditional variables
  // pthread_cond_init(&on_connect, NULL);
  // pthread_cond_init(&on_match, NULL);
  // pthread_cond_init(&on_signal, NULL);

  // ASK FOR TEAM
  // pthread_create(&tid[0], NULL, &socket_io, NULL);
  pthread_create(&tid[0], NULL, &graphical_interface, NULL);

  while (1) {
    sleep(1);
  }

  return 0;
}

void * graphical_interface() {

  // pthread_mutex_lock(&general_mutex);

  // Create board
  char * the_board = create_board();

  // pthread_cond_wait(&on_connect, &general_mutex);
  printf("watch_count(): thread Condition signal received.\n");

  // Print initial state of board
  print_board();

  while (1) {
    // Wait for signal of user recieved
    // pthread_cond_wait(&on_connect, &general_mutex);
    while (ask_move());
    make_move();
    print_board();
  }
  return 0;
  // pthread_mutex_unlock(&general_mutex);
}


void * socket_io() {
  pthread_mutex_lock(&general_mutex);

  // Connect to server

  // Send team type
  printf("Connecting to server...\n");
  sleep(2);
  // Send connected signal
  // pthread_cond_signal(&on_connect);

  // On challenger found
  printf("Looking for open match...\n");
  sleep(2);
  // GET MATCH_ID

  // pthread_cond_signal(&on_connect);

  //Unlock mutex
  // pthread_mutex_unlock(&general_mutex);

  // Listen for changes
  while (1) {
    // Send signals recieved to UI thread
    // signal_data =
    // pthread_cond_signal(&on_signal);
  }
  return 0;
}

void socket_io_emit(char * argv) {
  // Send data to server
}

void move_piece(int * origin, int * dest) {
  // Move piece in board from origin to dest
  board[*dest][*(dest+1)] = board[*origin][*(origin+1)];
  board[*origin][*(origin+1)] = 0;
}

void translate_move(char * cmove, int * imove) {

  // Get int components
  int x = (*(cmove)-'0')-49; // Char to int and to board position
  int y = 8-(*(cmove+1)-'0'); // Int to board position

  // Set int move values
  *(imove) = y;
  *(imove+1) = x;

}

bool is_diagonal(int x_moves, int y_moves) {

  if ((abs(x_moves)-abs(y_moves)) != 0) {
    return false;
  }
  return true;

}

/* Check if the diagonal move is clear one position before dest */
int is_diagonal_clear(int * origin, int * dest) {

  int x_moves = *(origin) - *(dest);
  int y_moves = *(origin+1) - *(dest+1);

  // Check valid diagonal
  if (!is_diagonal(x_moves, y_moves)) return false;

  // Get all positions ine before dest
  int index = abs(x_moves) - 1;
  wchar_t item_at_position;

  // Iterate thru all items excepting initial posi
  while (index > 0) {

    if (x_moves > 0 && y_moves > 0) item_at_position = board[*origin-index][*(origin+1)-index];
    if (x_moves > 0 && y_moves < 0) item_at_position = board[*origin-index][*(origin+1)+index];
    if (x_moves < 0 && y_moves < 0 ) item_at_position = board[*origin+index][*(origin+1)+index];
    if (x_moves < 0 && y_moves > 0 ) item_at_position = board[*origin+index][*(origin+1)-index];

    if (item_at_position != 0) return false;

    index --;
  }

  return true;
}

bool is_rect(int x_moves, int y_moves) {
  if ((abs(x_moves) > 0 && y_moves == 0) || (abs(y_moves) > 0 && x_moves == 0)) return true;
  return false;
}

/* Check if the rect move is clear one position before dest */
int is_rect_clear(int * origin, int * dest) {

  int x_moves = *(origin) - *(dest);
  int y_moves = *(origin+1) - *(dest+1);

  // Is a side rect
  int index;

  if (abs(x_moves) > abs(y_moves)) {
    index = abs(x_moves) - 1;
  } else {
    index = abs(y_moves) - 1;
  }

  wchar_t item_at_position;

  // Iterate thru all items excepting initial posi
  while (index > 0) {

    if (x_moves > 0) item_at_position = board[*origin-index][*(origin+1)];
    if (x_moves < 0) item_at_position = board[*origin+index][*(origin+1)];
    if (y_moves > 0 ) item_at_position = board[*origin][*(origin+1)-index];
    if (y_moves < 0 ) item_at_position = board[*origin][*(origin+1)+index];

    if (item_at_position != 0) return false;

    index --;
  }

  return true;

}

bool eat_piece(int * position) {
  // If there's no piece there return false
  return (get_piece_team(position) != 0);
}

/* Check if the move status like valid, invalid, eated piece and more */

int check_move(int * origin, int * dest) {

  wchar_t piece_in_origin = board[*origin][*(origin+1)];
  wchar_t piece_in_dest = board[*dest][*(dest+1)];

  int team_in_dest = get_piece_team(dest);
  int team_in_origin = get_piece_team(origin);

  int x_moves = *(origin) - *(dest);
  int y_moves = *(origin+1) - *(dest+1);

  int origin_piece_type = get_piece_type(piece_in_origin);

  // DEBUG
  // printf("Pieces : %lc(o) %lc(d)\n", piece_in_origin, piece_in_dest);
  // printf("Teams  : %d(o) %d(d)\n", team_in_dest, team_in_origin);
  // printf("[%d,%d]\n", *(origin), *(origin+1));
  // printf("piece type: %d\n", origin_piece_type);

  /*  --- GENERAL RULES --- */
  if (piece_in_origin == 0) return 1; // If selected piece == 0 there's nothing selected
  if (team_in_origin == team_in_dest) return 2; // If the origin piece's team == dest piece's team is an invalid move

  /* --- SPECIFIC RULES --- */
  switch (origin_piece_type) {
    case 0: /* --- ♚ --- */
      if (abs(x_moves) > 1 || abs(y_moves) > 1) return 0;
      break;
    case 1: /* --- ♛ --- */
      // Check if is a diagonal or straight move
      if (is_diagonal(x_moves, y_moves)) {
        if (!is_diagonal_clear(origin, dest)) return 11; // Check if diagonal is clear (no jumping)
        if (eat_piece(dest)) return 99; // Check if there's something to eat
      } else {
        if (!is_rect(x_moves, y_moves)) return 10; // Check if it's a valid straight move
        if (!is_rect_clear(origin, dest)) return 11; // Check if rect is clear (no jumping)
        if (eat_piece(dest)) return 99; // Check if there's something to eat
      }
      break;
    case 2: /* --- ♜ --- */
      if (!is_rect(x_moves, y_moves)) return 20;
      if (!is_rect_clear(origin, dest)) return 21; // Check if rect is clear (no jumping)
      if (eat_piece(dest)) return 99; // Check if there's something to eat
      break;
    case 3: // ♝
      if (!is_diagonal(x_moves, y_moves)) return 30; // Check if it's a valid diagonal move
      if (!is_diagonal_clear(origin, dest)) return 31; // Check if diagonal is clear (no jumping)
      if (eat_piece(dest)) return 99; // Check if there's something to eat
      break;
    case 4: /* --- ♞ --- */
      // Check if if it's a knight's valid move
      if ((abs(x_moves) != 1 || abs(y_moves) != 2) && (abs(x_moves) != 2 || abs(y_moves) != 1)) return 40;
      if (eat_piece(dest)) return 99; // Check if there's something to eat
      break;
    case 5: /* --- ♟ --- */
      if (y_moves != 0) {
        if (!is_diagonal(x_moves, y_moves) || (team_in_dest == 0)) return 50; // Check if it's a diagonal move and it's not an empty location
        if (eat_piece(dest)) return 99; // Check if there's something to eat
      }
      if (*origin == 6 || *origin == 1) {
        if (x_moves > 2) return 51;
      } else {
        if (x_moves > 1) return 52;
      }
      break;
    default:
      break;
  }
  return 0;
}

int make_move() {

  //debug_print_board();

  translate_move(move_from, imove_from);
  translate_move(move_to, imove_to);

  int status = check_move(imove_from, imove_to);
  /*
  -- MOVE STATUS --

  0X
  1X
  2X
  3X
  4X
  5X

  99 Eated piece

  */

  switch (status) {
    case 0: //Simple move
      move_piece(imove_from, imove_to);
      break;
    case 1:
      printf(RED "Nothing selected" RESET);
      break;
    case 2:
      printf(RED "Invalid move, cannot move into your own pieces position" RESET);
      break;
    case 4:
      printf(RED "Invalid knight move" RESET);
      break;
    case 99:
      printf(RED "You defeated a piece" RESET);
      move_piece(imove_from, imove_to);
      break;
    default:
      printf(RED "Invalid move [%d]" RESET, status);
      break;
  }

  return status;
}

void debug_print_board() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      printf("%lc ", board[i][j]);
    }
    printf("\n");
  }
}

int ask_move() {

  printf("\nMake your move: ");
  scanf("%s", move);
  char from[2];
  char to[2];

  // Look for -
  if (move[2] != '-') {
    printf(RED "Invalid move      ↑ ('-' missing)\n" RESET); return 1;
  }

  //First and 3th should be characters
  if (move[0]-'0' < 10) {
    printf(RED "Invalid move    ↑ (should be letter)\n" RESET); return 1;
  }
  if (move[3]-'0' < 10) {
    printf(RED "Invalid move       ↑ (should be letter)\n" RESET); return 1;
  }

  //Second and 5th character should be numbers
  if (move[1]-'0' > 10) {
    printf(RED "Invalid move     ↑ (should be number)\n" RESET); return 1;
  }
  if (move[1]-'0' > 8) {
    printf(RED "Invalid move     ↑ (out of range)\n" RESET); return 1;
  }
  if (move[4]-'0' > 10) {
    printf(RED "Invalid move        ↑ (should be number)\n" RESET); return 1;
  }
  if (move[4]-'0' > 8) {
    printf(RED "Invalid move        ↑ (out of range)\n" RESET); return 1;
  }

  // Move out of range
  if (move[0]-'0' > 56 || move[3]-'0' > 56) {
    printf(RED "Invalid move (out of range)\n" RESET); return 1;
  }

  move_from[0] = move[0];
  move_from[1] = move[1];
  move_to[0] = move[3];
  move_to[1] = move[4];

  cpmove_from = move_from;
  cpmove_to = move_to;

  return 0;

  printf("from %s, to %s\n", from, to);
}
