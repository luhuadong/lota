#include <stdio.h>
#include <unistd.h>
#include "lota.h"

#define TOPIC           "test"
#define PAYLOAD         "Hello, Bocon OTA!"
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)

static const char *url = "https://static.getiot.tech/flag-of-china.png";
static const char *file = "test.png";

static void user_cb(char *msg)
{
    printf("## ^_^ ## %s\n", msg);
    if (isaurl(msg)) {
        printf("Is a url\n");
    }
    else {
        printf("Is not a url\n");
    }
}

int main(int argc, char *agrv[])
{
    int ret = 0;

    printf(" _      ____ _______         \n");
    printf("| |    / __ \\__   __|/\\    \n");
    printf("| |   | |  | | | |  /  \\    \n");
    printf("| |   | |  | | | | / /\\ \\  \n");
    printf("| |___| |__| | | |/ ____ \\  \n");
    printf("|______\\____/  |_/_/    \\_\\ \n");
    printf("\n");

    /* Init MQTT */
    MQTTClient client = mqtt_create();
    if (!client) {
        return -1;
    }

    mqtt_set_callback(client, user_cb);
    mqtt_connect(client);
    mqtt_subscribe(client, TOPIC);
    mqtt_publish(client, TOPIC, PAYLOAD);
    mqtt_publish(client, TOPIC, url);

    /* Downloading */
    printf("Download begin...\n");

    ret = lota_download(url, file);
    if (ret < 0) {
        printf("Download failed.\n");
    }
    printf("Download success, save as %s\n", file);

    /* Checking */
    char md5_str[MD5_STR_LEN + 1];

    ret = check_md5_file(file, md5_str);
    if (ret < 0) {
        printf("Check failed.\n");
    }
    printf("MD5sum: %s  %s\n\n", md5_str, file);

    /* Release */
    mqtt_disconnect(client);
    mqtt_destroy(client);

    return 0;
}
