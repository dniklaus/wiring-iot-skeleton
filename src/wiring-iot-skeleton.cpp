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
#include <MqttTopic.h>
#include <string.h>

#define MQTT_SERVER "iot.eclipse.org"

SerialCommand*        sCmd = 0;
#ifdef ESP8266
WiFiClient*           wifiClient = 0;
MqttClient*           mqttClient = 0;
//PubSubClient*           mqttClient = 0;
#endif

class TestLedMqttSubscriber : public MqttTopicSubscriber
{
public:
  TestLedMqttSubscriber(const char* topic)
  : MqttTopicSubscriber(topic)
  { }

  bool processMessage()
  {
    bool msgHasBeenHandled = false;
    MqttRxMsg* rxMsg = getRxMsg();
    Serial.print("TestLedMqttSubscriber, ");
    Serial.print("isMyTopic(): ");

    if (isMyTopic())
    {
      Serial.print("true");
      if (0 != rxMsg)
      {
        // take responsibility
        Serial.print(", pin state: ");
        bool pinState = atoi(rxMsg->getRxMsg());
        Serial.println(pinState);
        digitalWrite(BUILTIN_LED, !pinState);  // LED state is inverted on ESP8266
        msgHasBeenHandled = true;
      }
      else
      {
        Serial.println("ERROR: rxMsg unavailable!");
      }
    }
    else
    {
      Serial.println("false");
    }
    return msgHasBeenHandled;
  }

private:
  // forbidden default functions
  TestLedMqttSubscriber();                                              // default constructor
  TestLedMqttSubscriber& operator = (const TestLedMqttSubscriber& src); // assignment operator
  TestLedMqttSubscriber(const TestLedMqttSubscriber& src);              // copy constructor
};

//-----------------------------------------------------------------------------

class TestDiniMqttSubscriber : public MqttTopicSubscriber
{
public:
  TestDiniMqttSubscriber()
  : MqttTopicSubscriber("/test/dini")
  { }

  bool processMessage()
  {
    bool msgHasBeenHandled = false;
    MqttRxMsg* rxMsg = getRxMsg();
    if (isMyTopic())
    {
      if (0 != rxMsg)
      {
        // take responsibility
        Serial.print("TestDiniMqttSubscriber, rxMsg: ");
        Serial.print("isMyTopic(): ");
        if (isMyTopic())
        {
          Serial.print("true");
          if (0 != rxMsg)
          {
            Serial.print(", rx msg: ");
            Serial.println(rxMsg->getRxMsg());
            msgHasBeenHandled = true;
          }
          else
          {
            Serial.println("ERROR: rxMsg unavailable!");
          }
        }
        else
        {
          Serial.println("false");
        }
      }
    }
    return msgHasBeenHandled;
  }

private:
  // forbidden default functions
  TestDiniMqttSubscriber& operator = (const TestDiniMqttSubscriber& src); // assignment operator
  TestDiniMqttSubscriber(const TestDiniMqttSubscriber& src);              // copy constructor
};

//-----------------------------------------------------------------------------
const byte ledPin = 0; // Pin with LED on Adafruit Huzzah

//void callback(char* topic, byte* payload, unsigned int length)
//{
//  Serial.print("Message arrived [");
//  Serial.print(topic);
//  Serial.print("] ");
//  for (int i = 0; i < length; i++)
//  {
//    char receivedChar = (char) payload[i];
//    Serial.print(receivedChar);
//    if (receivedChar == '0')
//    {
//      // ESP8266 Huzzah outputs are "reversed"
//      digitalWrite(BUILTIN_LED, HIGH);
//    }
//    if (receivedChar == '1')
//    {
//      digitalWrite(BUILTIN_LED, LOW);
//    }
//  }
//  Serial.println();
//}
//
//void reconnect()
//{
//  if (0 != mqttClient)
//  {
//    // Loop until we're reconnected
//    while (!mqttClient->connected())
//    {
//      yield();
//      Serial.print("Attempting MQTT connection...");
//      // Attempt to connect
//      if (mqttClient->connect("ESP8266 Client"))
//      {
//        Serial.println("connected");
//        // ... and subscribe to topic
//        mqttClient->subscribe("/test/led");
//      }
//      else
//      {
//        Serial.print("failed, rc=");
//        Serial.print(mqttClient->state());
//        Serial.println(" try again in 5 seconds");
//        // Wait 5 seconds before retrying
//        delay(5000);
//      }
//    }
//  }
//}

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
//  mqttClient = new PubSubClient(*(wifiClient));
//  mqttClient->setServer("iot.eclipse.org", 1883);
//  mqttClient->setCallback(callback);

  mqttClient = new MqttClient(MQTT_SERVER);
  mqttClient->subscribe(new TestLedMqttSubscriber("/test/led"));
  mqttClient->subscribe(new TestDiniMqttSubscriber());
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
//    if (!mqttClient->connected())
//    {
//      reconnect();
//    }
    mqttClient->loop();
  }
  yield();                  // process Timers
}
