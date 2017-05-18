# C Chess
> Online chess game written in C, using sockets, threads, dynamic memory and more

This program was written for Final School Project for Advanced Programming class.

## Build
**Server**
```
> gcc cchess-server.c board.c -o server -pthread
```
**Client**
```
> gcc cchess-client.c -o client
```

## Run
**Server**
```
> ./server
```
**Client**
```
> ./client localhost 8080
```

## License
MIT &copy; 2017 César Guadarrama Cantú
