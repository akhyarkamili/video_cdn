
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logging.h"


/* Global Variables */
FILE *log_file;

/**
 * @brief      initializes logger
 *
 * @param[in]  file_name  path where logger file is written
 */
void logger_init(const char *file_name)
{
    time_t now; 
    time(&now);

    log_file = fopen(file_name, "w");
    fprintf(log_file, "Logging started on %s", ctime(&now));
    fflush(log_file);
}


/**
 * @brief      Fills logger file
 *
 * @param[in]  log_type  The log type
 * @param[in]  tag       The tag
 * @param[in]  msg       The message
 */
void logger_log(int log_type, const char *tag, const char* msg)
{
    time_t now; int rc;
    time(&now);
    switch(log_type) {
        case INFO:
            rc = fprintf(log_file ,"%s - [%s]: %s\n", ctime(&now), tag, msg);
            fflush(log_file);
            if(!rc) exit(1);
            break;
        case ERROR:
            fprintf(log_file ,"(ERROR)  %s - [%s]: %s\n", ctime(&now), tag, msg);
            fflush(log_file);
            break;
        default:
            fprintf(log_file ,"%s - [%s]: %s\n", ctime(&now), tag, msg);
    }
}




/**
 * @brief      { function_description }
 *
 * @param[in]  tput       The tput
 * @param[in]  avg_tput   The average tput
 * @param[in]  bitrate    The bitrate
 * @param      server_ip  The server ip
 * @param      chunkname  The chunkname
 */
void logger_request(time_t duration, int tput, int bitrate, char* server_ip, char* chunkname){
    time_t now;

    now = time(NULL);
    int avg_tput;

    avg_tput = 0;    

    fprintf(log_file, "<%ld> <%ld> <%d> <%d> <%d> <%s> <%s>\n", 
    now, duration, tput, avg_tput, bitrate, server_ip, chunkname);
    fflush(log_file);
}


/**
 * @brief      Fills logger file in DNS operation
 *
 * @param      client_ip   The client ip
 * @param      query_name  The query name
 */
void logger_dns(char* client_ip, char* query_name, char *returned_ip){
    time_t now;
    // char* returned_ip;
    // struct hostent *hstnm;

    // hstnm = gethostbyname(query_name);
    // returned_ip = inet_ntoa(*(struct in_addr *)hstnm->h_name);

    now = time(NULL);

    fprintf(log_file, "<%ld> <%s> <%s> <%s>\n", now, client_ip, query_name, returned_ip);
    fflush(log_file);
}


/**
 * @brief      closes log file
 */
void logger_close()
{
    fclose(log_file);
}