/*----------- Included Libraries ------------*/

#include <bits/stdc++.h>
#include <LiquidCrystal.h>
#include "CTBot.h"

using namespace std;


/*------------ All about Connections ------------*/

CTBot fiona;

// Set Your Wi-Fi SSID Name and Password
String SSID = "";
String PASSWORD = "";

// Secret Code (Telegram API Key)
String KEY = "";

// Set FionaID
char* FionaID0 = "***REMOVED***";
char* FionaID1 = "***REMOVED***";
char* FionaID2 = "";


/*------------ Global Variables and Functions ------------*/

// Set 16x2 LCD Pin
const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Emergency Checker Function
int IsEmergency(String text) {
  String splitedText = text.substring(0, 10);

  for (int i = 0; i < splitedText.length(); i++) {
    splitedText[i] = tolower(splitedText[i]);
  }
  String emergency = "/emergency";
  Serial.println(splitedText);  // dummy line

  if (splitedText == emergency) {
    return 1;
  }
  return 0;
}


/*------------ Setup Section ------------*/
void setup() {
  Serial.begin(115200);

  // Connect with Internet
  fiona.wifiConnect(SSID, PASSWORD);

  // Connect with Telegram
  fiona.setTelegramToken(KEY);

  // Check the Internet Connection
  if (fiona.testConnection()) {
    Serial.println("\nYou are awesome. Thanks for feeding me internet.");  // dummy line
    // If Internet Connection is Ok then 'TURN ON' Built In LED
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
  } else {
    // If Internet Connection is not Ok
    Serial.println("\nOhho! No Internet. I am Hungry.");  // dummy line
  }

  // Start LCD
  lcd.begin(16, 2);
}


/*------------ Loop Section ------------*/

void loop() {
  TBMessage notice;

  // Check Incoming Message
  if (CTBotMessageText == fiona.getNewMessage(notice)) {
    char* ID = (char*)notice.sender.id;
    if (strcmp(ID, FionaID0) || strcmp(ID, FionaID1) || strcmp(ID, FionaID2)) {

    /*------------ LEDs, Piezo and LCD ------------*/

      // D1,D2 - LEDs
      // D3 - Piezo
      // 

      digitalWrite(D1, LOW);
      if (D1 == LOW) {
        lcd.setCursor(0,0);
        lcd.print(notice.text);
        Serial.println(notice.text);  // dummy line
      }

      // 1 = notice with /emergency, then turn on second led
      if (D1 == LOW && 1) {
        // Turn ON Emergency LED
        digitalWrite(D2, LOW);
        lcd.setCursor(0,0);
        String emergencyText = (String) notice.text;
        int last = emergencyText.length();
        String text = emergencyText.substring(11, last);
        lcd.print(text);

        // Turn ON Piezo
        digitalWrite(D3, LOW); //on
        delay(5000); // 5s
        digitalWrite(D3, HIGH); //off
      }

      /*------------ Response Message ------------*/

      String text = (String)notice.text;

      if (notice.text.equalsIgnoreCase("/start")) {
        // Send welcome message to sender
        String welcome = (String) "Hello! Your FionaID is " + notice.sender.id + ". You can use this ID to make your system unique.\n\n Type '/help' to learn more.\n\n Thanks.";
        fiona.sendMessage(notice.sender.id, welcome);
      } else if (notice.text.equalsIgnoreCase("/help")) {
        // Send help page to sender
        String help = (String) "/emergency text - Send notice with emergency priority.\n\text - Send notice without any priority.\n\nnotice_Text must be longer than 10 characters.";
        fiona.sendMessage(notice.sender.id, help);
      } else {
        if (text.length() < 10) {
          String report = (String) "Unable to publish. It is too short or an invalid notice.";
          fiona.sendMessage(notice.sender.id, report);
        } else if (IsEmergency(text) == 1 && text.length() < 12) {
          // Send notice as False Emergency
          String report = (String) "Invalid notice. Type '/emergency text' to send emergency notice.";
          fiona.sendMessage(notice.sender.id, report);
        } else if (IsEmergency(text) == 1 && text.length() > 11) {
          // Send notice as emergency report
          String report = (String) "Notice has been published as emergency.";
          fiona.sendMessage(notice.sender.id, report);
        } else {
          // Send notice delivered report
          String report = (String) "Notice has been published.";
          fiona.sendMessage(notice.sender.id, report);
        }
      }
    }
  }
}
