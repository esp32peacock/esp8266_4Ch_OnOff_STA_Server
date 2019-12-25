// https://github.com/boblemaire/asyncHTTPrequest

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "asyncHTTPrequest.h"

int break_state = 0;
int anum = 0;
asyncHTTPrequest request[85];
asyncHTTPrequest req;

String Chip_id;
String host_ip;
int LED_state1 = 0;
int LED_state2 = 0;
int LED_state3 = 0;
int LED_state4 = 0;

int getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  String ret = found>index ? data.substring(strIndex[0], strIndex[1]) : "";
  return ret.toInt();
}


void requestCB(void* optParm, asyncHTTPrequest* request, int readyState){
    host_ip = request->responseText();
    Serial.println(host_ip);
    Serial.println("out");
    request->setDebug(false);
		break_state = 1;
}

void reqCB(void* optParm, asyncHTTPrequest* request, int readyState){
    String payload = request->responseText();
    Serial.println(payload);
    request->setDebug(false);
    LED_state1 = getValue(payload,',',0);
    LED_state2 = getValue(payload,',',1);
    LED_state3 = getValue(payload,',',2);
    LED_state4 = getValue(payload,',',3);
    digitalWrite(D4,LED_state1);
    digitalWrite(D5,LED_state2);
    digitalWrite(D6,LED_state3);
    digitalWrite(D7,LED_state4);
}

int scan(int start, int quan) {
  int time;
  int j=0;
  for (int i=start; i<start+quan; i++) {
    request[j].onData(requestCB);
    request[j].setTimeout(2);
    time = millis();
    String url = "http://192.168.43."+String(i)+"/ip";
    request[j].open("GET", url.c_str());
    request[j].send();
    Serial.print(url);
    while (time+500>millis()) {
      Serial.print(".");
      delay(100);
    }
    if (break_state == 1) {
      break;
    }
    Serial.println("/");
    j+=1;
  }
  if (break_state != 1) {
    delay(1000);
  }
  return j;
}

void WiFi_connect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin("Bobzt3rz", "12345678");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println(" ");
}


void setup() {
  Chip_id = String(ESP.getChipId());
  pinMode(D4,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);
  Serial.begin(115200);
  WiFi_connect();
  anum = scan(2,81);
  if (break_state!=1) {
    anum = scan(83,81);
  }
  if (break_state!=1) {
    anum = scan(164,81);
  }
  Serial.println(" ");
  Serial.println("IP_SCAN");
  Serial.println("==============");
  if (break_state==1) {
    Serial.println(host_ip);
    Serial.println(anum);
    req.onData(reqCB);
    req.setTimeout(2);
  } else {
    Serial.println("IP_SCAN Not found");
    while (true) {
      delay(100);
    }
  }
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.mode(WIFI_OFF);
      delay(100);  
      WiFi_connect();
    }
    int time = millis();
    String url = "http://"+host_ip+"/get?id="+Chip_id;
    req.open("GET", url.c_str());
    req.send();
    Serial.println(url);
    while (time+2000>millis()) {
      Serial.print(".");
      delay(100);
    }
    Serial.println(".");
    delay(1000);
}
