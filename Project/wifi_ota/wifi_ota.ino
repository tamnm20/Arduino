#include "iot47_wifi_ota.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#else
#include <WiFi.h>
#include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
const int ledPin = 2; // Pin kết nối đèn LED

unsigned long previousMillis = 0; // Biến để lưu thời điểm lần cuối cùng thực hiện hành động
const long interval = 100;      // Thời gian chờ giữa các lần thay đổi trạng thái (milliseconds)

int ledState = LOW;              // Trạng thái hiện tại của LED
void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("\nConnect to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  //iot47_ota_set_debug(false);
  iot47_wifi_ota_begin(&server);
  server.begin();
}

void loop() 
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Lưu thời điểm hiện tại là thời điểm lần cuối cùng thực hiện hành động
    previousMillis = currentMillis;
  
    // Đảo trạng thái của LED
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }
  iot47_wifi_ota_loop();
}
