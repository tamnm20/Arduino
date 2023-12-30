#define FULL 10
#define HALF FULL/2

#define SCL_PIN 3
#define SDA_PIN 4

#define SDA_OUTPUT DDRD |= (1 << DDB4)
#define SCL_OUTPUT DDRD |= (1 << DDB3)

#define SDA_INPUT DDRD &= ~(1 << DDB4)
#define SCL_INPUT DDRD &= ~(1 << DDB3)

#define SDA_HIGH PORTD |= (1<<PD4)  //SDA =1
#define SCL_HIGH PORTD |= (1<<PD3)  //SCL =1
#define SDA_LOW PORTD &= ~(1<<PD4)  //SDA =0
#define SCL_LOW PORTD &= ~(1<<PD3)  //SCL =0

#define SDA_READ (PIND & (1 << PIND4)) >> PIND4  //1/0
#define SCL_READ (PIND & (1 << PIND3)) >> PIND3  //1/0
//byte data[4];
char data[8];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100); 
}

void loop() {
  I2C_readBuff(0x40,data,8);
  for(byte i=0;i<8;i++)
  {
    Serial.print(data[i]);
    Serial.print(" ");
  }
//  //Serial.print(data);
  Serial.println(" ");
  delay(100);
}
int8_t I2C_readByte(){
  int8_t tmp = 0;
  for(byte i=0;i<8;i++)
  {
    while(SCL_READ ==0);
    tmp = (tmp << 1) | SDA_READ;
    while(SCL_READ ==1);
  }
  return tmp;
}
void ACK()
{
  SDA_LOW;
  SDA_OUTPUT;
  while(SCL_READ ==0);
  while(SCL_READ ==1);
  SDA_INPUT;
}
int8_t I2C_readBuff(byte address, byte *Reiv, uint8_t len){
  
  int8_t i;
  SDA_INPUT;
  SCL_INPUT;
  
  // doi start
  if( (SDA_READ ==1) && (SCL_READ == 1))
  {
    //Serial.println("Tu");
    while((SDA_READ !=1) || (SCL_READ != 1));
    while( SDA_READ == 1); 
    
    //delayMicroseconds(HALF/2);
    if(SCL_READ == 1)
    {
      while(SCL_READ ==1);    
      int8_t tmp = I2C_readByte();
      if(tmp == address)
      {
        ACK();
        for(byte i=0;i<8;i++)
        {
          Reiv[i] = I2C_readByte();
          ACK();
        }
      }
      return 1;
    }
  }
  else{
    return 0;
  }
}
