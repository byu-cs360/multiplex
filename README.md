# Multiplex

Example code showing how to use the select(), poll(), fork(), and
pthread_create() system calls to handle multiple clients on a simple
echo server.

DANGER: The select() and poll() code handles one socket event at a
time.  This means that a client that requires a large amount of work
will affect the response time of other clients.

DANGER: The fork() code will spawn a new process for each client.  The
pthread_create() code will spawn a new thread for each client.  Under
heavy load, this will exhaust system resources.

A real implementation would use a process pool, thread pool, or other
advanced architecture.

## Contents

1) Makefile - Will compile the code and keep track of dependencies, so
that the code is recompiled correctly when any of the source code is
modified.

make all
(will compile all the code)
make clean
(will remove object files)
make realclean
(will remove object files, dependency files, and the executable)

2) select.cc/poll.cc/fork.cc/thread.cc - A server that will echo back
whatever a client sends it.  All versions can handle multiple clients
connected simultaenously.  Two versions of the server use event-driven
multiplexing -- with select() or poll().  Another version creates a
separate process for each client, using fork().  A final version
creates a separate thread for each client, using pthreads.  Use
control-C to terminate the server.

Syntax:

```
./select <port>
./poll <port>
./fork <port>
```

Examples:

```
./select 8000
```

3) client.cc - A client that will connect to the echo server.  It will
then read standard input, send this to the server, and print its
response.  Use control-D to terminate the server.

Note that I have intentionally ignored the return value of send() and
recv() in this code, in order to simplify it.  We discuss in class how
to correctly send and receive data on a socket.

Syntax:

```
./client <host> <port>
```

Examples:

```
./client localhost 8000
```

This will connect to the server running on the same machine, using
the loopback address.

```
./client carmelo.cs.byu.edu 8000
```

This will connect to the server running on a different machine, assuming
it is running at that address.
