#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define FTP_CTRL 21
#define MAX_LINE_SIZE 256

#define OPEN_CONNECTION 150
#define READY_USER 220
#define FILE_ACTION_SUCCESS 226
#define PASSIVE_MODE 227
#define LOGGED_IN 230
#define USER_OK_PASSWORD 331

typedef struct {
    char * user;
    char * password;
    char * host;
    char * url_path;
} connection_parameters;

int check_parameters(char * url);

int initialize_connection_parameters(const char * user, const char * password, const char * host, const char * url_path);

void print_connection_parameters();

int open_connection(char *adress, int port);

int login(int fd);

int download(int fd);

int app();