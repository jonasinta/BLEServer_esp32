#include "Arduino.h"
/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

 BLECharacteristic *pCharacteristic; //global for the characterisy=tic, that way i can access it in loop

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      BLEUUID gotUUID = pCharacteristic->getUUID();
      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
      }
      if (gotUUID.bitSize() > 0) {
    	  std::string valueUUID= gotUUID.toString();
        for (int i = 0; i < valueUUID.length(); i++)
                  Serial.print(valueUUID[i]);

    }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("GasBottleTransmitterONE");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
 pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
										 BLECharacteristic::PROPERTY_NOTIFY
                                       );
 pCharacteristic->setNotifyProperty(true);
  pCharacteristic->setValue("Hello World says Neil");
// pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
 // pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
 // pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}
int i =1;

void loop() {
  // put your main code here, to run repeatedly:
	String string4int;
	std::string Cstring;

	 i=i*4;
	 string4int= String(i);
	string4int = string4int + "Hello World says Neil";
	 Cstring = string4int.c_str();
	 Serial.println(string4int);
	pCharacteristic->setValue(Cstring);
	pCharacteristic->notify();
  delay(2000);
}
