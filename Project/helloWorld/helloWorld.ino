#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0X27,16,2); //SCL A5 SDA A4
void setup()
{
  lcd.init();                    
  lcd.backlight(); 
}

void loop()
{
  lcd.setCursor(2,0);
  lcd.print("DIEN TU DAT");
  lcd.setCursor(0,1);
  lcd.print("Bai Test LCD I2C "); 
  delay (100);
 
  }