#include <CAN.h>
#include <Arduino.h>
//============================================define============================================
#define TX_GPIO_NUM 21  // Connects to CTX
#define RX_GPIO_NUM 22  // Connects to CRX
uint8_t  txdata[16];

void Tx_can(uint16_t id, int sz, uint8_t data[8]) {
  CAN.beginPacket(id);  // Thiết lập ID của tin nhắn
  CAN.write(data, sz);  // Gửi dữ liệu
  CAN.endPacket();
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    while (!Serial)
    ;
  delay(1000);
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);
  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    while (1);
  } else {
  }
}
void loop() {
  // Đọc dữ liệu từ cổng serial
  int RxData[16];
  if (CAN.parsePacket()) {
    // Đọc kích thước dữ liệu của tin nhắn CAN
    int packetSize = CAN.packetDlc();
    uint16_t   id  =CAN.packetId();
    // Lưu dữ liệu từ tin nhắn CAN vào mảng txdata
    txdata[0] = 0x2E;
    txdata[1] = 0x41;
    txdata[2] = 0x0B;
    txdata[3] = (packetSize  << 4) | ((id & 0x0f00)>>8);
    txdata[4] = (uint8_t)(id & 0xFF);
    for (int i = 0; i < packetSize; i++) {
      txdata[5 + i] = CAN.read();
    }
    txdata[13] = 0x00;
    txdata[14] = 0x13;
    txdata[15] = 0x10;

    Serial.write(txdata, sizeof(txdata));
    //Serial.flush();
  } 

  if(Serial.available())
  {
    RxData[0] = Serial.read();
    if(RxData[0] == 0x2E)
    {
      for (int i = 1; i < 16; i++) {
        //while (!Serial.available());
        RxData[i] = Serial.read(); // Đọc dữ liệu và lưu vào mảng receivedData
      }
      uint16_t tmp = (RxData[3] << 8) | RxData[4];
      // Lấy 4 bit đầu tiên để gán vào biến sz
      int sz = (tmp >> 12) & 0x0F;
      uint16_t id = tmp & 0x0FFF;
      uint8_t data[8]; // Khai báo mảng data với kích thước 8
      for (int i = 0; i < 8; i++) {
        data[i] = RxData[i + 5]; // Sao chép dữ liệu từ phần tử thứ 6 đến thứ 13 vào mảng data
      }
      // Gọi hàm TX_can để gửi dữ liệu qua giao thức CAN
      Tx_can(id, sz, data);
      //}    
    }
  }
  }