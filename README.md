# esp8266_4Ch_OnOff_STA_Server

![4Ch](https://github.com/esp32peacock/esp8266_4Ch_OnOff_STA_Server/blob/master/4Ch.png?raw=true)

esp8266 many Client to 1 STA Server 4 Channel OnOff

This code about Web Browser that connect to STA webserver
can turn on-off GPIO2 on STA client

Emulate that STA web server is a Web Service
and STA client is your project on-off esp8266

* Client can automatic scan Server ip
* if Client has an OLED you can use that ip to serf STA webserver
* Browser that connect to STA server can on-off GPIO on STA client

Browser ---> STA Server ----> STA Client ----> on-off GPIO


             STA Server <---- STA Client ----> show ip of server on OLED
