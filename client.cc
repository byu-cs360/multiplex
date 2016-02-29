#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <fstream>
#include <iostream>

using namespace std;

int
main(int argc, char **argv)
{
    struct sockaddr_in server;
    int s, port;
    string host,line;
    char *buf = (char *) calloc(1024,sizeof(char));

      // Get port passed on the command line.  Normally you should use
      // getopt() or similar command parsing.

    if (argc < 3) {
        cout << "client <IP address> <port>" << endl;
        exit(-1);
    }
    host = argv[1];
    port = atoi(argv[2]);

      // use DNS to get host name
    struct hostent *hostEntry;
    hostEntry = gethostbyname(host.c_str());
    if (!hostEntry) {
        cout << "No such host name: " << host << endl;
        exit(-1);
    }

      // setup socket address structure
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    memcpy(&server.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);

      // Create socket
    s = socket(PF_INET,SOCK_STREAM,0);
    if (!s) {
        perror("socket");
        exit(-1);
    }

      // Connect to server
    if (connect(s,(const struct sockaddr *)&server,sizeof(server)) < 0) {
        perror("connect");
        exit(-1);
    }

    while (getline(cin,line)) {

          // write the data to the server
        send(s, line.c_str(), line.length(), 0);

          // read the response
        memset(buf,0,1024);
        recv(s,buf,1024,0);
        
          // print the response
        cout << buf << endl;
    }

      // Close socket
    close(s);
}

