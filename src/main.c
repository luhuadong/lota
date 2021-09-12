#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "lota.h"
#include "cJSON.h"

#define ADDRESS         "tcp://localhost:1883" /* 更改此处地址 */
#define CLIENTID        "ota"                  /* 更改此处客户端ID */
#define OTA_TOPIC       "/ota/device/upgrade"
#define PAYLOAD         "Ready to OTA"
#define URL_SIZE        2048
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)

static const char *test_url = "https://static.getiot.tech/flag-of-china.png";
static const char *savefile = "test.png";

static pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
static sem_t sem;

typedef struct {
    char url[URL_SIZE];
    char md5sum[MD5_STR_LEN + 1];
} ota_info_t;

static ota_info_t ota_info;

static void user_cb(char *msg)
{
    if (isaurl(msg)) {

        printf("Is a url\n");
        pthread_rwlock_wrlock(&lock);

        memset(ota_info.url, 0, sizeof(ota_info.url));
        strncpy(ota_info.url, msg, strlen(msg));

        pthread_rwlock_unlock(&lock);
        sem_post(&sem);
    }
    else {
        printf("Is not a url\n");
    }
}

static int run_ota(ota_info_t *ota)
{
    int ret = 0;

    /* Downloading */
    printf("Download begin...\n");
    char *file = get_name_from_url(ota->url, strlen(ota->url)+1);
    printf("Target: %s\n", file);

    ret = lota_download(ota->url, file);
    if (ret < 0) {
        printf("Download failed.\n");
        return -1;
    }
    printf("Download success, save as %s.\n", file);

    /* Checking */
    char md5_str[MD5_STR_LEN + 1];

    ret = check_md5_file(file, md5_str);
    if (ret < 0) {
        printf("Calculate MD5 failed.\n");
        return -1;
    }
    printf("MD5sum: %s  %s\n\n", md5_str, file);

#if 0
    if (0 != strncmp(ota->md5sum, md5_str, MD5_STR_LEN)) {
        printf("Invalid file, MD5 sum did not match.\n");
        return -1;
    }
#endif

    /* Upgrade */
    char cmd[256] = {0};
    snprintf(cmd, sizeof(cmd), "cp %s /tmp/", file);
    system(cmd);

    return 0;
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

    printf("cJSON version: %s\n", cJSON_Version());

    /* Init semaphore */
    ret = sem_init(&sem, 0, 0);
	if (ret == -1) {
		perror("semaphore intitialization failed\n");
		return -1;
	}

    /* Init MQTT */
    MQTTClient client = mqtt_create(ADDRESS, CLIENTID);
    if (!client) {
        return -1;
    }

    mqtt_set_callback(client, user_cb);
    mqtt_connect(client);
    mqtt_subscribe(client, OTA_TOPIC);
    mqtt_publish(client, OTA_TOPIC, PAYLOAD);
    mqtt_publish(client, OTA_TOPIC, test_url);

    while (1) {
        ota_info_t lota;

        /* wait */
        sem_wait(&sem);

        pthread_rwlock_rdlock(&lock);
        memcpy(&lota, &ota_info, sizeof(ota_info_t));
        pthread_rwlock_unlock(&lock);

        printf("URL: %s\n", lota.url);
        ret = run_ota(&lota);
        if (ret == 0) {
            printf("Upgrade successfully\n");
        }
        else {
            printf("Upgrade failed\n");
        }
    }

    /* Release */
    mqtt_disconnect(client);
    mqtt_destroy(client);
    sem_destroy(&sem);

    return 0;
}
