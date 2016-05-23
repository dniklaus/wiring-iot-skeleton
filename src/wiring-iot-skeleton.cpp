/*
 * wiring-iot-skeleton.cpp
 *
 *  Created on: 19.05.2016
 *      Author: niklausd
 */

#include <Arduino.h>
#include <DbgCliTopic.h >
#include <DbgTracePort.h>

DbgTrace_Port* trPort = 0;

void setup()
{
  trPort = new DbgTrace_Port("loop", DbgTrace_Level::info);

}

void loop()
{

}
