Hệ thống báo động cháy: Sử dụng cảm biến khói và cảm biến nhiệt độ 
để phát hiện sự cố cháy trong nhà và gửi cảnh báo đến người dùng qua Wi-Fi

ESP8266
. Variables and constants in RAM (global, static), used 31288 / 80192 bytes (39%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ DATA     1684     initialized variables
╠══ RODATA   3484     constants       
╚══ BSS      26120    zeroed variables
. Instruction RAM (IRAM_ATTR, ICACHE_RAM_ATTR), used 60795 / 65536 bytes (92%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ ICACHE   32768    reserved space for flash instruction cache
╚══ IRAM     28027    code in IRAM    
. Code in flash (default, ICACHE_FLASH_ATTR), used 368604 / 1048576 bytes (35%)
║   SEGMENT  BYTES    DESCRIPTION
╚══ IROM     368604   code in flash 

. Variables and constants in RAM (global, static), used 29356 / 80192 bytes (36%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ DATA     1576     initialized variables
╠══ RODATA   1916     constants       
╚══ BSS      25864    zeroed variables
. Instruction RAM (IRAM_ATTR, ICACHE_RAM_ATTR), used 60603 / 65536 bytes (92%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ ICACHE   32768    reserved space for flash instruction cache
╚══ IRAM     27835    code in IRAM    
. Code in flash (default, ICACHE_FLASH_ATTR), used 355752 / 1048576 bytes (33%)
║   SEGMENT  BYTES    DESCRIPTION
╚══ IROM     355752   code in flash 

You can use this token to access HTTP API:
5765659274:AAF1MrBZwC52uunp-t2CE6KhQn2yEmvKaik

const char* ssid = "Active 3_4769";
const char* password = "244466666";

#include <ESP8266WiFi.h>
#include  <WiFiClientSecure.h>
#include <TelegramBot.h>
#define LED 1 //led pin number
// Initialize Wifi connection to the router 
const char* ssid     = "Active 3_4769"; 
const char* password = "244466666"; 
// Initialize Telegram BOT 
const char BotToken[] = "5765659274:AAF1MrBZwC52uunp-t2CE6KhQn2yEmvKaik"; 
WiFiClientSecure net_ssl; 
TelegramBot bot (BotToken, net_ssl); 
// the number of the LED pin   
void setup()  
{   
	Serial.begin(115200);   
	while (!Serial) {}  //Start running when the serial is open  
	delay(3000);   
	// attempt to connect to Wifi network:   
	Serial.print("Connecting Wifi: ");   
	Serial.println(ssid);   
	while (WiFi.begin(ssid, password) != WL_CONNECTED)  
	{   
	  Serial.print(".");   
	  delay(500);   
	}   
	Serial.println("");   
	Serial.println("WiFi connected");   
	bot.begin();   
	pinMode(LED, OUTPUT);   
}   
void loop()  
{   
	message m = bot.getUpdates(); // Read new messages   
	if (m.text.equals("on"))  
		  {   
	  digitalWrite(LED, 1);    
	  bot.sendMessage(m.chat_id, "The Led is now ON");   
	}   
	else if (m.text.equals("off"))  
		  {   
	  digitalWrite(LED, 0);    
	  bot.sendMessage(m.chat_id, "The Led is now OFF");   
	}   
}   