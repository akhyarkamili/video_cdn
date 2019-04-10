#include "wrapper.h"
#include "connection.h"
#include "types.h"

typedef struct {
    int listenfd; /* FD for the server to listen to */
    int dnsfd; /* DNS server socket */
    float alpha;
    char *fake_ip; /* fake ip of the web servers */
    char *fake_port; /* fake port of the web servers */
    char *www_ip; /* www_port of the web servers */
    char *www_fd;
    conn_list_t *conn_list;
    pool_t pool;
} server_t;

int server_init(server_t* server, int listenfd, char* alpha, char* fake_ip, 
    char* dns_ip, char* dns_port, char *www_ip);
int server_loop(server_t* server);
void server_end(server_t* server);