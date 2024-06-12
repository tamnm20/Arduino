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
//#define BOTtoken "5765659274:AAF1MrBZwC52uunp-t2CE6KhQn2yEmvKaik"  // your Bot Token (Get from Botfather)
#define BOTtoken "7082207773:AAGKeothCOSTOA9Sfbz1IgElPxsvU4LDR0Y"
String CHAT_ID = "5348160018";

X509List cert(TELEGRAM_CERTIFICATE_ROOT);

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 2;
bool ledState = LOW;
uint8_t connect_state = 0;

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
      String text = bot.messages[i].text;
      if (text != "/setup") {
        bot.sendMessage(chat_id, "Group is not registered.\nPlease send /setup to set a group to be an alarm group \n");
      }
      if (text == "/setup") {
        CHAT_ID = chat_id;
        saveChatID();
        String welcome = "The chat group has been set to an alarm group.\n\n";
        bot.sendMessage(chat_id, welcome);
      }
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
//    if (text == "Liên heo") {
//      String welcome = "Đúng vậy. Theo tôi được biết Liên chính là 🐷.\n\n";
//      bot.sendMessage(chat_id, welcome);
//    }
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 1);
  
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
    digitalWrite(ledPin, 0);
    connect_state = 1;
    Serial.print("Retrieving time:...... ");
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
    time_t now = time(nullptr);
    while (now < 24 * 3600)
    {
      delay(100);
      now = time(nullptr);
    }
    Serial.println(now);
    Serial.println("#ESP:Ready");
  }
}

void loop() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  while(numNewMessages) {
    Serial.println("got response");
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
  if (WiFi.status() != WL_CONNECTED) {
    //Serial.print("connect_state:");
    if(!connect_state){
      digitalWrite(ledPin, 1);
      connect_state = 1;
      Serial.println("#ESP:Lost_wifi");
    }
    //Serial.println(connect_state);
    if(connect_state==2){
       server.handleClient();
       //Serial.println("server.handleClient");
    }
    if(connect_state==1){
      Serial.print("Connecting to WiFi......");
      connect_state = 0;
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        unsigned long startTime = millis();
        //Serial.print("\nConnecting to WiFi......");
        while (WiFi.status() != WL_CONNECTED && millis() - startTime < 5000) {
          Serial.print(".");
          delay(100);
        }
        if(WiFi.status() == WL_CONNECTED){
            Serial.println("#ESP:Ready");
            digitalWrite(ledPin, 0);
            connect_state = 1;
        }
    }
  }
  handleSerialInput();
}

void setupAP() {
  connect_state=2;
  Serial.println("#ESP_AP_mode");
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
    CHAT_ID[i] = EEPROM.read(64 + i);
  }
}

void saveCredentials() {
  for (int i = 0; i < 32; ++i) {
    EEPROM.write(i, ssid[i]);
    EEPROM.write(32 + i, password[i]);
  }
  EEPROM.commit();
}
void saveChatID() {
  for (int i = 0; i < 32; ++i) {
    EEPROM.write(64 + i, CHAT_ID[i]);
  }
  EEPROM.commit();
}
void handleSerialInput() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Loại bỏ các ký tự xuống dòng và khoảng trắng
    if (command == "AT+ALARM=1") {
      Serial.println("Canh bao chay!!!Ve nha ngay!!!");
      bot.sendMessage(CHAT_ID, "🔔🔔🔔Cảnh báo cháy!!!🆘🆘🆘Gọi cứu hỏa ngay!!!");
    }
    if (command == "AT+ALARM=2") {
      Serial.println("Phat hien khi gas!!!");
      String gas = "🔔🔔🔔Cảnh báo phát hiện khí gas!!!\n\n";
      gas +="Quy trình xử lý an toàn:\n";
      gas +="Bước 1: Khóa van bình gas\n";
      gas +="Bước 2: Mở hết cửa sổ, cửa ra vào để khí gas thoát ra ngoài\n";
      gas +="- Mở hết các cửa sổ sau khi đã đóng van gas để cho khí gas thoát ra ngoài.\n";
      gas +="- Bạn có thể sử dụng bìa carton hoặc quạt tay để lùa khí gas ra môi trường.\n";
      gas +="- Lưu ý không bật quạt điện bởi dễ phát ra tia lửa điện gây cháy.\n";
      gas +="- Khi quạt, hãy quạt theo phương ngang để khí thoát ra ngoài một cách nhanh chóng.\n";
      gas +="- Không quạt theo phương thẳng đứng sẽ khiến khí gas bay cao và bạn dễ hít phải.\n";
      gas +="- Dùng khăn ướt để che mũi lại tránh hít phải khí gas và đưa mọi người trong gia đình nhanh chóng ra khỏi nhà.\n";
      gas +="Bước 3: Không bật/tắt các công tắc, thiết bị điện trong nhà\n";
      gas +="- Tuyệt đối không được bật/tắt các công tắc điện, dùng diêm hay bật lửa…..những thứ phát ra tia lửa điện dễ gây bắt lửa dễ dàng.\n";
      gas +="Bước 4: Bạn cần liên hệ ngay với các cơ sở gas uy tín để được kiểm tra, xử lý kịp thời\n";
      bot.sendMessage(CHAT_ID, gas);
    }
    else if(command == "AT+RST"){
      ESP.restart();
    }
    else {}
  }
}
