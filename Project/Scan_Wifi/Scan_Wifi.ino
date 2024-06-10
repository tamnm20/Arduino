#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

const char* ap_ssid = "ESP01S_AP";
const char* ap_password = "12345678";

ESP8266WebServer server(80);

char ssid[32] = "cuckoo";
char password[32] = "68686867";

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

  // Read SSID and Password from EEPROM
  readCredentials();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for WiFi connection for 5 seconds
  unsigned long startTime = millis();
  Serial.print("\nConnecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(100);
    Serial.print(".");
  }

  // If not connected, switch to AP mode
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWiFi connection failed, switching to AP mode");
    setupAP();
  } else {
    Serial.println("\nWiFi connected successfully");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  server.handleClient();
}

void setupAP() {
  Serial.println("\nWiFi AP mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Access Point initialized");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", "<form action=\"/save\" method=\"post\">SSID: <input type=\"text\" name=\"ssid\"><br>Password: <input type=\"password\" name=\"password\"><br><input type=\"submit\" value=\"Save\"></form>");
  });

  server.on("/save", HTTP_POST, []() {
    String new_ssid = server.arg("ssid");
    String new_password = server.arg("password");

    new_ssid.toCharArray(ssid, 32);
    new_password.toCharArray(password, 32);

    // Save SSID and Password to EEPROM
    saveCredentials();

    server.send(200, "text/html", "SSID and Password saved. Restarting to connect.");
    delay(1000);
    ESP.restart();
  });

  server.begin();
}

void readCredentials() {
  for (int i = 0; i < 32; ++i) {
    ssid[i] = EEPROM.read(i);
    password[i] = EEPROM.read(32 + i);
  }
}

void saveCredentials() {
  for (int i = 0; i < 32; ++i) {
    EEPROM.write(i, ssid[i]);
    EEPROM.write(32 + i, password[i]);
  }
  EEPROM.commit();
}
