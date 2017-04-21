#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>

#include "board.c"
#include "piece.c"

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define MAX_MOVE_LENGTH 256

char move[MAX_MOVE_LENGTH];

char * cpmove_from;
char * cpmove_to;

int imove_from[2];
int imove_to[2];

char move_from[2];
char move_to[2];

int ask_move();
int make_move();
void debug_print_board();
void translate_move(char *, int *);
void move_piece(int *, int *);

int main(int argc, char const *argv[]) {

  // Print the board
  print_board();

  while (1) {
    while (ask_move());
    make_move();
    print_board();
  }

  return 0;
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

/* Check if the diagonal move is clear one position before dest */
int is_diagonal_clear(int * origin, int * dest) {
  // Invalid diagonal
  if (((abs(*(origin)) - abs(*(dest))) != 0) ||( (abs(*(origin+1)) - abs(*(dest+1))) != 0)) {
    return false;
  }

  // TODO: Parallel?
  for (int i = *origin; i < *dest; i++) {
    /* code */
  }

  return true;
}

/* Check if the rect move is clear one position before dest */
int is_rect_clear(int * origin, int * dest) {
  return true;
}

bool eat_piece(int * position) {
  // Check if there's a piece there
  // if (board[*(position)][*(position+1)] == 0) return false;
  // Add piece to the graveyard
  printf(RED "Eating piece..." RESET);
  return true;
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
  printf("Pieces : %lc(o) %lc(d)\n", piece_in_origin, piece_in_dest);
  printf("Teams  : %d(o) %d(d)\n", team_in_dest, team_in_origin);
  printf("[%d,%d]\n", *(origin), *(origin+1));
  printf("piece type: %d\n", origin_piece_type);

  /*  --- GENERAL RULES --- */
  // If selected piece == 0 there's nothing selected
  if (piece_in_origin == 0) {
    return 1;
  }

  // If the origin piece's team == dest piece's team is an invalid move
  if (team_in_origin == team_in_dest) {
    return 2;
  }

  /* --- SPECIFIC RULES --- */
  switch (origin_piece_type) {
    case 0: // ♚
      if (abs(x_moves) > 1 || abs(y_moves) > 1) return 0;
      break;
    case 1: // ♛
      //Diagonal or straight move
      break;
    case 2: // ♜
      if ((y_moves > 0 && x_moves != 0) || (x_moves > 0 && y_moves != 0)) return 20;
      break;
    case 3: // ♝
      if ((abs(x_moves)-abs(y_moves)) != 0) return 30;
      printf("break one\n");
      break;
    case 4: // ♞
      // Check if if it's a knight's valid move
      if ((abs(x_moves) != 1 || abs(y_moves) != 2) && (abs(x_moves) != 2 || abs(y_moves) != 1)) return 40;
      break;
    case 5: // ♟
      if (y_moves != 0) {
        // Check if it's a diagonal move and it's not an empty location
        if ((abs(y_moves) == 1 && abs(x_moves == 1)) && (team_in_dest != 0)) {
          if (eat_piece(dest)) return 99;
        }
        return 50;
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
  printf("status %d\n", status);

  switch (status) {
    case 0: //Simple move
      move_piece(imove_from, imove_to);
      break;
    case 1:
      printf(RED "Nothing selected" RESET);
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

// int can_move(int x, int y, int newX, int newY) {
//
// }
