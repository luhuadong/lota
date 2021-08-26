#include <stdio.h>
#include "lota.h"

static const char *url = "https://static.getiot.tech/flag-of-china.png";
static const char *file = "./test.png";

int main(int argc, char *agrv[])
{
    printf(" _      ____ _______         \n");
    printf("| |    / __ \\__   __|/\\    \n");
    printf("| |   | |  | | | |  /  \\    \n");
    printf("| |   | |  | | | | / /\\ \\  \n");
    printf("| |___| |__| | | |/ ____ \\  \n");
    printf("|______\\____/  |_/_/    \\_\\ \n");
    printf("\n");

    printf("Download begin...\n");

    if (0 > lota_download(url, file)) {
        printf("Download failed.\n");
    }
    printf("Download success.\n");

    return 0;
}
