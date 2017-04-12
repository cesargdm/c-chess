#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <wchar.h>
#include <locale.h>

#include "board.c"

#define MAX_MOVE_LENGTH 256

char move[MAX_MOVE_LENGTH];
void ask_move();

int main(int argc, char const *argv[]) {

  initiate_board();
  print_board();
  ask_move();

  while (0) {
    printf("Asking move...\n");
    ask_move();
    print_board();
  }

  return 0;
}

void ask_move() {
  printf("\nMake your move: ");
  scanf("%s", move);
  printf("move: %s\n", move);
}

// int can_move(int x, int y, int newX, int newY) {
//
// }
