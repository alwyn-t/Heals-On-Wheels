/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE" 
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second. 
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "health_check.h"

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
#define ford 19 // 36
#define tleft 18 // 39
#define tright 5 // 34
#define done 17 // 35

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// #define LED_BUILTIN 23
void go_forward();
void turn_right();
void turn_left();

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.println();
        Serial.print("Command Input: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);
        Serial.println();
        //Serial.println("*********");
        
        
        // Run all checks for control inputs and run respective functions --------------------------------------------------------
        // begin commands
        int input_length = rxValue.length();
        // const std::string _command = "";
        const std::string help_command = "!help";
        const std::string check_patient_command = "!check-patient-";
        const std::string check_all_command = "!check-all-patients";
        const std::string meds_to_patient_command = "!meds-to-patient-";
        const std::string medicate_all_command = "!medicate-all";


        if (input_length == help_command.length() + 1 && rxValue.find(help_command, 0) == 0) {
          Serial.println("help command executed");
          pTxCharacteristic->setValue("help command executed");
          pTxCharacteristic->notify();
          //Serial.println("");
          Serial.println("use !check-patient-<patient number> command to check patient of number <patient number>");
          pTxCharacteristic->setValue("use !check-patient-<patient number> command to check patient of number <patient number>");
          pTxCharacteristic->notify();
          //Serial.println("");
          Serial.println("use !check-all-patients command to check all patients");
          pTxCharacteristic->setValue("use !check-all-patients command to check all patients");
          pTxCharacteristic->notify();
          //Serial.println("");
          Serial.println("use !meds-to-patient-<patient number> command to deliver medicine to patient of number <patient number>");
          pTxCharacteristic->setValue("use !meds-to-patient-<patient number> command to deliver medicine to patient of number <patient number>");
          pTxCharacteristic->notify();
          //Serial.println("");
          Serial.println("use help command executed");
          pTxCharacteristic->setValue("use help command executed");
          pTxCharacteristic->notify();
          // yep, organization
          Serial.println();

        } else if ( /* input_length == check_patient_command.length() + 2 && */ rxValue.find(check_patient_command, 0) == 0) {

          if(rxValue[input_length-1] == '1' || rxValue[input_length-1] =='2' || rxValue[input_length-1] == '3' || rxValue[input_length-1] == '4') {
            //yes
            Serial.println("executing check-patient command"); // ?
            pTxCharacteristic->setValue("executing check-patient command");
            pTxCharacteristic->notify();
            Serial.println();

            // which patient patient should be checked

          } else{
            Serial.println("invalid patient number");
            pTxCharacteristic->setValue("invalid patient number");
            pTxCharacteristic->notify();
            Serial.println("use !help for list of valid commands");
            pTxCharacteristic->setValue("use !help for list of valid commands");
            pTxCharacteristic->notify();
            Serial.println();
          }

        } else if ( /* input_length == check_all_command.length() + 1 && */ rxValue.find("!check-all-patients", 0) == 0) {
          Serial.println("executing check-all-patients command . . .");
          //pTxCharacteristic->setValue("executing check-all-patients command . . .");
          //pTxCharacteristic->notify();

          go_forward();
          turn_left();
          go_forward();
          turn_right();
          //examinePatient("1"); // ----------------------- do {int z = function} while(z == -1);
          turn_right();

          // first patient examined

          go_forward();
          go_forward();
          turn_left();
          //examinePatient("2");
          turn_left();

          // second patient examined

          go_forward();
          turn_right();
          go_forward();
          turn_left();
          go_forward();
          turn_right();
          //examinePatient("3");
          turn_right();

          // third patient examined

          go_forward();
          go_forward();
          turn_left();
          //examinePatient("4");
          turn_left();
          go_forward();
          turn_left();
          go_forward();
          go_forward();
          turn_right();
          turn_right();

          // now, back to initial position

          Serial.println("check-all-patients command executed");
          pTxCharacteristic->setValue("check-all-patients command executed");
          pTxCharacteristic->notify();
          Serial.println();

        } else if ( /* input_length == meds_to_patient_command.length() + 2 && */ rxValue.find(meds_to_patient_command, 0) == 0) {

          Serial.println("executing meds-to-patient command");
          pTxCharacteristic->setValue("executing meds-to-patient command");
          pTxCharacteristic->notify();
          Serial.println();

        } else if ( /* input_length == medicate_all_command.length() + 1 && */ rxValue.find(medicate_all_command, 0) == 0) {
          Serial.println("executing medicate-all-patients command . . .");
          pTxCharacteristic->setValue("executing medicate-all-patients command . . .");
          pTxCharacteristic->notify();

          go_forward();
          turn_left();
          go_forward();
          turn_right();
          //examinePatient("1"); // ----------------------- do {int z = function} while(z == -1);
          
          turn_right();

          // first patient examined

          go_forward();
          go_forward();
          turn_left();
          //examinePatient("2");
          turn_left();

          // second patient examined

          go_forward();
          turn_right();
          go_forward();
          turn_left();
          go_forward();
          turn_right();
          //examinePatient("3");
          turn_right();

          // third patient examined

          go_forward();
          go_forward();
          turn_left();
          //examinePatient("4");
          turn_left();
          go_forward();
          turn_left();
          go_forward();
          go_forward();
          turn_right();
          turn_right();

          // now, back to initial position

          Serial.println("medicate-all-patients command executed");
          pTxCharacteristic->setValue("medicate-all-patients command executed");
          pTxCharacteristic->notify();
          Serial.println();

        } else {
          Serial.println("invalid command");
          Serial.println("use !help for list of valid commands");
          pTxCharacteristic->setValue("invalid command");
          pTxCharacteristic->notify();
          Serial.println();
          //Serial.println("*********");

        }

        // end commands -------------------------------------------------------------------------------------
        //delay(1000000);
      
      }
    }
};

void go_forward(){
  while(digitalRead(done) == LOW){
    digitalWrite(ford, HIGH);
    delay(10);
  }
  digitalWrite(ford, LOW);
  delay(10);
}
void turn_right(){
  while(digitalRead(done) == LOW){
    digitalWrite(tright, HIGH);
    delay(10);
  }
  digitalWrite(tright, LOW);
  delay(10);
}
void turn_left(){
  while(digitalRead(done) == LOW){
    digitalWrite(tleft, HIGH);
    delay(10);
  }
  digitalWrite(tleft, LOW);
  delay(10);
}

// void medicate(){
//   //yep
// }

void setup() { // -------------------------------------------------- setup ------------------------------
  
  pinMode(ford, OUTPUT);
  pinMode(tleft, OUTPUT);
  pinMode(tright, OUTPUT);
  pinMode(done, INPUT);
  
  Serial.begin(115200);
  while(!Serial);

  // pinMode(forward, OUTPUT);
  // pinMode(tleft, OUTPUT);
  // pinMode(tright, OUTPUT);
  // pinMode(done, INPUT);

  // setup for health_check
  setupAHT();
  setupMAX30105();

  // Create the BLE Device
  BLEDevice::init("UART Service");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

}

void loop() {
  if (deviceConnected) {
    // pTxCharacteristic->setValue(&txValue, 1);
    // pTxCharacteristic->notify();
    txValue++;
		delay(10); // bluetooth stack will go into congestion, if too many packets are sent
	}

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }

  // connecting
  if (deviceConnected && !oldDeviceConnected) {
  // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }

}
