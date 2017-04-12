#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

#define white_king   0x2654 // ♔
#define white_queen  0x2655 // ♕
#define white_rook   0x2656 // ♖
#define white_bishop 0x2657 // ♗
#define white_knight 0x2658 // ♘
#define white_pawn   0x2659 // ♙
#define black_king   0x265A // ♚
#define black_queen  0x265B // ♛
#define black_rook   0x265C // ♜
#define black_bishop 0x265D // ♗
#define black_knight 0x265E // ♞
#define black_pawn   0x265F // ♟

static wchar_t board [8][8] = {
  { white_rook, white_knight, white_bishop, white_queen, white_king, white_bishop, white_knight, white_rook },
  { white_pawn, white_pawn, white_pawn, white_pawn, white_pawn, white_pawn, white_pawn, white_pawn },
  {},
  {},
  {},
  {},
  { black_pawn, black_pawn, black_pawn, black_pawn, black_pawn, black_pawn, black_pawn, black_pawn },
  { black_rook, black_knight, black_bishop, black_queen, black_king, black_bishop, black_knight, black_rook }
};

static int piece_at(int x, int y) {
  wchar_t piece = board[x][y];
  if (piece == 0) {
    piece = ' ';
  }
  return piece;
}

static void initiate_board() {
  printf("Initiating board...\n");
}

static void print_board() {
  setlocale( LC_ALL, "en_US.UTF-8" );

  system("clear");
  printf("     a   b   c   d   e   f   g   h     \n");
  printf("   ┌───┬───┬───┬───┬───┬───┬───┬───┐   \n");
  printf(" 8 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 8 \n", piece_at(0,0), piece_at(0,1), piece_at(0,2), piece_at(0,3), piece_at(0,4), piece_at(0,5), piece_at(0,6), piece_at(0,7));
  printf("   ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf(" 7 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 7 \n", piece_at(1,0), piece_at(1,1), piece_at(1,2), piece_at(1,3), piece_at(1,4), piece_at(1,5), piece_at(1,6), piece_at(1,7));
  printf("   ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf(" 6 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 6 \n", piece_at(2,0), piece_at(2,1), piece_at(2,2), piece_at(2,3), piece_at(2,4), piece_at(2,5), piece_at(2,6), piece_at(2,7));
  printf("   ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf(" 5 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 5 \n", piece_at(3,0), piece_at(3,1), piece_at(3,2), piece_at(3,3), piece_at(3,4), piece_at(3,5), piece_at(3,6), piece_at(3,7));
  printf("   ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf(" 4 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 4 \n", piece_at(4,0), piece_at(4,1), piece_at(4,2), piece_at(4,3), piece_at(4,4), piece_at(4,5), piece_at(4,6), piece_at(4,7));
  printf("   ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf(" 3 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 3 \n", piece_at(5,0), piece_at(5,1), piece_at(5,2), piece_at(5,3), piece_at(5,4), piece_at(5,5), piece_at(5,6), piece_at(5,7));
  printf("   ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf(" 2 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 2 \n", piece_at(6,0), piece_at(6,1), piece_at(6,2), piece_at(6,3), piece_at(6,4), piece_at(6,5), piece_at(6,6), piece_at(6,7));
  printf("   ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf(" 1 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 1 \n", piece_at(7,0), piece_at(7,1), piece_at(7,2), piece_at(7,3), piece_at(7,4), piece_at(7,5), piece_at(7,6), piece_at(7,7));
  printf("   └───┴───┴───┴───┴───┴───┴───┴───┘   \n");
  printf("     a   b   c   d   e   f   g   h     \n");

}
