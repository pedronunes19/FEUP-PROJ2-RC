#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <libgen.h>


#define FTP_CTRL 21
#define MAX_MSG_SIZE 256

#define OPEN_CONNECTION 150
#define READY_USER 220
#define TRANFER_COMPLETE 226
#define PASSIVE_MODE 227
#define LOGGED_IN 230
#define READY_PASSWORD 331

typedef struct {
    char * user;
    char * password;
    char * host;
    char * url_path;
} connection_parameters;

int parse_url(char * url);

int initialize_connection_parameters(const char * user, const char * password, const char * host, const char * url_path);

int open_connection(char *adress, int port);

int get_response_code(FILE* file);

int login(int fd);

int download(int fd);

int app();