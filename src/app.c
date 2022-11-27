#include "app.h"

connection_params params;

void print_connection_params(){ 
    
    if(strcmp(params.user, "anonymous") != 0)
        printf("FTP CONNECTION - user:%s; password:%s; host:%s; url_path:%s\n", params.user, params.password, params.host, params.url_path);
    else 
        printf("FTP CONNECTION - user:anonymous; host:%s; url_path:%s\n", params.host, params.url_path);

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

    int sockfd = open_connection(params.host, FTP_CTRL);

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