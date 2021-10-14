#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "MQTTClient.h"
#include "cJSON.h"
#include "lota.h"

#define ADDRESS         "tcp://localhost:1883" /* 更改此处地址 */
#define CLIENTID        "ota_publish"          /* 更改此处客户端ID */
#define PAYLOAD         "TEST"
#define PAYLOAD1        "{\"url\": \"https://static.getiot.tech/flag-of-china.png\", \"md5\": \"de0e54231f75ecc5ec1bad7143a420e4\"}"
//#define PAYLOAD1        "{\"url\": \"file:///opt/flag-of-china.png\", \"md5\": \"de0e54231f75ecc5ec1bad7143a420e4\"}"
#define PAYLOAD2        "{\"url\": \"https://static.getiot.tech/HelloRepo-v1.0.0.zip\", \"md5\": \"6b386e3c126c81c14a37f1c404302168\"}"
#define QOS         1
#define TIMEOUT     10000L

//声明一个MQTTClient
static MQTTClient client;
static sem_t sem;

static void user_cb(char *topic, char *msg)
{
    if (0 == strncmp(topic, ACK_TOPIC, strlen(ACK_TOPIC))) {

        cJSON *root = cJSON_Parse(msg);
        printf("%s\n", cJSON_Print(root));
        cJSON *percent_obj = cJSON_GetObjectItem(root, "progress");
        int percent = cJSON_GetNumberValue(percent_obj);

        if (percent == 100) {
            printf("Completed!\n");
            sem_post(&sem);
        }

        cJSON_Delete(root);
    }
}

int main(int argc, char* argv[])
{
    int rc;

    /* Init semaphore */
    rc = sem_init(&sem, 0, 0);
    if (rc == -1) {
        perror("semaphore intitialization failed\n");
        return -1;
    }

    /* Init MQTT */
    client = mqtt_create(ADDRESS, CLIENTID);
    if (!client) {
        return -1;
    }

    rc = mqtt_set_callback(client, user_cb);
    rc = mqtt_connect(client);
    rc = mqtt_subscribe(client, ACK_TOPIC);

    printf("## publish 1\n");
    rc = mqtt_publish(client, OTA_TOPIC, PAYLOAD1);
    sleep(2);

    printf("## publish 2\n");
    rc = mqtt_publish(client, OTA_TOPIC, PAYLOAD2);
    sleep(2);

    printf("## check status\n");
    rc = mqtt_publish(client, CHK_TOPIC, PAYLOAD);

    printf("## wait\n");
    sem_wait(&sem);

    /* Release */
    rc = mqtt_disconnect(client);
    mqtt_destroy(client);

    return rc;
}
