/*  
 * proxy.c - A concurrent, event-based
 * proxy using select. 
 * by Akhyar Kamili
 * January 2019
*/ 

#include <sys/select.h>
#include <stdio.h>
#include "server.h"
#include "logging.h"

/* DEFINITIONS */
void sigint_handler(int sig);
// debug mode to ignore the rest of the arguments
#define DEBUG_MODE 0

int byte_cnt = 0; /* counts total bytes received by server */ 
server_t *server;

int main(int argc, char** argv)
{
    int rc, listenfd;
    char www_ip[100];
    

    /* Check if all arguments is passed */
    if (argc != 9 && !DEBUG_MODE) {
        fprintf(stderr, "%s <log> <alpha> <listen-port>"
                        "<fake-ip> <dns-ip> <dns-port> [<www-ip>]\n", argv[0]);
        exit(1);
    }

    /* Enable logging to file */
    logger_init(argv[1]);

    /* Ignore SIGPIPE, handle error in code */
    signal(SIGPIPE, SIG_IGN);
    /* Provides a way to exit cleanly */
    signal(SIGINT, sigint_handler);


    /* Main routine */
    listenfd = open_listenfd(argv[3]);
    server = (server_t *) malloc(sizeof(server_t));
    if(argc == 8) {
        strcpy(www_ip, argv[7]);
    } else {
        strcpy(www_ip, "");
    }
    rc = server_init(server, listenfd, argv[2], argv[4], 
        argv[5], argv[6], www_ip);
    if (rc < 1)
    {
        free(server);
        exit(-1);
    }
    rc = server_loop(server);
    server_end(server);
    free(server);
    return 1;
}

void sigint_handler(int sig)
{
    server_end(server);
    logger_close();
    exit(sig);
}
