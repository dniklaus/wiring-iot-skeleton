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
#include <ArduinoJson.h>    // pio lib install 64,  lib details see https://github.com/bblanchon/ArduinoJson


// private libraries
#include <Timer.h>
#include <DbgCliNode.h>
#include <DbgCliTopic.h>
#include <DbgTracePort.h>
#include <DbgTraceContext.h>
#include <DbgTraceOut.h>
#include <DbgPrintConsole.h>
#include <DbgTraceLevel.h>
#include <MqttClient.h>
#include <MqttClientController.h>
#include <PubSubClientWrapper.h>
#include <MqttTopic.h>
#include <string.h>
#include <AppDebug.h>
#include <ProductDebug.h>

#define MQTT_SERVER "iot.eclipse.org"

SerialCommand*        sCmd = 0;

#ifdef ESP8266
WiFiClient*           wifiClient = 0;
MqttClient*           mqttClient = 0;
#endif

class TestLedMqttSubscriber : public MqttTopicSubscriber
{
private:
  DbgTrace_Port* m_trPort;
public:
  TestLedMqttSubscriber()
  : MqttTopicSubscriber("test/led")
  , m_trPort(new DbgTrace_Port("mqttled", DbgTrace_Level::debug))
  { }

  ~TestLedMqttSubscriber()
  {
    delete m_trPort;
    m_trPort = 0;
  }

  bool processMessage()
  {
    bool msgHasBeenHandled = false;
    MqttRxMsg* rxMsg = getRxMsg();

    if (isMyTopic())
    {
      if (0 != rxMsg)
      {
        // take responsibility
        bool pinState = atoi(rxMsg->getRxMsgString());
        TR_PRINTF(m_trPort, DbgTrace_Level::debug, "LED state: %s", (pinState > 0) ? "on" : "off");
        digitalWrite(BUILTIN_LED, !pinState);  // LED state is inverted on ESP8266
        msgHasBeenHandled = true;
      }
      else
      {
        TR_PRINTF(m_trPort, DbgTrace_Level::error, "rxMsg unavailable!");
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
  TestLedMqttSubscriber& operator = (const TestLedMqttSubscriber& src); // assignment operator
  TestLedMqttSubscriber(const TestLedMqttSubscriber& src);              // copy constructor
};

//-----------------------------------------------------------------------------

const byte ledPin = 0; // Pin with LED on Adafruit Huzzah

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 1);

  setupProdDebugEnv();

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
  mqttClient->subscribe(new TestLedMqttSubscriber());
  mqttClient->subscribe(new DefaultMqttSubscriber("test/startup"));
  mqttClient->installAutoPublisher(new MqttTopicPublisher("test/startup", WiFi.macAddress().c_str(), true));
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
    mqttClient->loop();
  }
  yield();                  // process Timers
}
