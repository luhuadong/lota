#ifndef __LOTA_H
#define __LOTA_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "MQTTClient.h"

bool isaurl(const char *url);

int lota_download(const char *url, const char *save_path);

int check_md5_string(unsigned char *dest_str, unsigned int dest_len, char *md5_str);
int check_md5_file(const char *file_path, char *md5_str);

MQTTClient mqtt_create(const char *address, const char *id);
int mqtt_destroy(MQTTClient client);
int mqtt_connect(MQTTClient client);
int mqtt_disconnect(MQTTClient client);
int mqtt_publish(MQTTClient client, const char *topic, const char *payload);
int mqtt_subscribe(MQTTClient client, const char *topic);
int mqtt_unsubscribe(MQTTClient client, const char *topic);
int mqtt_set_callback(MQTTClient client, void (*user_cb)(char *));

#endif /* __LOTA_H */