#ifndef Can_Analyzer_h
#define Can_Analyzer_h

#include <avr/interrupt.h>
#include <avr/io.h>
#include <String.h>
#include "RingBuffer.h"
class Uart {
  private:

  public:
    Uart(){

    }
    void begin(int baurd);
    void send_byte(uint8_t data);
    void send_buf(uint8_t  data[],byte len);
    void send_str(char*  str);
};



void Uart::begin(int baurd){
  switch(baurd)
  {
    case 9600: 
      UBRR0 = 103;
    break;  
    case 38400: 
      UBRR0 = 25;
    break;
    default : 
      UBRR0 = 103;
    break;  
  }

   UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);  // Use 8-bit character sizes
   UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);  
  /*
  RXEN0: Bật chế độ nhận (Receiver Enable).
  TXEN0: Bật chế độ truyền (Transmitter Enable).
  RXCIE0: Bật ngắt nhận (Receive Complete Interrupt Enable)*/
   sei();// enable global interrupt
}
void Uart::send_byte(uint8_t data){
  // Chờ cho đến khi bộ đệm truyền trống
  while (!(UCSR0A & (1 << UDRE0)));
  
  // Đặt dữ liệu vào thanh ghi truyền
  UDR0 = data;
}
void Uart::send_buf(uint8_t  data[],byte len){
  for(uint8_t i = 0; i < len;i++){
    Uart::send_byte(data[i]);
  }

}

void Uart::send_str(char*  str){
  for(uint8_t i = 0; i < strlen(str);i++){
    Uart::send_byte(str[i]);
  }
  Uart::send_byte('\r');
  Uart::send_byte('\n');
}
#endif
