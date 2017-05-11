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
#define black_bishop 0x265D // ♝
#define black_knight 0x265E // ♞
#define black_pawn   0x265F // ♟

static void debug_print_board(wchar_t **);

static wchar_t ** create_board() {
  // create dinamically malloc

  wchar_t ** board = (wchar_t **) malloc(sizeof(wchar_t *)*8);
  for (int i = 0; i < 8; i++) {
    board[i] = (wchar_t *) malloc(sizeof(wchar_t)*8);
  }

  return board;
}

static void initialize_board(wchar_t ** p_board) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      if (i == 0) {
        if (j == 0 || j == 7) p_board[i][j] = white_rook;
        if (j == 1 || j == 6) p_board[i][j] = white_knight;
        if (j == 2 || j == 5) p_board[i][j] = white_bishop;
        if (j == 3) p_board[i][j] = white_queen;
        if (j == 4) p_board[i][j] = white_king;
      }

      if (i == 1) p_board[i][j] = white_pawn;

      if (i > 1 && i < 6) p_board[i][j] = 0;

      if (i == 6) p_board[i][j] = black_pawn;
      if (i == 7) {
        if(j == 0 || j == 7) p_board[i][j] = black_rook;
        if(j == 1 || j == 6) p_board[i][j] = black_knight;
        if(j == 2 || j == 5) p_board[i][j] = black_bishop;
        if(j == 3) p_board[i][j] = black_queen;
        if(j == 4) p_board[i][j] = black_king;
      }
    }
  }
}

static char * create_od_board() {
  char * board = (char *) malloc(sizeof(char *)*8*8);
  return board;
}

static void to_one_dimension_char(wchar_t ** board, char * od_board) {

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i][j] == white_rook) od_board[8*i+j] = '1';
      else if (board[i][j] == white_knight) od_board[8*i+j] = '2';
      else if (board[i][j] == white_bishop) od_board[8*i+j] = '3';
      else if (board[i][j] == white_queen) od_board[8*i+j] = '4';
      else if (board[i][j] == white_king) od_board[8*i+j] = '5';
      else if (board[i][j] == white_pawn) od_board[8*i+j] = '6';
      else if (board[i][j] == black_rook) od_board[8*i+j] = '7';
      else if (board[i][j] == black_knight) od_board[8*i+j] = '8';
      else if (board[i][j] == black_bishop) od_board[8*i+j] = '9';
      else if (board[i][j] == black_queen) od_board[8*i+j] = 'a';
      else if (board[i][j] == black_king) od_board[8*i+j] = 'b';
      else if (board[i][j] == black_pawn) od_board[8*i+j] = 'c';
      else od_board[8*i+j] = '0';
    }
  }

}

static void free_od_board(wchar_t * od_board) {
  free(od_board);
}

static void free_board(wchar_t ** board) {
  for (int i = 0; i < 8; i++) {
    free(board[i]);
  }
  free(board);
}

static void debug_print_board(wchar_t ** board) {
  setlocale( LC_ALL, "en_US.UTF-8" );
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i][j] != 0) { printf("%lc", board[i][j]); }
      else { printf(" "); }
      printf(" ");
    }
    printf("\n");
  }
}

static wchar_t translate_piece (char char_piece) {
  if (char_piece == '1') return white_rook;
  else if (char_piece == '2') return white_knight;
  else if (char_piece == '3') return white_bishop;
  else if (char_piece == '4') return white_queen;
  else if (char_piece == '5') return white_king;
  else if (char_piece == '6') return white_pawn;
  else if (char_piece == '7') return black_rook;
  else if (char_piece == '8') return black_knight;
  else if (char_piece == '9') return black_bishop;
  else if (char_piece == 'a') return black_queen;
  else if (char_piece == 'b') return black_king;
  else if (char_piece == 'c') return black_pawn;
  else return ' ';
}

static void print_board_buff(char * board) {
  setlocale( LC_ALL, "en_US.UTF-8" );
  printf("\n             ┌───────────────────┐          \n");
  printf("             │       C*CHESS     │          \n");
  printf("             └───────────────────┘          \n\n");
  printf("         a   b   c   d   e   f   g   h     \n");
  printf("       ┌───┬───┬───┬───┬───┬───┬───┬───┐   \n");
  printf("     8 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 8 \n", translate_piece(board[0]), translate_piece(board[1]), translate_piece(board[2]), translate_piece(board[3]), translate_piece(board[4]), translate_piece(board[5]), translate_piece(board[6]), translate_piece(board[7]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     7 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 7 \n", translate_piece(board[8]), translate_piece(board[9]), translate_piece(board[10]), translate_piece(board[11]), translate_piece(board[12]), translate_piece(board[13]), translate_piece(board[14]), translate_piece(board[15]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     6 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 6 \n", translate_piece(board[16]), translate_piece(board[17]), translate_piece(board[18]), translate_piece(board[19]), translate_piece(board[20]), translate_piece(board[21]), translate_piece(board[22]), translate_piece(board[23]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     5 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 5 \n", translate_piece(board[24]), translate_piece(board[25]), translate_piece(board[26]), translate_piece(board[27]), translate_piece(board[28]), translate_piece(board[29]), translate_piece(board[30]), translate_piece(board[31]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     4 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 4 \n", translate_piece(board[32]), translate_piece(board[33]), translate_piece(board[34]), translate_piece(board[35]), translate_piece(board[36]), translate_piece(board[37]), translate_piece(board[38]), translate_piece(board[39]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     3 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 3 \n", translate_piece(board[40]), translate_piece(board[41]), translate_piece(board[42]), translate_piece(board[43]), translate_piece(board[44]), translate_piece(board[45]), translate_piece(board[46]), translate_piece(board[47]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     2 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 2 \n", translate_piece(board[48]), translate_piece(board[49]), translate_piece(board[50]), translate_piece(board[51]), translate_piece(board[52]), translate_piece(board[53]), translate_piece(board[54]), translate_piece(board[55]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     1 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 1 \n", translate_piece(board[56]), translate_piece(board[57]), translate_piece(board[58]), translate_piece(board[59]), translate_piece(board[60]), translate_piece(board[61]), translate_piece(board[62]), translate_piece(board[63]));
  printf("       └───┴───┴───┴───┴───┴───┴───┴───┘   \n");
  printf("         a   b   c   d   e   f   g   h     \n\n");
}

static void print_board_buff_inverted(char * board) {
  setlocale( LC_ALL, "en_US.UTF-8" );
  printf("\n             ┌───────────────────┐          \n");
  printf("             │       C*CHESS     │          \n");
  printf("             └───────────────────┘          \n\n");
  printf("         a   b   c   d   e   f   g   h     \n");
  printf("       ┌───┬───┬───┬───┬───┬───┬───┬───┐   \n");
  printf("     1 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 8 \n", translate_piece(board[56]), translate_piece(board[57]), translate_piece(board[58]), translate_piece(board[59]), translate_piece(board[60]), translate_piece(board[61]), translate_piece(board[62]), translate_piece(board[63]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     2 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 7 \n", translate_piece(board[48]), translate_piece(board[49]), translate_piece(board[50]), translate_piece(board[51]), translate_piece(board[52]), translate_piece(board[53]), translate_piece(board[54]), translate_piece(board[55]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     3 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 6 \n", translate_piece(board[40]), translate_piece(board[41]), translate_piece(board[42]), translate_piece(board[43]), translate_piece(board[44]), translate_piece(board[45]), translate_piece(board[46]), translate_piece(board[47]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     4 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 5 \n", translate_piece(board[32]), translate_piece(board[33]), translate_piece(board[34]), translate_piece(board[35]), translate_piece(board[36]), translate_piece(board[37]), translate_piece(board[38]), translate_piece(board[39]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     5 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 4 \n", translate_piece(board[24]), translate_piece(board[25]), translate_piece(board[26]), translate_piece(board[27]), translate_piece(board[28]), translate_piece(board[29]), translate_piece(board[30]), translate_piece(board[31]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     6 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 3 \n", translate_piece(board[16]), translate_piece(board[17]), translate_piece(board[18]), translate_piece(board[19]), translate_piece(board[20]), translate_piece(board[21]), translate_piece(board[22]), translate_piece(board[23]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     7 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 2 \n", translate_piece(board[8]), translate_piece(board[9]), translate_piece(board[10]), translate_piece(board[11]), translate_piece(board[12]), translate_piece(board[13]), translate_piece(board[14]), translate_piece(board[15]));
  printf("       ├───┼───┼───┼───┼───┼───┼───┼───┤   \n");
  printf("     8 │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ %lc │ 1 \n", translate_piece(board[0]), translate_piece(board[1]), translate_piece(board[2]), translate_piece(board[3]), translate_piece(board[4]), translate_piece(board[5]), translate_piece(board[6]), translate_piece(board[7]));
  printf("       └───┴───┴───┴───┴───┴───┴───┴───┘   \n");
  printf("         a   b   c   d   e   f   g   h     \n\n");
}
