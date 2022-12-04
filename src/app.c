#include "app.h"

connection_parameters parameters;

int check_parameters(char * url){

    if(strstr(url,"ftp://") != url) 
        return -1;

    char * user = NULL;
    char * password = NULL;
    char * hostname = NULL;
    char * url_path = NULL;

    char * no_header = url + 6;  // exclude ftp header

    unsigned int url_size = strlen(no_header); // needed before strtok 

    char * host = strtok(no_header, "/");

    if (strlen(host) == url_size){
        url_path = malloc(1);
        url_path = "";

    } else url_path = no_header + strlen(host) + 1;

    if(strstr(host, "@") != NULL) {

        char * auth = strtok(host, "@");
        hostname = strtok(NULL, "@");

        user = strtok(auth, ":");
        password = strtok(NULL, ":");

    } else hostname = host;

    return initialize_connection_parameters(user, password, hostname, url_path);
}

int initialize_connection_parameters(const char * user, const char * password, const char * host, const char * url_path){
    if(host == NULL || url_path == NULL) return -1;

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

    if ((h = gethostbyname(host)) == NULL) {
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
        printf("FTP CONNECTION \n\t- user:%s\n\t- password:%s\n\t- host:%s\n\t- url_path:%s\n", parameters.user, parameters.password, parameters.host, parameters.url_path);
    else 
        printf("FTP CONNECTION \n\t- user:anonymous\n\t- host:%s\n\t- url_path:%s\n", parameters.host, parameters.url_path);

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

int get_response_code(FILE* file){
    char *line = malloc(MAX_LINE_SIZE);
    int code = -1;
    size_t n;
    
    while (getline(&line, &n, file) != -1) {
        printf("%s\n",line);
        if (line[3] == ' ') {
            code = atoi(line); // integer value of string = response code
            break;
        }
    }

    free(line);

    return code;
}

int login (int fd){
    FILE *file = fdopen(fd, "r");

    char message[MAX_LINE_SIZE];
    int res = -1;

    while(res != LOGGED_IN){
        res = get_response_code(file);
        switch(res){
            case READY_USER:
                sprintf(message, "user %s\n", parameters.user);
                break;
            case USER_OK_PASSWORD:
                sprintf(message, "pass %s\n", parameters.password);
                break;
            case LOGGED_IN:
                sprintf(message, "pasv\n");
                break;
            default:
                return -1;
        }

        printf("%s\n", message);
        write(fd, message, strlen(message));
    }

    return res;

}

int app(){

    int sockfd = open_connection(parameters.host, FTP_CTRL);


    if (login(sockfd) == LOGGED_IN){
        printf("DEBUG - successfully logged in and set passive mode\n");
        //download(sockfd);
    }


    if (close(sockfd)<0) {
        perror("close()");
        exit(-1);
    }

    return 0;
}