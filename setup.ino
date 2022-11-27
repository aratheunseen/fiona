//#include <LiquidCrystal.h>
#include "CTBot.h"

// Our Bot (Kitty) is Fiona.
CTBot fiona;

// Set Your Wi-Fi SSID Name and Password
String SSID = "";
String PASSWORD = "";

// Secret Code
String KEY = "";

// Set FionaID
char* FionaID = "";

// Set LCD Pin
/*const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);*/


void setup() {
	Serial.begin(115200);
	Serial.println("Starting Fiona...");

  //lcd.begin(16,2);

  // Connect with Internet
	fiona.wifiConnect(SSID, PASSWORD);

  // Connect with Telegram
	fiona.setTelegramToken(KEY);

	// Check the Internet Connection
	if (fiona.testConnection()){
		Serial.println("\nYou are awesome.");
    // If Internet Connection is Ok then 'TURN ON' Built In LED
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
  }
	else{
		Serial.println("\nSorry, No Internet.");
  }
}

void loop() {
	TBMessage notice;

	// Check Incoming Message
	if (CTBotMessageText == fiona.getNewMessage(notice)) {
    char* ID = (char*) notice.sender.id;
    if(std::strcmp(ID,FionaID)){
      Serial.println(notice.text); // dummy line

      digitalWrite(D1, LOW);
      if(D1==LOW){
        //lcd.setCursor(0,0);
        //lcd.print(notice.text);
        Serial.println(notice.text); // dummy line
      }

      // 1 = notice.text contains emergency word turn on second led
      if(D1==LOW && 1){
        digitalWrite(D2,LOW);
      }

      if (notice.text.equalsIgnoreCase("/start")) {
        // do nothing boss
      }

      if (notice.text.equalsIgnoreCase("hi fiona")) {
        //Send FionaID to sender
        String id = (String) "Hello, Your FionaID is " + notice.sender.id + ". You can use this ID to make your system unique. Thanks senorita <3.";
        fiona.sendMessage(notice.sender.id,id);
      }
      else {
        // Send Roll Out Text
        String reply = (String)"Notice has been published.";
        fiona.sendMessage(notice.sender.id, reply);
      }
    }
	}
}
