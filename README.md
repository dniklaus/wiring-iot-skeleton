# wiring-iot-skeleton
Wiring / Arduino based IoT Skeleton Embedded Application, powered by [PlatformIO](http://platformio.org "Cross-platform build system")

## How to build (for Eclipse)
  1. Create a directory where your Eclipse Workspace will be stored and where this project shall be cloned into. E.g. `C:\git\pio-prj`
  2. Clone this repository recursively into the folder you created before, `git clone --recursive git@github.com:dniklaus/wiring-iot-skeleton.git`
  3. Open a command shell in the just cloned project folder, i.e in `C:\git\pio-prj\wiring-iot-skeleton`
  4. Run the command `pio init -b huzzah --ide eclipse`
  5. Run the command `pio run`

## Open project in Eclipse CDT
  6. Open Eclipse CDT, choose the folder you created before as workspace, i.e `C:\git\pio-prj`
  7. Import the project with File->Import->General->Existing Projects into Workspace, choose the `wiring-iot-skeleton` (i.e `C:\git\pio-prj\wiring-iot-skeleton`)