# C Chess

> Online chess game written in C, using sockets, threads, dynamic memory and more

This program was written for Final School Project for Advanced Programming class in Tec de Monterrey.

![Animation of a chess move](https://github.com/cesargdm/c-chess/blob/main/c-chess.gif?raw=true)

## Compile

### Server

```bash
> gcc cchess-server.c board.c -o server -pthread
```

### Client

```bash
> gcc cchess-client.c -o client -pthread
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

MIT. César Guadarrama.
