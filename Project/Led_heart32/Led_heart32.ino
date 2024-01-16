void setup() {
  Serial.begin(9600);
}

void loop() {
  uint32_t x=0,y=0,z=0,t,i;
  y = (uint32_t)1<<17;
  Serial.println(y,BIN);
  for(i=0;i<32;i++){
    x = ~(1UL<<i);
    y = x >> 8;
    z = x >> 16;
    t = x >> 24;
    Serial.println(x,BIN);
    Serial.println(y,BIN);
    Serial.println(z,BIN);
    Serial.println(t,BIN);
    Serial.println("--------------------------------");
    delay(1000);
  }
}
