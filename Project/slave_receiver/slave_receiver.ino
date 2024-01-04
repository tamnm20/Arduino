// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


//#include <Wire.h>
//
//void setup() {
//  Wire.begin(0x27);                // join i2c bus with address #8
//  Wire.onReceive(receiveEvent); // register event
//  Serial.begin(9600);           // start serial for output
//}
//
//void loop() {
//  //delay(100);
//}
//
//// function that executes whenever data is received from master
//// this function is registered as an event, see setup()
//void receiveEvent(int howMany) {
////  while (1 < Wire.available()) { // loop through all but the last
////    char c = Wire.read(); // receive byte as a character
////    Serial.print(c);         // print the character
////  }
//  int x = Wire.read();    // receive byte as an integer
//  Serial.println(x,HEX);         // print the integer
//}

#include <Wire.h>


#define SLAVE_ADDRESS_LCD  0x27
#define ISP_SLAVE_ADDRESS  0x48


int DATA_LENGTH =0;

//byte receivedData[DATA_LENGTH];
char receivedData[10];
byte dataToSend[2]={0x30, 0x00};

void setup() {
  Wire.begin(ISP_SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);
//  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop() {
  delay(100);
//  if(DATA_LENGTH > 2){
//    processReceivedData();
//  }
}

void receiveEvent() {
  DATA_LENGTH = Wire.available();
  if (DATA_LENGTH > 0 ) {
    for (int i = 0; i < DATA_LENGTH; i++) {
      receivedData[i] = Wire.read();
    }
  }
}

void processReceivedData() {
  // Xử lý dữ liệu nhận được, ví dụ:
  Serial.print("Received Data: ");
  for (int i = 0; i < DATA_LENGTH; i++) {
    Serial.print(receivedData[i], HEX);
    //Serial.print(receivedData[i]);
    Serial.print(" ");
  }
  Serial.println();
  //Serial.println(receivedData);
}

void requestEvent() {
  // Trả lời yêu cầu của Master bằng cách gửi dữ liệu
  Wire.write(dataToSend, sizeof(dataToSend));
//  for (int i = 0; i < sizeof(dataToSend); i++) {
//    Serial.print(dataToSend[i], HEX);
//    //Serial.print(receivedData[i]);
//    Serial.print(" ");
//  }
//  Serial.println();
}
