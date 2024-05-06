#include <SPI.h>

const int dataPin = 12; // Chân MISO của Slave
int receivedData;

void setup() {
  SPI.begin(); // Khởi tạo SPI
  Serial.begin(9600); // Khởi tạo Serial
  pinMode(dataPin, OUTPUT); // Cài đặt chân MISO là OUTPUT
  SPI.setBitOrder(MSBFIRST); // Thiết lập thứ tự truyền nhận bit
  SPI.setClockDivider(SPI_CLOCK_DIV32); // Thiết lập tốc độ truyền SPI là 500kHz
  SPI.setDataMode(SPI_MODE0); // Thiết lập chế độ truyền dữ liệu
}

void loop() {
  if (SPI.transfer(0) == 5) { // Kiểm tra xem có dữ liệu nào được gửi từ Master không
    receivedData = SPI.transfer(0); // Nhận dữ liệu từ Master
    receivedData *= 2; // Nhân đôi dữ liệu nhận được
    Serial.print("Received data from Master: ");
    Serial.println(receivedData);
    SPI.transfer(receivedData); // Gửi dữ liệu đã xử lý trở lại cho Master
  }
}
