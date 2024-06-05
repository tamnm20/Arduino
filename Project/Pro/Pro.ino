 /*** Include library**********/
// Thu vien cam bien van tay
#include <Adafruit_Fingerprint.h>
//Thu vien hien thi LCD
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
//Thu vien RFID
#include <SPI.h>
#include <MFRC522.h>
//Thu vien Keypad
#include <Keypad.h> 
/**********Define***********/
/**********finger **********/
int getFingerprintIDez();
#define mySerial Serial2
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
bool FINGER_CHECK();
/*******keypad**************/
const byte n_nk = 6;    //num num key
const byte rows = 4;    //rows
const byte columns = 3; //columns
char correctPassword[n_nk] = {'1', '3', '7', '9','4','8'}; //Correct password
char enteredPassword[n_nk];                        //Entered password on keypad
char charKey;                                   //Key on keypad 
int i = 0;                                      //Counter
char keypad[4][3] =                             //Keypad array
{ 
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

byte rowsPins[4] = {13, 12, 14, 27};           //Rows pins 
byte columnspPins[3] = {26, 25, 33};         //Columns pins 

Keypad kpad = Keypad(makeKeymap(keypad), rowsPins, columnspPins, 4, 3);//Keypad map
bool KEY_CHECK();
bool Key_check = true;
/******MRC522****************/
#define RST_PIN         15
#define SS_PIN          5
/****LED*AND*BUZZER***/
#define LED_RED         2
#define LED__BLUE       4
#define BUZZER          3
uint32_t Pass_RFID[3]={0xB9FDD1B8,0xDBFF670E,0x00000000};
MFRC522 mfrc522(SS_PIN, RST_PIN);
bool lookID = true;
/******LCD1602**************/
LiquidCrystal_I2C lcd(0x27, 16, 2); 
//****VARIABLES USED****//
uint8_t time_fail = 0;
void LOCK5S();
bool lookingForFinger = true;
bool locked = true; //The door remains locked at first
void OPEN_DOOR();
void PASS_FAIL();
void loop_finger();
void loop_keypad();
void loop_MRC522();
int Select_option();
void locked_90();
bool look;
bool check = true;
void setup() {
  Serial.begin(9600);
  /***setup*LED*AND*BUZZER***/
  pinMode(LED_RED,OUTPUT);
  pinMode(LED__BLUE,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  digitalWrite(LED__BLUE, LOW);
  digitalWrite(LED_RED, HIGH);
  /***MRC522*******/
  SPI.begin();
  mfrc522.PCD_Init();
  /***setup LCD*******/
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  STARTINGG!!!");
  Serial.println("  STARTINGG!!!");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("WAITING CONNECT!");
  Serial.println("WAITING CONNECT!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SECLECT TOOL");
  Serial.println(" SECLECT TOOL ");
  delay(1000);
  /***setup finger****/
  finger.begin(57600);
  delay(5);
  /***setup keypad****/
}
int option =0;
void loop() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WELCOME TO HOME!");
  Serial.println("WELCOME TO HOME!");
  digitalWrite(LED__BLUE, LOW);
  digitalWrite(BUZZER, LOW);
  lcd.setCursor(0,1);
  lcd.print("SECLECT TOOL");
  Serial.println(" SECLECT TOOL ");
  delay(1000);
  option =0;
  check = true;
  look = true;
  locked = true;
  lookID = true;
  lookingForFinger = true;
  Key_check = true;
  option = Select_option();
  lcd.clear();
  switch(option){
    case 1: 
      loop_finger();
      break;
    case 2:
      loop_keypad();
      break;
    case 3:  
      loop_MRC522();
      break;  
  }
  if(!look){
     LOCK5S();
  }
}
void loop_finger(){
  Serial.println("SCANNER FINGER!");
  lcd.clear();
  lcd.print("SCANNER FINGER!");
  look = FINGER_CHECK();
}
void loop_keypad(){
  Serial.println("Enter password: ");
  lcd.clear();
  lcd.print("Enter password: ");
  look = KEY_CHECK();
}
void loop_MRC522(){
  Serial.println("Scan the card: ");
  lcd.clear();
  lcd.print("Scan the card: ");
  look = RFID_CHECK();
}

//Ham canh bao tu dong khoa sau 5s
void LOCK5S(){
    time_fail=0;
    digitalWrite(LED__BLUE, HIGH);
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER, LOW);
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CLOSE DOOR AFTER:\n");
    Serial.println("CLOSE DOOR AFTER:\n");
    for(byte time__=5;time__>0;time__--){
        digitalWrite(BUZZER, HIGH);
        delay(500);
        Serial.printf("%d\n",time__);
        lcd.setCursor(7,1);
        lcd.print(time__);
        digitalWrite(BUZZER, LOW);
        delay(500);
        }
    locked = true;
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED__BLUE, LOW);
    digitalWrite(LED_RED, HIGH);
}
//Ham khoa 90s
void locked_90(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TRY AGAIN AFTER:");
    lcd.setCursor(3, 1);
    lcd.print("90s: ");
    delay(500);
    for(byte time__=9;time__>0;time__--){
      lcd.setCursor(8, 1);
      digitalWrite(LED_RED, HIGH);
      if (time__ == 9||time__==8) digitalWrite(BUZZER, HIGH);
      else digitalWrite(BUZZER, LOW);
      lcd.print(time__);
      Serial.printf("%d\n",time__);
      delay(1000);
    }  
     time_fail=0;
  }
//Ham bao sai
void PASS_FAIL(){
  time_fail++;
    Serial.println("WRONG PASSWORD!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WRONG PASSWORD!");
  digitalWrite(LED_RED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(500);
  if(time_fail == 3) locked_90();
  }
/*******keypad**************/
bool KEY_CHECK(){
  while(Key_check){
    charKey = kpad.getKey();  
    if(charKey)  
    {
      Serial.println(charKey);
      if (charKey > 47 && charKey < 58) {
        lcd.setCursor(i,1);
        lcd.print("*");
        enteredPassword[i++] = charKey; 
      } 
    }
    if (i == n_nk)
    { 
      delay(200);
      if ((strncmp(enteredPassword, correctPassword, n_nk) == 0)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Acess Granted");
        Serial.println("Acess Granted");
        lcd.setCursor(0, 1);
        lcd.print("OPEN DOOR");
        Serial.println("OPEN DOOR");
        locked = false;
        Key_check = false;
        i=0;
        delay(500);
      }
      else {
      Serial.println("\nAcess Denied\n");
          PASS_FAIL();
      i = 0;
      return locked;
      }
    }
  }
  return locked;
}
//Ham chon che do mo khoa
int Select_option(){
    
    bool kpad_stt = kpad.getKey();
    bool finger_stt = finger.getImage();
    bool mfrc522_stt = mfrc522.PICC_IsNewCardPresent() ;
    bool mfrc522_rcs = mfrc522.PICC_ReadCardSerial();
    
    while((finger_stt != FINGERPRINT_OK) && (!kpad_stt) &&  (!mfrc522_stt) && (!mfrc522_rcs)){
      kpad_stt = kpad.getKey();
      finger_stt = finger.getImage();
      mfrc522_stt = mfrc522.PICC_IsNewCardPresent() ;
      mfrc522_rcs = mfrc522.PICC_ReadCardSerial();   
      }
  if(finger_stt == FINGERPRINT_OK)  return 1;
    if (kpad_stt)             return 2;
    if ( mfrc522_stt && mfrc522_rcs )   return 3;
  }
/**********finger **********/
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
bool FINGER_CHECK(){
  if (lookingForFinger) {
        uint8_t p = finger.image2Tz();
        uint8_t id = getFingerprintIDez();
        Serial.println("finger.image2Tz: ");
        Serial.println(p,HEX);
        Serial.println("finger.getImage: ");
        Serial.println(id,HEX);
        if (id == 11 || id == 12 || id == 14) {
        Serial.println(id);
          if (id >= 10 && id < 30) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("FINGER VERIFIED");
            Serial.println("FINGER VERIFIED");
            lcd.setCursor(0, 1);
            lcd.print("OPEN DOOR");
            Serial.println("OPEN DOOR");
            locked = false;
            lookingForFinger = false;
            delay(500);
            }
        }
        else if(p==0x00){
            PASS_FAIL();
        }
        else  {
            lcd.clear();
            Serial.println("SCANNER FINGER!");
            lcd.setCursor(0, 0);
            lcd.print("SCANNER FINGER!");
        }
        return locked;
    }
}
/******MRC522****************/
bool RFID_CHECK(){
  while(lookID){
    uint32_t id_RF;
    for(int n_id=0;n_id <3;n_id++){
    id_RF= Pass_RFID[n_id];
    Serial.println("ID thẻ: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      if((((id_RF & ((0xff000000)>>(i*8))) >> ((3-i)*8)) != mfrc522.uid.uidByte[i])){
        check = false;
        Serial.println("");
        break;
      }
      if(i==3) Serial.println("");
      check = true;
    }
    if(check) break;
  }
  Serial.println(id_RF,HEX);
  if(check) {
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ID Correct");
    Serial.println("ID Correct");
    lcd.setCursor(0, 1);
    lcd.print("OPEN DOOR");
    Serial.println("OPEN DOOR");
    locked = false;
    lookID = false;
    delay(500);
  }
  else {
    Serial.println("PASS FAIL");
      PASS_FAIL();
    return locked;
  }
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  }
  return locked;
}
