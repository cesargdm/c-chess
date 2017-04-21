#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <wchar.h>
#include <locale.h>

#include "board.c"

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

int main(int argc, char const *argv[]) {

  initiate_board();
  print_board();


  while (1) {
    while (ask_move());
    make_move();
    print_board();
  }


  return 0;
}

void translate_move(char * cmove, int * imove) {

  // printf("move: '%s'\n", cmove);
  // printf("cmove[0] '%c' (d)%d (dn)%d\n", cmove[0], cmove[0], cmove[0]-'0');
  // printf("cmove[0] (n)%d\n", (*cmove-'0')-49);
  // printf("move[1]: '%d'\n", *(move+1)-'0');
  // printf("move[1] '%c' (d)%d (c->d)%d\n", cmove[1], cmove[1], cmove[1]-'0');
  // printf("move[1] (n)%d\n", 8-(cmove[1]-'0'));
  // printf("\n");

  int x = (*(cmove)-'0')-49;


  int y = 8-(*(cmove+1)-'0');

  *(imove) = y;
  *(imove+1) = x;

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

int get_piece_team(int * piece_position) {
  wchar_t piece = board[*(piece_position)][*(piece_position+1)];
  if (piece == (white_king | white_queen | white_rook | white_bishop | white_knight | white_pawn)) {
    printf("Piece is in team 0\n");
    return 0;
  }
  if (piece == (black_king | black_queen | black_rook | black_bishop | black_knight | black_pawn)) {
    printf("Piece is in team 1\n");
    return 1;
  }
  return 2;
}

int check_move(int * move_from, int * move_to) {

  wchar_t piece_in_origin = board[*move_from][*(move_from+1)];
  int team_in_origin = get_piece_team(move_from);

  wchar_t piece_in_dest = board[*move_to][*(move_to+1)];
  int team_in_dest = get_piece_team(move_to);

  int x_moves = *(move_from) - *(move_to);
  int y_moves = *(move_from+1) - *(move_to+1);

  // if (*move_to > 7 || *move_to < 0 || *(move_to+1) > 7 || *(move_to+1) < 0) {
  //   return 2;
  // }

  //printf("x moves: %d, y moves: %d\n", x_moves, y_moves);

  if (piece_in_origin == 0) {
    return 1;
  }

  int piece_type = get_piece_type(piece_in_origin);

  printf("piece type: %d\n", piece_type);

  switch (piece_type) {
    case 0: // ♚
      break;
    case 1: // ♛
      break;
    case 2: // ♜
      printf("Case 2\n");
      if ((y_moves > 0 && x_moves != 0) || (x_moves > 0 && y_moves != 0)) return 2;
      break;
    case 3: // ♝
      break;
    case 4: // ♞
      if ((abs(x_moves) != 1 || abs(y_moves) != 2) && (abs(x_moves) != 2 || abs(y_moves) != 1)) return 4;
      break;
    case 5: // ♟
      if (x_moves > 1 || y_moves > 0) return 5;
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
  printf("status %d\n", status);

  switch (status) {
    case 0:
      board[*imove_to][*(imove_to+1)] = board[*imove_from][*(imove_from+1)];
      board[*imove_from][*(imove_from+1)] = 0;
      break;
    case 1:
      printf(RED "Nothing selected" RESET);
      break;
    case 4:
      printf(RED "Invalid knight move" RESET);
      break;
    default:
      printf(RED "Invalid move" RESET);
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
