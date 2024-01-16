#include "Can_Analyzer.h"

RingBuffer USB_data(64);
Uart USB;

void transmitByte(uint8_t data);
void setup()
{
  USB.begin(38400);
}
uint8_t data[16] = {0x2E,0x44,0x10,0x81,0x23,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'\r','\n'};
void loop()
{

  if(USB_data.available())
  {
    USB.send_byte(USB_data.read());
  }
  
  for(byte i = 5; i < 13; i++)
  {
    data[i] +=i;
  }
  USB.send_buf(data,sizeof(data));
  //USB.send_str("Tu");
  delay(10);
}

ISR(USART_RX_vect)
{
  uint8_t temp; 
  temp=UDR0;// read the received data byte in temp
  USB_data.write(temp);
}
