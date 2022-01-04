#include "Arduino.h"
#include "thermistor.h"
#include "LED.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "config.h"

const char ssid[] = WIFI_SSID;
const char passphrase[] = WIFI_PASS;

const int PIN_THERMISTOR = 34;

const char mqttServerHost[] = MQTT_HOST;
const int mqttServerPort = MQTT_PORT;
const char mqttId[] = MQTT_ID;
const char kind[] = MQTT_KIND;

WiFiClient wificlient;
PubSubClient mqttClient(wificlient);

void setupWifi(char *ssid, char *passphrase)
{
  Serial.println("start connecting wifi");
  WiFi.begin(ssid, passphrase);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }
  Serial.print("\nWifi Connected.\nIP addr: ");
  Serial.print(WiFi.localIP());
  Serial.println();
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void setupMqtt(char *mqttServerHost, int mqttServerPort)
{
  Serial.println("setting up mqtt client");
  mqttClient.setServer(mqttServerHost, mqttServerPort);
  mqttClient.setCallback(callback);
  Serial.println("mqtt done.");
}

void setup()
{
  LED::setup();
  Serial.begin(9600);
  setupWifi((char *)ssid, (char *)passphrase);
  setupMqtt((char *)mqttServerHost, mqttServerPort);
}

void reconnect()
{
  String topic = String(kind) + "/cmd";
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect(mqttId))
    {
      Serial.println("connected");
      mqttClient.subscribe(topic.c_str());
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void publishState(float value)
{
  StaticJsonDocument<200> doc;
  doc["temperature"] = value;
  char buffer[256];
  serializeJson(doc, buffer);
  mqttClient.publish((String(kind) + "/state").c_str(), buffer);
}

void loop()
{
  float temp = Thermistor::readTemperature(PIN_THERMISTOR);
  Serial.printf("temperature: %f C\n", temp);
  delay(2000);
  reconnect();
  mqttClient.loop();
  publishState(temp);
}