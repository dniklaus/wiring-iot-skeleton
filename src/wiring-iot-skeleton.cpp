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
MqttClient*           mqttClient = 0;
#endif

class TestLedMqttMsgHandler : public MqttMsgHandler
{
public:
  TestLedMqttMsgHandler(const char* topic)
  : MqttMsgHandler(topic)
  { }

  void handleMessage(const char* topic, unsigned char* payload, unsigned int length)
  {
    if (isMyTopic(topic))
    {
      // take responsibility
      char msg[length+1];
      memcpy(msg, payload, length);
      msg[length] = 0;

      Serial.print("LED test handler, topic: ");
      Serial.print(getTopic());
      Serial.print(", msg: ");
      Serial.println(msg);

      bool pinState = atoi(msg);
      digitalWrite(BUILTIN_LED, !pinState);
    }
    else if (0 != next())
    {
      // delegate
      Serial.println("LED test handler has to delegate the job.");
      next()->handleMessage(topic, payload, length);
    }
    else
    {
      Serial.println("LED test handler is the last in the chain");
    }
  }

private:
  // forbidden default functions
  TestLedMqttMsgHandler();                                              // default constructor
  TestLedMqttMsgHandler& operator = (const TestLedMqttMsgHandler& src); // assignment operator
  TestLedMqttMsgHandler(const TestLedMqttMsgHandler& src);              // copy constructor
};

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
  mqttClient = new MqttClient(MQTT_SERVER);
  mqttClient->subscribe(new TestLedMqttMsgHandler("/test/led"));
#endif
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  mqttClient->loop();
  yield();                  // process Timers
}
