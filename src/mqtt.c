/*
 * A publish example for paho.mqtt.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define QOS         1
#define TIMEOUT     10000L  /* unit: ms */

typedef void (*user_cb_t)(char *);
static user_cb_t user_callback = NULL;

volatile MQTTClient_deliveryToken deliveredtoken;

static void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %.*s\n\n", message->payloadlen, (char*)message->payload);
    
    if (user_callback) {
        user_callback((char*)message->payload);
    }
    
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

static void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int mqtt_set_callback(MQTTClient client, void (*user_cb)(char *))
{
    int rc = 0;

    rc = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if (rc != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to set callbacks, return code %d\n", rc);
        rc = EXIT_FAILURE;
    }

    user_callback = user_cb;
    return rc;
}

MQTTClient mqtt_create(const char *address, const char *id)
{
    int rc = 0;
    //声明一个MQTTClient
    MQTTClient client;

    //使用参数创建一个client，并将其赋值给之前声明的client
    rc = MQTTClient_create(&client, address, id, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (rc < 0) {
        return NULL;
    }
    return client;
}

void mqtt_destroy(MQTTClient client)
{
    MQTTClient_destroy(&client);
}

int mqtt_connect(MQTTClient client)
{
    int rc = 0;
    char *username = "rudy";      //添加的用户名
    char *password = "p@ssw0rd";  //添加的密码

    //初始化MQTT Client选项
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = username; //将用户名写入连接选项中
    conn_opts.password = password;//将密码写入连接选项中

    //使用MQTTClient_connect将client连接到服务器，使用指定的连接选项。成功则返回MQTTCLIENT_SUCCESS
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        return EXIT_FAILURE;
    }
    return 0;
}

int mqtt_disconnect(MQTTClient client)
{
    return MQTTClient_disconnect(client, 10000);
}

int mqtt_subscribe(MQTTClient client, const char *topic)
{
    int rc = 0;
    
    //#define MQTTClient_message_initializer { {'M', 'Q', 'T', 'M'}, 0, 0, NULL, 0, 0, 0, 0 }
    //MQTTClient_message pubmsg = MQTTClient_message_initializer;
    //声明消息token
    //MQTTClient_deliveryToken token;

    rc = MQTTClient_subscribe(client, topic, QOS);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to subscribe, return code %d\n", rc);
        rc = EXIT_FAILURE;
    }

    return rc;
}

int mqtt_unsubscribe(MQTTClient client, const char *topic)
{
    int rc = 0;

    rc = MQTTClient_unsubscribe(client, topic);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to unsubscribe, return code %d\n", rc);
        rc = EXIT_FAILURE;
    }

    return rc;
}

int mqtt_publish(MQTTClient client, const char *topic, const char *payload)
{
    int rc = 0;

    //#define MQTTClient_message_initializer { {'M', 'Q', 'T', 'M'}, 0, 0, NULL, 0, 0, 0, 0 }
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    //声明消息token
    MQTTClient_deliveryToken token;

    pubmsg.payload = (void *)payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    printf("Waiting for publication of '%s' on topic '%s'\n", payload, topic);

    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);

    return rc;
}
