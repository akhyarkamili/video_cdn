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


/* ENUMERATIONS */
enum states {R_HEADERS = 1, R_BODY = 2};
enum HTTP_METHODS {GET = 0, POST = 1, HEAD = 2};
enum HTTP_CODES {
    HTTP_OK = 200,
    HTTP_BAD_REQUEST = 400,
    HTTP_NOT_FOUND = 404,
    HTTP_REQUEST_TIMEOUT = 408,
    HTTP_INTERNAL_SERVER_ERROR = 500,
    HTTP_NOT_IMPLEMENTED = 501,
    HTTP_VERSION_NOT_SUPPORTED = 505
};

server_t *SERVER;