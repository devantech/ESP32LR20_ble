# ESP32LR20_ble
A firmware for the ESP32LR20 that enables control over ble. This firmware defines several services and characteristics that are used to get information from and control the module.

# Services

This module has 3 services defined as detailed below.

## Module info service
UUID - "3d666724-e775-11ea-adc1-0242ac120002"  
This service provides information about the module itself. It has the following characteristics:  

**MODULE_ID** ( read only )  
UUID - "af1fdb15-5f75-4559-aa02-f575cf21da39"  
Contains the ID for this module (39)  
  
**FIRMWARE_VERSION** ( read only )  
UUID - "91857612-9071-45c3-88ad-433105d5f8fa"  
Contains the firmware version of the module.

## Relay info service
UUID - "eda0f9e6-e78f-11ea-adc1-0242ac120002"  
This service provides information about the relays on the module. It has the following characteristics:  

**RELAY_COUNT** ( read only )  
UUID - "a96f34fe-19f7-401f-873a-30462c535402"  
Contains the number of relays that this module has (2).  
  
**RELAY_STATES** ( read write )    
UUID - "d1482882-1fd0-4f72-96bd-b6c953345653"  
Reading this characteristic will return a binary string indicating the state of the relays, a '1' means the relay is active and a '0' means it is inactive. If the module returns "01" this would mean relay 1 is active and relay 2 is inactive.  
Writing a binary string to this characteristic will set the relay states accordingly. Writing the string "11" would set relays 1 and 2 active.

# Install this firmware

Make sure that you have installed the Arduino IDE ( https://www.arduino.cc/ ) on your computer and connected your module to your computer via a USB cable.

### Install the ESP32 arduino core.

1. Start Arduino IDE and open the preferences window.
2. In Additional Board Manager URLs field add https://dl.espressif.com/dl/package_esp32_index.json
3. Open Boards Manager from **Tools > Board** menu and install esp32 platform.


### Install the ESP32ble library

1. Open the Library Manager from the **Sketch > Include Library** menu.
2. Search for the "ESP32 BLE Arduino" library by Neil Kolban and click install.

### Upload

1. Open the ESP32SR88_ble.ino file in the Arduino IDE.
2. From the **Tools > Board** menu select **ESP32 Dev Module**
3. Select the serial port for your module.
4. Click the upload button.
