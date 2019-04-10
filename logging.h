/* logging.h - A logging module
 * Part of HTTP 1.1 Parser / Checkpoint 2
 * Akhyar Kamili, 02/2019
 */
#ifndef LOGGING_H
#define LOGGING_H

/* Definitions */
enum {INFO = 0, ERROR = 1};

// typedef struct {
// 	int tput;
// 	int bitrate;
// 	// time 
// 	time_t time_download;
// } logger;

void logger_init(const char* file_name);
void logger_log(int log_type, const char *tag, const char* msg);
void logger_request(time_t duration, int tput, int bitrate, char* server_ip, char* chunkname);
void logger_dns(char* client_ip, char* query_name, char *returned_ip);
void logger_close();
#endif