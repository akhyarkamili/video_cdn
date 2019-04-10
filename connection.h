#include "segment.h"
#include <time.h>

typedef struct conn {
    int serverfd;
    int browserfd;
    int state;
    int throughput;
    time_t last_request_time;
    segment_list_t *segments;
    struct conn *next;
    struct conn *prev;
} conn_t;

typedef struct {
    conn_t *head;
    conn_t *tail;
    int size;
} conn_list_t;

/* functions for the connection list structure */
conn_list_t *new_conn_list();
conn_t *find_by_browser(conn_list_t *cl, int browserfd);
conn_t *find_by_server(conn_list_t *cl, int serverfd);
void insert_conn(conn_list_t *cl, conn_t *c);
void remove_conn(conn_list_t *cl, conn_t *c);

/* functions for a connection */
conn_t *new_conn();