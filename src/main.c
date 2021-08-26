#include <stdio.h>
#include "lota.h"

int main(int argc, char *agrv)
{
    printf(" _      ____ _______         \n");
    printf("| |    / __ \\__   __|/\\    \n");
    printf("| |   | |  | | | |  /  \\    \n");
    printf("| |   | |  | | | | / /\\ \\  \n");
    printf("| |___| |__| | | |/ ____ \\  \n");
    printf("|______\\____/  |_/_/    \\_\\ \n");
    printf("\n");

    char *url = "http://static.getiot.tech/flag-of-china.png";
    char *file = "./test.png";

    lota_download(url, file);

    return 0;
}
