/*
 * wiring-iot-skeleton.cpp
 *
 *  Created on: 19.05.2016
 *      Author: niklausd
 */

#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#endif
// PlatformIO libraries
#include <PubSubClient.h>   // pio lib install 89,  lib details see https://github.com/knolleary/PubSubClient
#include <SerialCommand.h>  // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand
#include <ThingSpeak.h>     // pio lib install 550, lib details see https://github.com/mathworks/thingspeak-arduino

// private libraries
#include <Timer.h>
#include <DbgCliNode.h>
#include <DbgCliTopic.h>
#include <DbgTracePort.h>
#include <DbgTraceContext.h>
#include <DbgTraceOut.h>
#include <DbgPrintConsole.h>
#include <DbgTraceLevel.h>
#include <ProductDebug.h>
#include <MqttClient.h>
#include <MqttClientController.h>
#include <PubSubClientWrapper.h>
#include <MqttMsgHandler.h>
#include <string.h>

#define MQTT_SERVER "iot.eclipse.org"

SerialCommand*        sCmd = 0;
#ifdef ESP8266
WiFiClient*           wifiClient = 0;
//MqttClient*           mqttClient = 0;
PubSubClient*           mqttClient = 0;
#endif

class TestLedMqttMsgHandler : public MqttMsgHandler
{
public:
  TestLedMqttMsgHandler(const char* topic)
  : MqttMsgHandler(topic)
  { }

  bool processMessage()
  {
    bool msgHasBeenHandled = false;

    if (isMyTopic())
    {
      // take responsibility
      Serial.print("LED test handler, topic: ");
      Serial.print(getTopic());
      Serial.print(", msg: ");
      Serial.println(getRxMsg());

      bool pinState = atoi(getRxMsg());
      digitalWrite(BUILTIN_LED, !pinState);

      msgHasBeenHandled = true;
    }
    return msgHasBeenHandled;
  }

private:
  // forbidden default functions
  TestLedMqttMsgHandler();                                              // default constructor
  TestLedMqttMsgHandler& operator = (const TestLedMqttMsgHandler& src); // assignment operator
  TestLedMqttMsgHandler(const TestLedMqttMsgHandler& src);              // copy constructor
};

const byte ledPin = 0; // Pin with LED on Adafruit Huzzah

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    char receivedChar = (char) payload[i];
    Serial.print(receivedChar);
    if (receivedChar == '0')
    {
      // ESP8266 Huzzah outputs are "reversed"
      digitalWrite(BUILTIN_LED, HIGH);
    }
    if (receivedChar == '1')
    {
      digitalWrite(BUILTIN_LED, LOW);
    }
  }
  Serial.println();
}


void reconnect()
{
  if (0 != mqttClient)
  {
    // Loop until we're reconnected
    while (!mqttClient->connected())
    {
      yield();
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (mqttClient->connect("ESP8266 Client"))
      {
        Serial.println("connected");
        // ... and subscribe to topic
        mqttClient->subscribe("/test/led");
      }
      else
      {
        Serial.print("failed, rc=");
        Serial.print(mqttClient->state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  }
}

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 1);

  setupDebugEnv();
#ifdef ESP8266
  //-----------------------------------------------------------------------------
  // ESP8266 WiFi Client
  //-----------------------------------------------------------------------------
  wifiClient = new WiFiClient();

  //-----------------------------------------------------------------------------
  // ThingSpeak Client
  //-----------------------------------------------------------------------------
  ThingSpeak.begin(*(wifiClient));

  //-----------------------------------------------------------------------------
  // MQTT Client
  //-----------------------------------------------------------------------------
  mqttClient = new PubSubClient(*(wifiClient));
  mqttClient->setServer("iot.eclipse.org", 1883);
  mqttClient->setCallback(callback);

//  mqttClient = new MqttClient(MQTT_SERVER);
//  mqttClient->subscribe(new TestLedMqttMsgHandler("/test/led"));
#endif
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  if (0 != mqttClient)
  {
    if (!mqttClient->connected())
    {
      reconnect();
    }
    mqttClient->loop();
  }
  yield();                  // process Timers
}
