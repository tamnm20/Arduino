const int speaker = 4;//set speaker to pin 8
void setup() {
  //Serial.begin(9600);//begin the serial monitor
  // put your setup code here, to run once:
  pinMode(speaker, OUTPUT);//define the speaker as an output
}

void loop() {
  // put your main code here, to run repeatedly:
  tone(speaker, 800, 800);

  delay(200);

  tone(speaker, 600, 800);

  delay(200);
}
