#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Comunidad IoT";
const char* pass = "Sistecredito2023*";
const char* mqtt_server = "192.168.0.108";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
const int buttonLed = 0;
int val = 0;
int oldVal = 0;
int str = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi(){
  
  delay(10);
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());

  Serial.println();

  Serial.println("");
  Serial.println("Wifi connected ");
  Serial.println("Ip Address: ");
  Serial.println(WiFi.localIP());
}

void reconnected(){
  while(!client.connected()){
    Serial.print("Attempting connecion...");
    String clientID = "EPS8266Client.";
    clientID += String(random(0xffff), HEX);

    if (client.connect(clientID.c_str()))
    {
      Serial.println("Connected");
      
      if (client.subscribe("LED/1"))
      {
        Serial.printf("El cliente esta suscrito al broker\n", clientID.c_str());
      }
      

    }else{
      Serial.print("failed, rc");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);      
    }
  }
}

void setup() {
  pinMode(buttonLed, INPUT_PULLUP);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {

  ArduinoOTA.handle();

  if (!client.connected())
  {
    reconnected();
  }

  if (client.connected())
  {
    val = digitalRead(buttonLed);

    if (val == HIGH && oldVal == LOW)
    {
      str = 1 - str;
      delay(10);
    }

    oldVal = val;

    if (str == 0)
    {
      client.publish("LED/1", "on");
      delay(300);
    }
    else{
      client.publish("LED/1", "off");
      delay(300);
    }
  }
  client.loop();
}