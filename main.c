#include "app.h"

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Usage:\tdownload ftp://[<user>:<password>@]<host>/<url-path>\n");
        return -1;
    }

    if (check_parameters(argv[1]) != 0){
        printf("Invalid parameters\n");
        printf("Usage:\tdownload ftp://[<user>:<password>@]<host>/<url-path>\n");
        return -1;
    }

    app();

    return 0;
}
