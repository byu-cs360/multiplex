#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>
#include <set>

using namespace std;

void* handle(void*);

int
main(int argc, char **argv)
{
    struct sockaddr_in server,client;
    socklen_t clientlen = sizeof(client);
    int i, s, c, port;
    int opt;

      // Get port passed on the command line.  Normally you should use
      // getopt() or similar command parsing.

    if (argc < 2) {
        cout << "server <port>" << endl;
        exit(-1);
    }
    port = atoi(argv[1]);

      // setup socket address structure
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

      // create socket
    s = socket(PF_INET,SOCK_STREAM,0);
    if (!s) {
        perror("socket");
        exit(-1);
    }

      // set socket to reuse port from bind
    opt = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(-1);
    }

      // 
    if (bind(s,(const struct sockaddr *)&server,sizeof(server)) < 0) {
        perror("bind");
        exit(-1);
    }
    if (listen(s,SOMAXCONN) < 0) {
        perror("listen");
        exit(-1);
    }
    
    while (1) {
          // accept a new client
        c = accept(s,(struct sockaddr *)&client,&clientlen);
        if (c < 0) {
            perror("accept");
            break;
        }

          // create a thread to handle the connection
        pthread_t tid;
        int* client = new int;
        *client = c;
        pthread_create(&tid, NULL, &handle, client);
    }
}

  // Handle a client.  In this case, we do a simple echo. Return
  // false if an error occurs.
void*
handle(void* vptr)
{
    int nread,nwritten,nleft;
    char *buf = (char *) calloc(1024,sizeof(char));
    char *ptr;

      // grab data and free memory
    int client = *((int *) vptr);
    delete (int*) vptr;

    while (1) {
          // read data
        memset(buf,0,1024);
        nread = recv(client,buf,1024,0);
        if (nread < 0) {
            if (errno == EINTR)
                continue;
            else
                return NULL;
        } else if (nread == 0) {
            return NULL;
        }
          // write the same data back
        ptr = buf;
        nleft = nread;
        while (nleft) {
            nwritten = send(client, ptr, nleft, 0);
            if (nwritten < 0) {
                if (errno == EINTR) {
                    nwritten = 0;
                } else {
                    return NULL;
                }
            } else if (nwritten == 0) {
                  // the socket is closed
                return NULL;
            }
            nleft -= nwritten;
            ptr += nwritten;
        }
    }
}
