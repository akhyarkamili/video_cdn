/*  
 * server.c - Where the magic happens
 * implementation of handling the requests is here
 * by Akhyar Kamili
 * February 2019
*/ 

#include <sys/select.h>
#include <string.h>
#include "server.h"
#include "connection.h"
#include "logging.h"
#include "stdlib.h"
#include "helper.h"
// #include "types.h"

/* GLOBALS */
server_t *SERVER;

/* FORWARD DECLARATIONS */
void init_pool(int listenfd, pool_t *p);
void init_client(client_t *c);
int add_client(int connfd, pool_t *p, int type);
void check_clients(pool_t* p);
void close_client(client_t *client, pool_t *p);

/* CORE DECLARATIONS */

int server_init(server_t *server, int fd, char* alpha, char* fake_ip, 
    char* dns_ip, char* dns_port, char *www_ip) {
    server->listenfd = fd;
    server->dnsfd = open_clientfd(dns_ip, dns_port);
    server->alpha = strtof(alpha, NULL);
    server->fake_ip = fake_ip;
    server->fake_port = 0;

    if(www_ip != NULL)
        server->www_ip = www_ip;

    init_pool(fd, &(server->pool));

    server->conn_list = new_conn_list();

    SERVER = server;
    return 1;
}

/* HELPER DECLARATIONS */

int server_loop(server_t *server) {
    int connfd, rc, listenfd;
    char port[MAXLINE], hostname[MAXLINE], msg[MAXLINE];
    struct sockaddr_storage clientaddr;
    conn_t *conn;
    socklen_t clientlen;

    pool_t *pool = &server->pool;
    listenfd = server->listenfd;
    clientlen = sizeof(clientaddr); 
    msg[0] = '\0';
    while (1) {
        pool->ready_set = pool->read_set;
        pool->nready = Select(pool->maxfd + 1, &pool->ready_set, 
                                        NULL, NULL, NULL);
        sprintf(msg, "Select returned with nready %d\n", pool->nready);
        logger_log(INFO, "SELECT", msg);

        //  A new connection is accepted
        if(FD_ISSET(listenfd, &pool->ready_set))
        {
            connfd = Accept(listenfd, (SA*) &clientaddr, &clientlen);
            rc = add_client(connfd, pool, BROWSER_T);
            if (rc > 0) 
            {
                getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                        port, MAXLINE, 0);
                sprintf(msg, "Accepted connection from (%s, %s)\n", hostname, port);
                logger_log(INFO, "SELECT", msg);
            }

            /* Create new connection */

            conn = new_conn();
            conn->browserfd = connfd;
            insert_conn(SERVER->conn_list, conn);

        }

        // Proceeds to check the clients
        check_clients(pool);
    }
}


void server_end(server_t *server)
{
    free(SERVER);
    return;
}

void init_pool(int listenfd, pool_t *p)
{
    /* initialize the pool */
    int i;
    p->maxi = -1;

    for(i = 0; i < FD_SETSIZE; i++)
    {
        init_client(&p->clients[i]);
    }
    p->maxfd = listenfd;
    FD_ZERO(&(p->read_set));
    FD_SET(listenfd, &(p->read_set));
}

/* init_client: initializes the states, 
 * request and response objects 
 * for each client
 */
void init_client(client_t *cl)
{
    cl->fd = -1;
    cl->state = R_HEADERS;
    cl->line_buf_size = 0;

    cl->request = (request_t *) malloc(sizeof(request_t));
    cl->request->buf_size = 0;
    cl->request->request_header = NULL;
    cl->request->request_body = NULL; 

    // set up zero length string
    cl->request->buf[0] = '\0';
    cl->line_buf[0] = '\0';
}

/* add_client: creates a client object
 * and add it to the read 
 * to be listened for events later
 */
int add_client(int connfd, pool_t *p, int type)
{
    int i;
    p->nready--;

    for(i = 0; i < FD_SETSIZE; i++)
    {
        if (p->clients[i].fd < 0)
        {
            p->clients[i].fd = connfd;
            p->clients[i].type = type;
            FD_SET(connfd, &(p->read_set));
            if (connfd > p->maxfd)
                p->maxfd = connfd;
            if (i > p->maxi)
                p->maxi = i;
            break;
        }
    }

    if(i == FD_SETSIZE)
    {
        /* Nothing done, drop the connection */
        printf("DESCRIPTORS FULL\n");
        return -1;
    }
    return 1;
}
/* check_clients: use the read set
 * returned by select to check the clients
 * for any events that's happened
 */
void check_clients(pool_t *p)
{
    int i, connfd, nread, rc;
    client_t *client;
    char msg[1000];

    sprintf(msg, "Checking clients, p->maxi: %d; p->nready: %d\n", p->maxi, p->nready);
    logger_log(INFO, "server.c: check_clients", msg);

    for(i = 0; (i <= p->maxi) && (p->nready > 0); i++)
    {
        client = &p->clients[i];
        connfd = client->fd;

        if(connfd > 0 && FD_ISSET(connfd, &(p->ready_set)))
        {
            p->nready--;
            client = &p->clients[i];
            
            nread = client_recv(client);
            if(nread <= 0)
            {
                close_client(client, p);
            }
            /* process the received data */
            rc = client_process(client);
            switch(rc)
            {
                case 0:
                    logger_log(INFO, "server.c:check_clients", "Received partial data");\
                    break;
                case 1:
                    logger_log(INFO, "server.c:check_clients", "Success! Connection closing.");
                    close_client(client, p);
                    break;
                default:
                    sprintf(msg, "Error (%d). Closing connection.", rc);
                    logger_log(ERROR, "server.c:check_clients", msg);
                    close_client(client, p);
                    break;
            }
        }
    }
}

/* close_client: close the client and clean up
 */
void close_client(client_t *client, pool_t *p)
{
    char msg[100];

    close(client->fd);
    FD_CLR(client->fd, &p->read_set);

    sprintf(msg, "Client [FD: %d] closed.\n", client->fd);
    logger_log(INFO, "server.c: close_client", msg);

    free(client->request);
    init_client(client);
}