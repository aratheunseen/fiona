/*----------- Included Libraries ------------*/

#include <bits/stdc++.h>
#include <LiquidCrystal.h>
#include "CTBot.h"

using namespace std;

CTBot fiona;

/*------------ All about Connections ------------*/

// Set Your Wi-Fi SSID Name and Password
String SSID = "";
String PASSWORD = "";

// Secret Code (Telegram API Key)
String KEY = "";

// Set FionaID
char *FionaID0 = "";
char *FionaID1 = "";
char *FionaID2 = "";

/*------------ Global Variables and Functions ------------*/

String newNotice = "";

// Set 16x2 LCD Pin
const int rs = 14, en = 2, d4 = 0, d5 = 4, d6 = 5, d7 = 16;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int LED1 = 12;
int LED2 = 15;
int PIEZO = 13;

// Emergency Checker Function
int IsEmergency(String text) {
  String splitedText = text.substring(0, 10);

  for (int i = 0; i < splitedText.length(); i++) {
    splitedText[i] = tolower(splitedText[i]);
  }

  if (splitedText == "/emergency") {
    return 1;
  }
  return 0;
}

// Show notice on LCD Function
void showOnLCD(String txt) {

  lcd.setCursor(16, 0);
  lcd.print(txt);
  lcd.setCursor(0, 1);
  lcd.print("{fiona:} by Team Disconnection");
  for (int scrollCounter = 0; scrollCounter < txt.length(); scrollCounter++) {
    lcd.scrollDisplayLeft();
    delay(400);
  }
}

/*------------ Setup Section ------------*/
void setup() {
  // Start LCD
  lcd.begin(16, 2);

  // Declare LED Pins Mode
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(PIEZO, OUTPUT);

  Serial.begin(115200);

  // Connect with Internet
  fiona.wifiConnect(SSID, PASSWORD);

  // Connect with Telegram
  fiona.setTelegramToken(KEY);

  // Check the Internet Connection
  if (fiona.testConnection()) {
    // If Internet Connection is Ok
    digitalWrite(LED2, HIGH);
    delay(500);
    digitalWrite(LED1, HIGH);
    delay(500);
    digitalWrite(LED2, LOW);
    digitalWrite(LED1, LOW);
    lcd.clear();
    newNotice = "No new messages";
  }
  else{
    // If Internet Connection is not Ok
    digitalWrite(LED1, HIGH);
    delay(500);
    digitalWrite(LED2, HIGH);
    delay(5000);
    digitalWrite(LED2, LOW);
    digitalWrite(LED1, LOW);
    lcd.clear();
    newNotice = "No Internet Connection";
  }
}

/*------------ Loop Section ------------*/

void loop() {
  TBMessage notice;

  // Check Incoming Message
  if (CTBotMessageText == fiona.getNewMessage(notice)) {

    char *ID = (char *)notice.sender.id;

    if (notice.text.equalsIgnoreCase("/start")) {
      // Send welcome message to sender
      String welcome = (String) "Hello! Your FionaID is " + notice.sender.id + ". You can use this ID to make your system unique.\n\n After completing the setup, Type '/help' to learn more.\n\n Thanks.";
      fiona.sendMessage(notice.sender.id, welcome);
    }

    // Check ID
    if (strcmp(ID, FionaID0) || strcmp(ID, FionaID1) || strcmp(ID, FionaID2)) {
      String text = (String)notice.text;

      /*------------ Response & Message ------------*/

      if (notice.text.equalsIgnoreCase("/help")) {
        // Send help page to sender
        String help = (String) "/emergency text - Send notice with emergency priority.\n\text - Send notice without any priority.\n\nnotice_Text must be longer than 10 characters.\n\nTo learn more, visit https://github.com/aratheunseen/fiona/blob/main/DOCS.md";
        fiona.sendMessage(notice.sender.id, help);
      } else {
        if (text.length() < 10) {
          String report = (String) "Unable to publish. It is too short or an invalid notice.";
          fiona.sendMessage(notice.sender.id, report);
        } else if (IsEmergency(text) == 1 && text.length() < 12) {
          // Send notice as False Emergency
          String report = (String) "Invalid notice. Type '/emergency text' to send emergency notice.";
          fiona.sendMessage(notice.sender.id, report);
        }
        // Emergency Notice
        else if (IsEmergency(text) == 1 && text.length() > 11) {
          // LED
          digitalWrite(LED2, HIGH);
          digitalWrite(LED1, HIGH);

          // LCD
          String emergencyText = (String)notice.text;
          int last = emergencyText.length();
          lcd.clear();
          newNotice = emergencyText.substring(11, last);

          // Toggle Piezo for 5s
          digitalWrite(PIEZO, HIGH);
          delay(5000);
          digitalWrite(PIEZO, LOW);

          // Send notice as emergency report
          String report = (String) "Notice has been published as emergency.";
          fiona.sendMessage(notice.sender.id, report);
        }
        // General Notice
        else {
          // LED
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, LOW);

          // LCD
          lcd.clear();
          newNotice = (String)notice.text;

          // Send notice delivered report
          String report = (String) "Notice has been published.";
          fiona.sendMessage(notice.sender.id, report);
        }
      }
    }
  }
  showOnLCD(newNotice);
}