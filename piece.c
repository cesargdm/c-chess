#include <wchar.h>

/* Retrieve the piece team */
int get_piece_team(int * piece_position) {

  wchar_t piece = board[*(piece_position)][*(piece_position+1)];

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

// Return the piece in a int identifier
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
