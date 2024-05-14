e #include <SoftwareSerial.h>
#define TIMEOUT 5000 // mS
SoftwareSerial mySerial(7, 6); // RX, TX
const int button = 11;
int button_state = 0;
 
void setup()
{
 pinMode(LED_BUILTIN,OUTPUT);
 pinMode(button,INPUT); 
 Serial.begin(115200);
 mySerial.begin(115200);
 //SendCommand("AT+RST", "OK");
 delay(5000);
 SendCommand("AT+CWMODE=1","OK");
 SendCommand("AT+CIFSR", "OK");
 SendCommand("AT+CIPMUX=1","OK");
 SendCommand("AT+CIPSERVER=1,80","OK");
}
 
void loop(){
 button_state = digitalRead(button);
 
 if(button_state == HIGH){
    mySerial.println("AT+CIPSEND=0,23");
    delay(10);
    mySerial.println("Button was pressed!");
    delay(1000);
    SendCommand("AT+CIPCLOSE=0","OK");
  }
  
 String IncomingString="";
 boolean StringReady = false;
 
 while (mySerial.available()){
   IncomingString=mySerial.readString();
   StringReady= true;
  }
 
  if (StringReady){
    Serial.println("Received String: " + IncomingString);
  
  if (IncomingString.indexOf("LED=ON") != -1) {
    digitalWrite(LED_BUILTIN,HIGH);
   }
 
  if (IncomingString.indexOf("LED=OFF") != -1) {
    digitalWrite(LED_BUILTIN,LOW);
   }
  }
 }
 
boolean SendCommand(String cmd, String ack){
  mySerial.println(cmd); // Send "AT+" command to module
  if (!echoFind(ack)) // timed out waiting for ack string
    return true; // ack blank or ack found
}
 
boolean echoFind(String keyword){
 byte current_char = 0;
 byte keyword_length = keyword.length();
 long deadline = millis() + TIMEOUT;
 while(millis() < deadline){
  if (mySerial.available()){
    char ch = mySerial.read();
    Serial.write(ch);
    if (ch == keyword[current_char])
      if (++current_char == keyword_length){
       Serial.println();
       return true;
    }
   }
  }
 return false; // Timed out
}
