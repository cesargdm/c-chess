# C Chess
> Online chess game written in C, using sockets, threads, dynamic memory and more

This program was written for Final School Project for Advanced Programming class.

![Animation of a chess move](https://github.com/cesargdm/c-chess/blob/main/c-chess.gif?raw=true)

## Compile

### Server

```bash
> gcc cchess-server.c board.c -o server -pthread
```

### Client

```bash
> gcc -pthread cchess-client.c -o client
```

## Run

### Server

```bash
> ./server
```

### Client

```bash
> ./client localhost 8080
```

## Playing

You first need to run a server, after that you'll need to run two clients and start making your master chess moves ♟️.

## License

MIT &copy; 2017 César Guadarrama Cantú
