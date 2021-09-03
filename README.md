# LOTA

Linux Over the Air update tool

## Requires

- MQTT
- CJSON
- CURL
- SSL

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



## References

- [LINKSDK在linux系统中使用OTA升级](https://www.iotot.com/?p=353)

