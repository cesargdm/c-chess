# C Chess
> Online chess game written in C, using sockets, threads, dynamic memory and more

This program was written for Final School Project for Advanced Programming class.



## Compile

**Server**

```bash
> gcc cchess-server.c board.c -o server -pthread
```

**Client**

```bash
> gcc -pthread cchess-client.c -o client
```

## Run

**Server**

```bash
> ./server
```

**Client**

```bash
> ./client localhost 8080
```

## License
MIT &copy; 2017 César Guadarrama Cantú
