#include "app.h"

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Usage:\tdownload ftp://[<user>:<password>@]<host>/<url-path>\n");
        return -1;
    }

    //print_connection_params();

    app();

    return 0;
}
