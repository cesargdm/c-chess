#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#include <netdb.h>
#include <arpa/inet.h>

//Definitions
#define SERVICE_PORT 8642
#define MAX_QUEUE 5

//Function declarations
void usage(char *);
void startServer(char *);
void wait_for_connections(int);
void listen_loop(int);


int on_connection() {
  printf("Connection recieved\n");
  // Send new client_id
}

int emmit(int event, char * client_id, char const *data[]) {
  // Data == board status
  switch (event) {
    case 0:
      // SEnd to client_id the data passed thru
    break
  }
  return 0;
}

int main(int argc, char const *argv[]) {

  int match_connections = 0;

  printf("–––      C    C H E S S      –––\n");
  printf("           s e r v e r           \n");

  return 0;
}
