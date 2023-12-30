#include <CAN.h>
#include <Arduino.h>
#include <esp32-hal-timer.h>
//============================================define============================================
#define TX_GPIO_NUM 21  // Connects to CTX
#define RX_GPIO_NUM 22  // Connects to CRX
//==========================================variables===========================================
hw_timer_t *timer = NULL;
uint8_t ADD_F[8] = { 0x04, 0x2E, 0xB0, 0x06, 0x01, 0x00, 0x00, 0x00 };   // add fp
uint8_t SCAN_F[8] = { 0x04, 0x2E, 0xB0, 0x06, 0x02, 0x00, 0x00, 0x00 };  // scan fp
uint8_t DEL_F[8] = { 0x04, 0x2E, 0xB0, 0x06, 0x03, 0x00, 0x00, 0x00 };   // delete fp

uint8_t Ex[8] = { 0x02, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 };          //extended session
uint8_t IOCBI[8] = { 0x04, 0x2F, 0xF0, 0x53, 0x03, 0x00, 0x00, 0x00 };       //IOCBI enroll coverage
uint8_t FPM_Enroll[8] = { 0x03, 0x22, 0xB0, 0x0C, 0x00, 0x00, 0x00, 0x00 };  // fpm enroll
uint8_t Flow[8] = { 0x30, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00 };        // flow control
// --- Sensor touch check database
uint8_t STC1[8] = { 0x02, 0x10, 0x03, 0x55, 0x55, 0x55, 0x55, 0x55 };
uint8_t STC2[8] = { 0x04, 0x2F, 0xF0, 0x51, 0x03, 0x00, 0x00, 0x00 };
uint8_t STC3[8] = { 0x03, 0x22, 0xB0, 0x0D, 0x55, 0x55, 0x55, 0x55 };
uint8_t STC4[8] = { 0x30, 0x00, 0x7F, 0x55, 0x55, 0x55, 0x55, 0x55 };

uint8_t RDBI1[8] = { 0x03, 0x22, 0xB0, 0x0C, 0x00, 0x00, 0x00, 0x00 };
uint8_t RDBI2[8] = { 0x03, 0x22, 0xB0, 0x0B, 0x00, 0x00, 0x00, 0x00 };
uint8_t ExS[8] = { 0x30, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t LED_Red[8] = { 0xFF, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00 };
uint8_t LED_Gre[8] = { 0x00, 0xFE, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00 };
uint8_t LED_Blu[8] = { 0x00, 0x00, 0xFC, 0x13, 0x00, 0x00, 0x00, 0x00 };
uint8_t LED_Whi[8] = { 0xFF, 0xFE, 0xFD, 0x13, 0x00, 0x00, 0x00, 0x00 };
uint8_t LED_Off[8] = { 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00 };

uint8_t Manu_Data_1[8] = { 0x10, 0x10, 0x2E, 0xB0, 0x08, 0x32, 0x30, 0x32 };
uint8_t Manu_Data_2[8] = { 0x21, 0x33, 0x30, 0x33, 0x33, 0x31, 0x30, 0x30 };
uint8_t Manu_Data_3[8] = { 0x22, 0x30, 0x30, 0x30, 0xAA, 0xAA, 0xAA, 0xAA };

uint8_t Manu_Year[8] = { 0x10, 0x10, 0x2E, 0xB0, 0x08, 0x32, 0x30, 0x32 };
uint8_t Manu_Month[8] = { 0x21, 0x33, 0x30, 0x33, 0x33, 0x31, 0x30, 0x30 };
uint8_t Manu_Day[8] = { 0x22, 0x30, 0x30, 0x30, 0xAA, 0xAA, 0xAA, 0xAA };
uint8_t Manu_Infor[8] = { 0x03, 0x22, 0xF1, 0x8B, 0x00, 0x00, 0x00, 0x00 };

uint8_t ECU_Seri_Num[8] = { 0x03, 0x22, 0xF1, 0x8C, 0x00, 0x00, 0x00, 0x00 };

uint8_t Sup_HW_Ver[8] = { 0x03, 0x22, 0xF1, 0x93, 0x00, 0x00, 0x00, 0x00 };
uint8_t System_name[8] = { 0x03, 0x22, 0xF1, 0x97, 0x00, 0x00, 0x00, 0x00 };
uint8_t HMC_KIA_Sw[8] = { 0x03, 0x22, 0xF1, 0xA0, 0x00, 0x00, 0x00, 0x00 };
uint8_t Suply_code[8] = { 0x03, 0x22, 0xF1, 0xA1, 0x00, 0x00, 0x00, 0x00 };
uint8_t SW_Unit_Num[8] = { 0x03, 0x22, 0xF1, 0xB0, 0x00, 0x00, 0x00, 0x00 };
uint8_t SW_Unit_Ver[8] = { 0x03, 0x22, 0xF1, 0xB1, 0x00, 0x00, 0x00, 0x00 };
uint8_t HSM_Lock_check[8] = { 0x04, 0x2E, 0xB0, 0x06, 0x00, 0x00, 0x00, 0x00 };

uint8_t Delete[8] = { 0x04, 0x2E, 0xB0, 0x06, 0x03, 0x00, 0x00, 0x00 };

uint8_t Check_7AA[8] = { 0x04, 0x6F, 0xF0, 0x51, 0x03, 0xAA, 0xAA, 0xAA };
uint8_t Check_7AA1[8] = { 0x10, 0x27, 0x62, 0xB0, 0x0c, 0xC0, 0x00, 0xC0 };
uint8_t Check_7AA2[2] = { 0x21, 0x00 };

uint8_t Check_2F2[8] = { 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
uint8_t Check_54F[8] = { 0x4F, 0x14, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF };
int currentIndex = 0;
int state_machine = 0;
//uint8_t value = CAN.read();
//==================================================================================//
// void TIMER ontimer() {

// }
void Print_TX(uint16_t id, int sz, uint8_t data[8]) {

  Serial.print("Tx ----- >");
  Serial.print("| 0x");
  Serial.print(id, HEX);
  Serial.print("  |  ");
  Serial.print(sz);
  Serial.print("  |     ");

  for (int i = 0; i < sz; i++) {
    Serial.print(data[i] < 0x10 ? " 0" : " ");
    Serial.print(data[i], HEX);
  }
  Serial.print("      |");
  Serial.println();
}
//==================================================================================//
void TX_can(uint16_t id, int sz, uint8_t data[8]) {
  CAN.beginPacket(id);  // Thiết lập ID của tin nhắn
  CAN.write(data, sz);  // Gửi dữ liệu
  CAN.endPacket();
}

void Check_Rx(uint16_t id, uint8_t data[8], uint8_t test) {
  if (CAN.packetId() == id) {
    if (test == data[currentIndex]) {
      currentIndex++;
      if (currentIndex >= sizeof(data)) {
        state_machine++;   // Đặt biến state_machine thành 0x01
        currentIndex = 0;  // Đặt lại vị trí hiện tại của chuỗi byte
      }
    }
  }
}
void FP_check_add() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, ADD_F);
    Print_TX(0x7a2, 8, ADD_F);
    Serial.println("Please, Sensor touch to add !");
    state_machine++;
  }
}
void FP_check_scan() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, SCAN_F);
    Print_TX(0x7a2, 8, SCAN_F);
    Serial.println("Please, Sensor touch to scan !");
    state_machine++;
  }
}
void FP_check_delete() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, DEL_F);
    Print_TX(0x7a2, 8, DEL_F);
    Serial.println("Delete FP !");
    state_machine++;
  }
}

void FP_check() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, ADD_F);
    Print_TX(0x7a2, 8, ADD_F);
    Serial.println("Please, Sensor touch !");
    state_machine++;
  }
  if (state_machine == 2) {
    delay(2000);
    TX_can(0x7a2, 8, SCAN_F);
    Print_TX(0x7a2, 8, SCAN_F);
    Serial.println("Please, Sensor touch !");
    state_machine++;
  }
}
void Check_driver_FP1() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, RDBI1);
    Print_TX(0x7a2, 8, RDBI1);
    state_machine++;
  }
  if (state_machine == 2) {
    TX_can(0x7a2, 8, ExS);
    Print_TX(0x7a2, 8, ExS);
    state_machine++;
  }
}
void Sensor_touch_check() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, STC1);
    Print_TX(0x7a2, 8, STC1);
    delay(100);
    TX_can(0x7a2, 8, STC2);
    Print_TX(0x7a2, 8, STC2);
    delay(100);
    Serial.println("Sensor touch, wait check Id & Data of 7AA");
    state_machine++;
  }
  if (state_machine == 2) {
    TX_can(0x7a2, 8, STC3);
    Print_TX(0x7a2, 8, STC3);
    TX_can(0x7a2, 8, STC4);
    Print_TX(0x7a2, 8, STC4);
    Serial.println("Sensor touch check done ! ");
    state_machine++;
  }
}
void Pixel() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, RDBI2);
    Print_TX(0x7a2, 8, RDBI2);
    state_machine++;
  }
  if (state_machine == 2) {
    TX_can(0x7a2, 8, ExS);
    Print_TX(0x7a2, 8, ExS);
    state_machine++;
  }
}
void Led() {
  if (state_machine == 0) {
    TX_can(0x2df, 8, LED_Red);
    Print_TX(0x2df, 8, LED_Red);
    delay(2222);
    TX_can(0x2df, 8, LED_Gre);
    Print_TX(0x2df, 8, LED_Gre);
    delay(2222);
    TX_can(0x2df, 8, LED_Blu);
    Print_TX(0x2df, 8, LED_Blu);
    delay(2222);
    TX_can(0x2df, 8, LED_Whi);
    Print_TX(0x2df, 8, LED_Whi);
    delay(2222);
    TX_can(0x2df, 8, LED_Off);
    Print_TX(0x2df, 8, LED_Off);
    delay(2222);
    state_machine++;
  }
}
void ECU_Serial_Number() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, ECU_Seri_Num);
    Print_TX(0x7a2, 8, ECU_Seri_Num);
    state_machine++;
  }
}
void Manufacturing_Data() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, Manu_Year);
    Print_TX(0x7a2, 8, Manu_Year);
    delay(200);
    TX_can(0x7a2, 8, Manu_Month);
    Print_TX(0x7a2, 8, Manu_Month);
    delay(200);
    TX_can(0x7a2, 8, Manu_Day);
    Print_TX(0x7a2, 8, Manu_Day);
    delay(200);
    TX_can(0x7a2, 8, Manu_Infor);
    Print_TX(0x7a2, 8, Manu_Infor);
    state_machine++;
  }
}
void HW_Version() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, Sup_HW_Ver);
    Print_TX(0x7a2, 8, Sup_HW_Ver);
    state_machine++;
  }
}
void System_nam() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, System_name);
    Print_TX(0x7a2, 8, System_name);
    state_machine++;
  }
}
void HMC_KIA() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, HMC_KIA_Sw);
    Print_TX(0x7a2, 8, HMC_KIA_Sw);
    state_machine++;
  }
}
void Suplier_code() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, Suply_code);
    Print_TX(0x7a2, 8, Suply_code);
    state_machine++;
  }
}
void SW_Unit_number() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, SW_Unit_Num);
    Print_TX(0x7a2, 8, SW_Unit_Num);
    state_machine++;
  }
}
void SW_Unit_version() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, SW_Unit_Ver);
    Print_TX(0x7a2, 8, SW_Unit_Ver);
    state_machine++;
  }
}
void HSM_lock() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, HSM_Lock_check);
    Print_TX(0x7a2, 8, HSM_Lock_check);
    state_machine++;
  }
}
void Delete_check_FP1() {
  if (state_machine == 0) {
    TX_can(0x7a2, 8, RDBI1);
    Print_TX(0x7a2, 8, RDBI1);
    state_machine++;
  }
  if (state_machine == 2) {
    TX_can(0x7a2, 8, ExS);
    Print_TX(0x7a2, 8, ExS);
    state_machine++;
  }
}

//==================================================================================//

void canReceiver() {
  // try to parse packet
  int packetSize = CAN.parsePacket();
  if (packetSize) {
    Serial.print("Rx <-- ");
    if (CAN.packetExtended()) {
      // Serial.print("extended ");
    }
    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
    }
    if (CAN.packetRtr()) {
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());
    } else {

      Serial.print("| 0x");
      Serial.print(CAN.packetId(), HEX);
      Serial.print("  |  ");
      Serial.print(packetSize);
      Serial.print("  |     ");
      while (CAN.available()) {
        uint8_t value = CAN.read();
        Check_Rx(0x7AA, Check_7AA, value);
        Check_Rx(0x2f2, Check_2F2, value);
        Check_Rx(0x7AA, Check_7AA1, value);
        //Check_Rx(0x54F, Check_54F, value);
  // if (CAN.packetId() == 0x54F) {
  //   if (value == Check_54F[currentIndex]) {
  //     currentIndex++;
  //     if (currentIndex >= sizeof(Check_54F)) {
  //       //state_machine++;   // Đặt biến state_machine thành 0x01
  //       Serial.println("FPM Sleep ");
  //       currentIndex = 0;  // Đặt lại vị trí hiện tại của chuỗi byte
  //     }
  //   }
  // }

        //Check_Rx(0x7AA, Check_7AA2, value);
        //Check_Rx(0x19F, Check_19F, value);

        Serial.print(value < 0x10 ? " 0" : " ");
        Serial.print(value, HEX);
      }
      Serial.print("      |");
    }


    Serial.println();
  }
  // Serial.println("giá trị state sau khi nhận được massage");
  //       Serial.print(state_machine);
}


//==================================================================================//
void setup() {
  Serial.begin(115200);
  // timer = timerBegin(0, 80, true); // Sử dụng timer 0, chia tần số 80 (1 microsecond per tick)
  // timerAttachInterrupt(timer, &onTimer, true); // Thiết lập hàm callback và kích hoạt ngắt
  // timerAlarmWrite(timer, 1000000, true); // Thiết lập thời gian 1s (1000000 microseconds)
  // timerAlarmEnable(timer); // Kích hoạt bộ hẹn giờ
  while (!Serial)
    ;
  delay(1000);
  Serial.println("CAN Receiver/Receiver");
  // Set the pins
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);
  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1)
      ;
  } else {
    Serial.println("CAN Initialized");
  }
}

//==================================================================================//
int cmd;
void loop() {

  if (Serial.available() > 0) {
    cmd = Serial.parseInt();  // Đọc lệnh từ Serial Monitor
    switch (cmd) {
      case 0:
        Serial.println("Fill in the next check, please!");
        state_machine = 0;
    };
  }
  switch (cmd) {
    case 1:
      FP_check_add();
      break;
    case 2:
      FP_check_scan();
      break;
    case 3:
      FP_check_delete();
      break;
    case 5:
      FP_check();
      break;
    case 6:
      Check_driver_FP1();
      //canReceiver();
      break;
    case 7:
      Sensor_touch_check();
      //canReceiver();
      break;
    case 8:
      Pixel();
      //canReceiver();
      break;
    case 13:
      Led();
      //canReceiver();
      break;
    case 18:
      ECU_Serial_Number();
      break;
    case 19:
      Manufacturing_Data();
      break;
    case 20:
      HW_Version();
      break;
    case 21:
      System_nam();
      break;
    case 22:
      HMC_KIA();
      break;
    case 23:
      Suplier_code();
      break;
    case 24:
      SW_Unit_number();
      break;
    case 25:
      SW_Unit_version();
      break;
    case 26:
      HSM_lock();
      break;
    case 27:
      Delete_check_FP1();
      break;
      //default:
      //Serial.println("Lệnh không hợp lệ.");
  }
  canReceiver();
}

//==================================================================================//
/* sử dụng số để truyền vào serial monitor 
if (Serial.available() > 0) {
    String input = Serial.readString(); // Đọc chuỗi từ Serial Monitor
    switch input.equeals
    if (input.equals("testled")) {
      test1();
    } else if (input.equals("testled1")) {
      test2();
    } else {
      Serial.println("Lệnh không hợp lệ.");
    }
  }
      // if (buffer[0] == 0x04 || buffer[1] == 0x6F || buffer[2] == 0xF0 || buffer[3] == 0x51 ||
      //     buffer[4] == 0x03 || buffer[5] == 0xaa || buffer[6] == 0xaa || buffer[7] == 0xaa) {
  */
// Serial.println();
// Serial.print("|   ID   |");
// Serial.print(" DLC | ");
// Serial.print("               DATA               | ");
// Serial.println();
// Serial.print("|   ID   |");
// Serial.print(" DLC | ");
// Serial.print("               DATA               | ");
// Serial.println();
// Serial.print("|   ID   |");
// String input = Serial.readString(); // Đọc chuỗi từ Serial Monitor
// if (input.equals("a")) {
//   Serial.println("Mời nhập lệnh kiểm tra tiếp theo ! ");
//   state_machine = 0 ;
// }