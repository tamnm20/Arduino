#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

char ssid[32] = "cuckoo";
char password[32] = "68686867";
const char* ap_ssid = "ESP01S_AP";
const char* ap_password = "12345678";

// Đặt địa chỉ IP tĩnh cho AP mode
IPAddress ap_ip(192, 168, 1, 1);
IPAddress ap_gateway(192, 168, 1, 1);
IPAddress ap_subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

// Initialize Telegram BOT
#define BOTtoken "5765659274:AAF1MrBZwC52uunp-t2CE6KhQn2yEmvKaik"  // your Bot Token (Get from Botfather)
#define CHAT_ID "5348160018"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 2;
bool ledState = LOW;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    Serial.print("chat_id: ");
    Serial.println(chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on to turn GPIO ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome);
    }

    if (text == "/led_on") {
      bot.sendMessage(chat_id, "LED state set to ON");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/state") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "LED is ON");
      }
      else{
        bot.sendMessage(chat_id, "LED is OFF");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Read SSID and Password from EEPROM
  readCredentials();
  //saveCredentials();
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Wait for WiFi connection for 10 seconds
  unsigned long startTime = millis();
  Serial.print("\nConnecting to WiFi......");
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(100);
  }
  // If not connected, switch to AP mode
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWiFi connection failed, switching to AP mode");
    setupAP();
  } else {
    Serial.println("\nWiFi connected successfully");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Retrieving time: ");
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
    time_t now = time(nullptr);
    while (now < 24 * 3600)
    {
      Serial.print(".");
      delay(100);
      now = time(nullptr);
    }
    Serial.println(now);
  }
}

void loop() {
//  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
//    lastTimeBotRan = millis();
//  }
  //server.handleClient();
  if (WiFi.status() != WL_CONNECTED) {
    server.handleClient();
  }
  handleSerialInput();
}

void setupAP() {
  Serial.println("\nWiFi AP mode");
  WiFi.mode(WIFI_AP);
  // Cấu hình địa chỉ IP tĩnh cho AP
  WiFi.softAPConfig(ap_ip, ap_gateway, ap_subnet);
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
void handleSerialInput() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Loại bỏ các ký tự xuống dòng và khoảng trắng
    if (command == "AT+ALARM=1") {
      Serial.println("Canh bao chay!!!Ve nha ngay!!!");
      bot.sendMessage("5348160018", "Canh bao chay!!!Ve nha ngay!!!");
    } else {}
  }
}
