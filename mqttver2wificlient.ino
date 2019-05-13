#include <PubSubClient.h>
#include <ESP8266WiFi.h> //ESP8266WiFi.h
#include <ArduinoJson.h>
#include "string.h"
#define BBT "mqtt.beebotte.com"     

WiFiClient wifiClient;
PubSubClient client(wifiClient);
char* device;
char* num;
char* sta;
char* timer;
char* type;
char sep[] = "-";
int itimer=0; 
unsigned long usltimer;
int statetimer;
unsigned long a;
char device1[10];
char sta1[10]; 
char num1[10];
//////////////////////////////////////////////////////////
//////////////////// SETTUP /////////////////////////////
////////////////////////////////////////////////////////
void setup()
{ 
  Serial.begin(115200);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D1, OUTPUT);
  client.setServer(BBT, 1883);
  WiFi.begin("Phong22", "haonam2018");// ket noi wifi
  delay(10000);
  while (!WiFi.status() == WL_CONNECTED)
  {
    Serial.print(".");
  }
  client.setCallback(onMessage); // moi lan nodemcu nhan duoc goi tin tu cloud tien hanh goi ham onMessage
  while (!Serial) {
    //cho ket noi serial
  }
  delay(1000);
  Serial.println("connecting...");
}
//////////////////////////////////////////////////////////
//////////////////// ON MESSAGE /////////////////////////
////////////////////////////////////////////////////////
void onMessage(char* topic, byte* payload, unsigned int length) {
  Serial.begin(115200);
  // Doc json tu goi tin
  StaticJsonDocument<128> jsonBuffer;
  auto error = deserializeJson(jsonBuffer, payload);
  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return;
  }
  // doc du lieu ve bang kieu String
    String data = jsonBuffer["data"];
  // chuyen ve kieu const char  
    const char* dataconv = data.c_str();
    if(strcmp(topic,"myhome/lightone")==0){
      if(strcmp(dataconv,"0")==0){
        digitalWrite(D5, LOW);
        Serial.println("ligth 1 is on");
        }
        else digitalWrite(D5, HIGH);
      } 
     if(strcmp(topic,"myhome/lighttwo")==0){
      if(strcmp(dataconv,"0")==0){
        digitalWrite(D6, LOW);
        Serial.println("ligth 2 is on");
        }
        else digitalWrite(D6, HIGH);
      }
      if(strcmp(topic,"myhome/fanone")==0){
      if(strcmp(dataconv,"0")==0){
        digitalWrite(D7, LOW);
        Serial.println("fan 1 is on");
        }
        else digitalWrite(D7, HIGH);
      }
      if(strcmp(topic,"myhome/fantwo")==0){
      if(strcmp(dataconv,"0")==0){
        digitalWrite(D1, LOW);
        Serial.println("fan 2 is on");
        }
        else digitalWrite(D1, HIGH);
      }
      if(strcmp(topic,"myhome/timer")==0){
        a = millis(); //thoi gian bat dau goi toi timer
        statetimer = 1; // mode hen gio duoc bat
        Serial.println("call to timer");
        size_t len = strlen (dataconv) + 1;
        char dataconv2[len];
        strcpy (dataconv2, dataconv);
        device = strtok(dataconv2, sep);
        num = strtok( NULL, sep);
        sta = strtok( NULL, sep);
        timer = strtok( NULL, sep);
        type = strtok( NULL, sep);
        itimer = atoi(timer);
        if(strcmp(type,"minutes")==0){
          itimer = itimer*60000;
          }
        else itimer = itimer*1000; 
        usltimer = itimer; //chuyen doi int sang unsigned long
        //chuyen cac bien du lieu tu char* thanh char array
        // kieu char* bi sai kieu du lieu khi chuyen xuong loop() 
        strcpy(device1, device);
        strcpy(sta1, sta);
        strcpy(num1, num);
        Serial.println(device);
        Serial.println(num);
        Serial.println(sta);
        Serial.println(timer);
        Serial.println(type);
        Serial.println(itimer);
        //in cac slot timer ra man hinh  
      } //timer
}

//////////////////////////////////////////////////////////
//////////////////// RECONNECT //////////////////////////
////////////////////////////////////////////////////////

boolean reconnect() {
  Serial.begin(115200);
  if (client.connect("ESP8266","iamtkn_8IJvh8QMi4PlUMTH","")) {
    //dang ki cac resource tren cloud
    client.subscribe("myhome/lightone");
    client.subscribe("myhome/lighttwo");
    client.subscribe("myhome/fanone");
    client.subscribe("myhome/fantwo");
    client.subscribe("myhome/timer");
    Serial.println("Connected to Beebotte MQTT");
  }
  else Serial.println("Connect is failed");
  Serial.println(client.state()); //in ra code error connection neu co 
  return client.connected();
}

//////////////////////////////////////////////////////////
//////////////////// LOOP ///////////////////////////////
////////////////////////////////////////////////////////

void loop()
{  
  delay(1000);
  Serial.begin(115200);
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("wifi is connected"); 
  }
  else Serial.println("wifi connect fail");
  if (!client.connected()) {
    reconnect();
  } else {
    // Client connected
    delay(50);
    client.setCallback(onMessage);
    if(statetimer == 1){
     if((unsigned long) (millis() - a) >= usltimer ){
      //neu thoi gian hien tai tru di thoi gian tu luc bat dau hen gio
      //lon hon hoac bang thoi gian da hen gio
      // thuc hien cac cau lenh o duoi
        delay(1000);
        Serial.println(device1);
        Serial.println(num1);
        Serial.println(sta1);
        Serial.println(itimer);
          if(strcmp(device1,"light")==0)
          {
            if(strcmp(num1,"1")==0)
            {  
              Serial.println("timer for light 1");
               if(strcmp(sta1,"on")==0)
                { 
                  digitalWrite(D5, LOW);
                  Serial.println("light 1 is on");
                  client.publish("myhome/lightone","0");
                } // stat = 0
                 else if(strcmp(sta1,"off")==0)
                {
                  digitalWrite(D5, HIGH);
                  Serial.println("light 1 is off");
                  client.publish("myhome/lightone","1"); //gui du lieu len cloud
                }// stat = 1
             Serial.println("deadline for light 1");       
            }//num = 1
           else if(strcmp(num1,"2")==0)
            {
              Serial.println("timer for light 2");
               if(strcmp(sta1,"on")==0)
                { 
                  digitalWrite(D6, LOW);
                  client.publish("myhome/lightone","0");
                } // stat = 0
                 else if(strcmp(sta1,"off")==0)
                {
                  digitalWrite(D6, HIGH);
                  client.publish("myhome/lighttwo","1"); //gui du lieu len cloud
                }// stat = 1
             Serial.println("deadline for light 2");       
            }//num = 2
          }
          
          //device = light
         else if(strcmp(device1,"fan")==0)
          {
            if(strcmp(num1,"1")==0)
            {  
              Serial.println("timer for fan 1");
               if(strcmp(sta1,"on")==0)
                { 
                  digitalWrite(D7, LOW);
                  Serial.println("fan 1 is on");
                  client.publish("myhome/fanone","0");
                } // stat = 0
                 else if(strcmp(sta1,"off")==0)
                {
                  digitalWrite(D7, HIGH);
                  Serial.println("fan 1 is off");
                  client.publish("myhome/fanone","1"); //gui du lieu len cloud
                }// stat = 1
             Serial.println("deadline for fan 1");       
            }//num = 1
           else if(strcmp(num1,"2")==0)
            {
              Serial.println("timer for fan 2");
               if(strcmp(sta1,"on")==0)
                { 
                  digitalWrite(D1, LOW);
                  client.publish("myhome/fantwo","0");
                } // stat = 0
                 else if(strcmp(sta1,"off")==0)
                {
                  digitalWrite(D1, HIGH);
                  client.publish("myhome/fantwo","1"); //gui du lieu len cloud
                }// stat = 1
             Serial.println("deadline for fan 2");       
            }//num = 2
          }
          
          //device = fan
          
       statetimer = 0; //set lai co = 0 tranh lap lai   
     }
    }
    client.loop();
  }
  delay(1000);
}
