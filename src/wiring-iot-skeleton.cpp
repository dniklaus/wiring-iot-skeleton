/*
 * wiring-iot-skeleton.cpp
 *
 *  Created on: 19.05.2016
 *      Author: niklausd
 */

#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
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
#include <MqttClientController.h>
#include <PubSubClientWrapper.h>

#define MQTT_SERVER  "iot.eclipse.org"

SerialCommand*        sCmd = 0;
#ifdef ESP8266
WiFiClient*           wifiClient = 0;
#endif

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);

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
  MqttClientController::Instance()->assignMqttClientWrapper(new PubSubClientWrapper(*(wifiClient), MQTT_SERVER), new PubSubClientCallbackAdapter());
  MqttClientController::Instance()->setShallConnect(true);
#endif
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  MqttClientController::Instance()->loop();
  yield();                  // process Timers
}
