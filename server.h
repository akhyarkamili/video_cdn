#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>
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
#include <fcntl.h>
#include "wrapper.h"

/* DEFINITIONS */
#define MAXLINE 2 << 13
#define MAX_BUF 2 << 18
#define __FD_SETSIZE 1024

enum types {SERVER_T = 1, BROWSER_T = 0};
enum conn_states {F4M = 2, NOLIST_F4M = 1, SEGMENT = 0};

/* STRUCTS FOR HTTP PARSING */
typedef struct {
    char *request_header;
    char *request_body;
    int HTTP_VERSION;
    int HTTP_METHOD;
    int CONTENT_LENGTH;
    size_t buf_size;
    char buf[MAX_BUF];
    char uri[MAXLINE];
} request_t;

typedef struct {
    request_t *request;
    int type;
    union {
        int server_fd;
        int browser_fd;
    };
    int fd;
    int state;
    int connection_state;
    size_t line_buf_size;
    char line_buf[MAXLINE];
} client_t;

typedef struct { /* represents a pool of connected descriptors */ 
    int maxfd;        /* largest descriptor in read_set */   

    fd_set read_set;  /* set of all active descriptors to read from */ 
    // read_set needs to be copied to ready_set, which select() will then modify
    fd_set ready_set; /* subset of descriptors ready for reading  */ 

    int nready;       /* number of ready descriptors from select */    
    int maxi;         /* highwater index into client array */ 
    client_t clients[FD_SETSIZE];
} pool_t;

typedef struct {
    int listenfd; /* FD for the server to listen to */
    int dnsfd; /* DNS server socket */
    float alpha;
    char *fake_ip; /* fake ip of the web servers */
    char *fake_port; /* fake port of the web servers */
    char *www_ip; /* www_port of the web servers */
    char *webserverfd;
    pool_t pool;
} server_t;

int server_init(server_t* server, int listenfd, char* alpha, char* fake_ip, 
    char* dns_ip, char* dns_port, char *www_ip);
int server_loop(server_t* server);
void server_end(server_t* server);