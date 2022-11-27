#include "app.h"

connection_parameters parameters;

int check_and_initialize(char * url){

    if(strstr(url,"ftp://") != url) return -1;

    char * user = NULL;
    char * password = NULL;
    char * hostname = NULL;
    char * url_path = NULL;

    // parse url

    return initialize_connection_parameters(user, password, hostname, url_path);
}

int initialize_connection_parameters(const char * user, const char * password, const char * host, const char * url_path){
    if(hostname == NULL || url_path == NULL) return -1;

    if((user == NULL && password != NULL) ||(user != NULL && password == NULL)) return -1;

    if(user != NULL) {

        parameters.user = (char*) malloc(strlen(user) * sizeof(char));
        strcpy(parameters.user, user);
        parameters.password = (char*) malloc(strlen(password) * sizeof(char));
        strcpy(parameters.password, password);

    } else {
        
        parameters.user = (char*) malloc(strlen("anonymous"));
        parameters.user = "anonymous";
        parameters.password = (char *) malloc(1);
        parameters.password = "";

    }

    struct hostent *h;

    if ((h = gethostbyname(hostname)) == NULL) {
        printf("gethostbyname() failed");
        return -1;
    }

    char* hostip = inet_ntoa(*((struct in_addr *) h->h_addr));

    parameters.host = (char*) malloc(strlen(hostip) * sizeof(char));
    strcpy(parameters.host, hostip);
    parameters.url_path = (char*) malloc(strlen(url_path) * sizeof(char));
    strcpy(parameters.url_path, url_path);

    return 0;

}

void print_connection_parameters(){ 
    
    if(strcmp(parameters.user, "anonymous") != 0)
        printf("FTP CONNECTION - user:%s; password:%s; host:%s; url_path:%s\n", parameters.user, parameters.password, parameters.host, parameters.url_path);
    else 
        printf("FTP CONNECTION - user:anonymous; host:%s; url_path:%s\n", parameters.host, parameters.url_path);

}

int open_connection(char *adress, int port){
    int sockfd;
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(adress);    /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(port);        /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    /*connect to the server*/
    if (connect(sockfd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(-1);
    }

    return sockfd;
}

int app(){

    int sockfd = open_connection(parameters.host, FTP_CTRL);

    /*int result = login(sockfd);

    if (result == 1)
        download_file(sockfd);
    */
    if (close(sockfd)<0) {
        perror("close()");
        exit(-1);
    }

    return 0;
}