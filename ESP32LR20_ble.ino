/**
 * A firmwarefor the ESP32LR20 that enables ble communication.
 * 
 * James Henderson, 2020.
 */
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "relays.h"

#define SERVICE_MODULE_UUID       "3d666724-e775-11ea-adc1-0242ac120002"  // Service that defines module information
#define CHARACTERISTIC_ID         "af1fdb15-5f75-4559-aa02-f575cf21da39"  // Characteristic that contains the module ID
#define CHARACTERISTIC_VER        "91857612-9071-45c3-88ad-433105d5f8fa"  // Characteristic that defines a modules firmware version

#define SERVICE_RELAY_UUID        "eda0f9e6-e78f-11ea-adc1-0242ac120002"  // Service for relay interaction
#define CHARACTERISTIC_NR         "a96f34fe-19f7-401f-873a-30462c535402"  // Characteristic that tells you how many relays the board has
#define CHARACTERISTIC_RS         "d1482882-1fd0-4f72-96bd-b6c953345653"  // Characteristic for the state of the relays

#define MODULE_NAME               "ESP32LR20"
#define MODULE_ID                 "39"
#define MODULE_V                  "1"

#define CONNECT_LED               23

char relay_st[ NUMBER_OF_RELAYS + 1 ];

/**
 * Class that handles callbacks to the relay state 
 * characteristic.
 * 
 * Allows the getting and setting of the relays from
 * reading and writing the characteristic!
 */

class RelayCallbacks: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *pCharacteristic) {
 
      std::string value = pCharacteristic->getValue();

      if ( value.length() >= 2 ) {
        set_relay_states( value.c_str() );
      }
      
    }

    void onRead(BLECharacteristic *pCharacteristic) {
      
      get_relay_states( relay_st );
      pCharacteristic->setValue( relay_st );
      
    }
    
};

/**
 * Class that handles callbacks for connection and disconnection.
 */
class ConnectionCallbacks: public BLEServerCallbacks {
    void onConnect( BLEServer* pServer ) {
      digitalWrite( CONNECT_LED, LOW );
    };

    void onDisconnect(BLEServer* pServer) {
      digitalWrite( CONNECT_LED, HIGH );
    }
};

void setup() {

  pinMode( CONNECT_LED, OUTPUT );
  digitalWrite( CONNECT_LED, HIGH );

  init_relays();
  
  BLEDevice::init( MODULE_NAME );
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks( new ConnectionCallbacks() );
  init_module_service( pServer );
  init_relay_service( pServer );
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  
  pAdvertising->addServiceUUID( SERVICE_MODULE_UUID );
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  
  BLEDevice::startAdvertising();
  
}

void loop() {
  
}

/**
 * Init the service that provides information about the module.
 * This service provides characteristics for the module ID and the 
 * firmware version.
 * 
 */
void init_module_service( BLEServer * pServer ) {
  
  BLEService *mService = pServer->createService( SERVICE_MODULE_UUID );
  
  BLECharacteristic *idCharacteristic = mService->createCharacteristic(
                                         CHARACTERISTIC_ID,
                                         BLECharacteristic::PROPERTY_READ
                                       );
  
  idCharacteristic->setValue( MODULE_ID );
  
  BLECharacteristic *vCharacteristic = mService->createCharacteristic(
                                         CHARACTERISTIC_VER,
                                         BLECharacteristic::PROPERTY_READ
                                       );
  
  vCharacteristic->setValue( MODULE_V );
  
  mService->start();
  
}

/**
 * Init the service that provides the interface fot the relays on the module.
 */
void init_relay_service( BLEServer * pServer ) {

  BLEService *mService = pServer->createService( SERVICE_RELAY_UUID );
  
  BLECharacteristic *idCharacteristic = mService->createCharacteristic(
                                         CHARACTERISTIC_NR,
                                         BLECharacteristic::PROPERTY_READ
                                       );

  idCharacteristic->setValue( String( NUMBER_OF_RELAYS ).c_str() );
  
  BLECharacteristic *vCharacteristic = mService->createCharacteristic(
                                         CHARACTERISTIC_RS,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  
  vCharacteristic->setCallbacks( new RelayCallbacks() );
  
  mService->start();

}
