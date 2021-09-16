# LOTA

Linux Over the Air update tool



## Requires

- MQTT
- CJSON
- CURL
- OPENSSL



## Dependencies

### Installing curl

```shell
sudo apt update
sudo apt install curl libcurl4-openssl-dev
```

### Installing paho-mqtt

```shell
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
mkdir build
cd build
cmake ..
make
sudo make install
```

Optional

```shell
export LD_LIBRARY_PATH=/usr/local/lib
```

### Install MQTT Broker

Example: mosquitto

```shell
sudo apt install mosquitto
```



## Diagram

```sequence
UI->MQTT Broker: 下发远程升级命令
MQTT Broker->LOTA: 发布 update 主题
LOTA->HTTP Server: 请求数据（url）
HTTP Server->LOTA: 响应数据（package）
LOTA->LOTA: 校验、升级
LOTA->MQTT Broker: ACK（进度、结果）
MQTT Broker->UI: 升级成功/失败
```

> Figure1. OTA sequence diagram

![](https://luhuadong.gitee.io/k37x-wiki/images/lota-timing-diagram.png)


## Build & Test

### Compiling (Make)

```shell
make            # lota
make testcase   # ota_publish
```

### Compiling (CMake)

```shell
mkdir build
cd build
cmake ..
make
```

### Ensure MQTT Broker is running

```shell
$ systemctl status mosquitto
● mosquitto.service - Mosquitto MQTT v3.1/v3.1.1 Broker
     Loaded: loaded (/lib/systemd/system/mosquitto.service; enabled; vendor preset: enabled)
     Active: active (running) since Sun 2021-09-12 11:09:58 CST; 47min ago
       Docs: man:mosquitto.conf(5)
             man:mosquitto(8)
   Main PID: 1762458 (mosquitto)
      Tasks: 3 (limit: 9319)
     Memory: 1.0M
     CGroup: /system.slice/mosquitto.service
             └─1762458 /usr/sbin/mosquitto -c /etc/mosquitto/mosquitto.conf
```

### Run lota

```shell
$ ./lota 
 _      ____ _______         
| |    / __ \__   __|/\    
| |   | |  | | | |  /  \    
| |   | |  | | | | / /\ \  
| |___| |__| | | |/ ____ \  
|______\____/  |_/_/    \_\ 

```

### Publish OTA message

```shell
$ ./ota_publish 
Waiting for up to 10 seconds for publication of https://static.getiot.tech/HelloRepo-v1.0.0.zip
on topic '/ota/device/upgrade' for client with ClientID: ota_publish
Message with delivery token 1 delivered
```

Now, you can see some files has been downloaded in local directory.



## Data format

OTA string example:

- Remote HTTP server

  ```json
  {
  	"url":	"https://static.getiot.tech/flag-of-china.png",
  	"md5":	"de0e54231f75ecc5ec1bad7143a420e4"
  }
  ```

- Local file system

  ```json
  {
  	"url":	"file:///run/media/sda1/flag-of-china.png",
  	"md5":	"de0e54231f75ecc5ec1bad7143a420e4"
  }
  ```

ACK status:

```json
{
	"percent":	100
}
```

