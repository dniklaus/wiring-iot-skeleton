# wiring-iot-skeleton
Wiring / Arduino based IoT Skeleton Embedded Application, powered by [PlatformIO](http://platformio.org "Cross-platform build system")

The project is built for [Adafruit ESP8266 Huzzah](https://www.adafruit.com/product/2471 "Adafruit HUZZAH ESP8266 Breakout") and can easily be adapted to make it also run on other [ESP8266](https://en.wikipedia.org/wiki/ESP8266) based controllers, such as for [NodeMCU](http://nodemcu.com/index_en.html "NodeMCU Connect Things EASY") for instance.

## Purpose
This project builds up an Arduino Framework based IoT application skeleton and it comprises of several components helping with debugging and integrating embedded applications on ESP8266 based contoller modules.

This project integrates the following **components**:

* ***Timer***: configurable recurring or non-recurring timer to schedule events without having to use Arduino delay() function; helps to improve your application's architecture by encapsulating the timers into your components and thus make them active
* ***Ramutils***: helps to determine the free Ram that is currently available
* ***DbgTrace***: debug trace log environment with configurable log levels
* ***DbgCLI***: interactive console environment with command tree that can be built up decentralized (from any different location in your application code and within any component)
* ***App-Dbg***: boilerplate code setting up all the debug environment such as CLI and Tracing and free RAM info printer
* ***MqttClient***: Mqtt Client wrapping araound ***PubSubClient*** library, monitoring the LAN and Mqtt conection, able to automatically re-connect on connection loss, providing auto publish for selectable topics and auto subscribe for all registered topic subscriptions on re-connection, supports multiple subscritions also with wildcards in the topic path
* ***ThingSpeak***: Send data to channels on Mathwork's [thingspeak.com](https://thingspeak.com) open data platform for the Internet of Things with MATLAB analytics and visualization
* ***ArduinoJson***: 

The **command line interface** provides the following **functionality**:  

* configure the WiFi access point (SSID & Password) and show the WiFi connection status and the available hotspots around your device
* [ThingSpeak](http://thingspeak.com) features:
  * setup a  channel to be written to later on (ChID & API Key)
  * set ThingSpeak channel data fields (1..8) and write the data to the channel
* MQTT features:
  * connect to / disconnect from a broker (currently using [io.eclipse.org](https://iot.eclipse.org/))
  * subscribe / unsubscribe to / from MQTT Topics
  * publish to MQTT Topics

This skeleton application demonstrates how to integrate libraries provided by the PlatformIO ecosystem and also how to use your own libraries and can help you to build up your own Arduino Framework based applications with focus on IoT. 

The following components are in focus:

* [Timer](https://github.com/dniklaus/arduino-utils-timer)
* [Debug-Cli](https://github.com/ERNICommunity/debug-cli)
* [Dbg-Trace](https://github.com/ERNICommunity/dbg-trace)
* [Mqtt-Client](https://github.com/ERNICommunity/mqtt-client)
* [ThingSpeak](https://github.com/mathworks/thingspeak-particle)
* [PubSubClient](https://github.com/knolleary)

## Toolchain
[PlatformIO](http://platformio.org "Cross-platform build system") is the ideal foundation when developing IoT devices. It supports cross-platform development for many different controller boards widely used in the maker field and also for industrial applications.

### Installation
#### Python 2.7

**Windows**

* download Python 2.7.x from: https://www.python.org/downloads/
* install for all users
* select destinationdirectory (keep default): C:\Python27\
* add python.exe to path

**Linux**

Install Python 2.7 using your package manager.

#### PlatformIO
Install PlatformIO using the Python Package Manager
(see also http://docs.platformio.org/en/latest/installation.html#python-package-manager):

* in a cmd shell enter: `pip install -U platformio`
* upgrade pip, in the cmd shell enter: `python -m pip install --upgrade pip`
* upgrade PlatformIO, in the cmd shell enter: `pio upgrade`
 

## How to build for Eclipse CDT
  1. Create a directory where your Eclipse Workspace will be stored and where this project shall be cloned into. E.g. `C:\git\pio-prj`
  2. Clone this repository recursively into the folder you created before, `git clone --recursive git@github.com:dniklaus/wiring-iot-skeleton.git`
  3. Open a command shell in the just cloned project folder, i.e in `C:\git\pio-prj\wiring-iot-skeleton`
  4. Run the command `pio init -b huzzah --ide eclipse`
  5. Run the command `pio run`

Note: Print a list of all available boards with the command: `pio boards`

## Open project in Eclipse CDT
  1. Open Eclipse CDT, choose the folder you created before as workspace, i.e `C:\git\pio-prj`
  2. Import the project with File->Import->General->Existing Projects into Workspace, choose the `wiring-iot-skeleton` (i.e `C:\git\pio-prj\wiring-iot-skeleton`)

## Connect Terminal
In order to test and run the CLI commands, a terminal application shall be used. One of the following two applications might give you the best experience:

* [HTerm](http://www.der-hammer.info/terminal/)
* [CoolTerm](http://freeware.the-meiers.org/)

When using HTerm, load the _hterm-com18.cfg_ file to configure the application properly. Alter the COM18 accordingly to the one that has been selected on your computer.

CoolTerm users can load the _CoolTerm_0.stc_ file and alter the COM8 to the appropriate one.

## Debug Features
### Debug CLI Commands
#### Command Tree
     dbg                      Debug CLI root node
       tr                     Debug Trace Port config access
         heap                 Particular Trace Port (heap: see below in chapter Trace Port)
           out                Trace Output config access for the current trace port
             get              Show the assigned Trace Output
             set <outName>    Set a particular Trace Output name
             list             Show all available Trace Output names (and the currently selected)
           lvl                Trace Level config access for the current trace port
             get              Show the current trace level
             set <level>      Set a particular trace level
             list             Show all available trace levels (and the currenntly selected)
         conmon               Trace Port: LAN and MQTT connection monitor                
         mqttctrl             Trace Port: MQTT client control
         mqttrx               Trace Port: Received Messages from MQTT subscriptions
         mqttdflt             Trace Port: Default Mqtt Subscriber
         mqttled              Trace Port: Test LED Mqtt Subscriber
       wifi                   WiFi debug commands
         mac                  show WiFi MAC address
         nets                 list available WLAN networks
         stat                 Show WiFi status
         dis                  Disconnect WiFi
         con <SSID> <Pass>    Connect WiFi
       mqtt                   MQTT client control commands
         con                  Connect to broker
         dis                  Disconnect from broker
         sub <Topic>          Subscribe to MQTT Topic
         unsub <Topic>        Unsubscribe to MQTT Topic
         pub <Topic> <Value>  Publish Value to MQTT Topic
       thgspk                 ThingSpeak debug commands
         chid <channelId>     Set ThingSpeak Channel ID.
         key <APIKey>         Set ThingSpeak API key.
         set <value> [field]  Set ThingSpeak field value (field: 1..8, default: 1)
         wr                   ThingSpeak write fields.

#### Example commands
* `dbg tr heap lvl set debug`
* `dbg thgspk set 23.6 1`
* `dbg thgspk set 25.3 2`
* `dbg thgspk wr`


### Trace Port

|Trace Port|default level|functionality|
|----------|-------------|:------------------------------------------------------------------------------------|
|heap      |info         |if set to debug level: automatically print free heap memory [bytes], every 10 seconds|
|conmon    |info         |if set to debug level: show WLAN connect / disconnect status updates                 |
|mqttctrl  |info         |if set to debug: show MQTT Client connection status                                  |
|mqttrx    |info         |if set to degug level: show subscribed incoming messages                             |

## Library Usage
This chapter lists all the libraries used by this project.

### PlatformIO Libraries

|ID |Name         |URL                                             |Description                                                                         |
|:--|:------------|:-----------------------------------------------|:-----------------------------------------------------------------------------------|
|173|SerialCommand|https://github.com/kroimon/Arduino-SerialCommand|A Wiring/Arduino library to tokenize and parse commands received over a serial port.|
|550|ThingSpeak   |https://github.com/mathworks/thingspeak-particle|"MathWorks": ThingSpeak Communication Library for Arduino & ESP8266                 |
| 89|PubSubClient |https://github.com/knolleary                    |A client library for MQTT messaging.                                                |
| 64|ArduinoJson  |https://github.com/bblanchon/ArduinoJson        |An elegant and efficient JSON library for embedded systems                          |



### Homebrew Libraries

|Name       |URL                                             |Description                                                                                                                      |
|:----------|:-----------------------------------------------|:--------------------------------------------------------------------------------------------------------------------------------|
|Timer      |https://github.com/dniklaus/arduino-utils-timer |Universal recurring or non-recurring Timer                                                                                       |
|Debug-Cli  |https://github.com/ERNICommunity/debug-cli      |Debug CLI for Embedded Applications - Command Line  Interface for debugging and testing based on object oriented tree structure. |
|Dbg-Trace  |https://github.com/ERNICommunity/dbg-trace      |Debug Trace component for Embedded Applications - Debug and Trace Log message system based on trace ports with adjustable levels.|
|Mqtt-Client|https://github.com/ERNICommunity/mqtt-client    |MQTT Client with pluggable publish and subscribe topic objects                                                                   |
|RamUtils   |https://github.com/dniklaus/arduino-utils-mem   |Arduino Memory Utilities                                                                                                         |
|App-Debug  |https://github.com/dniklaus/wiring-app-debug.git|Wiring application debug setup component                                                                                         |

