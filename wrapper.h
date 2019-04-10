#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct sockaddr SA;
#define LISTENQ  1024  /* Second argument to listen() */

// taken from csapp library from 15-213
int Select(int  n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, 
	   struct timeval *timeout);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
void Close(int fd);
void Setsockopt(int s, int level, int optname, const void *optval, int optlen);
int open_listenfd(char *port);
int open_clientfd(char *hostname, char *port);


