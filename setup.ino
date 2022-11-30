/*----------- Included Libraries ------------*/

#include <bits/stdc++.h>
#include <LiquidCrystal.h>
#include "CTBot.h"

using namespace std;

/*------------ All about Connections ------------*/
int c = 0;
CTBot fiona;

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

// Set 16x2 LCD Pin
const int rs = 15, en = 13, d4 = 0, d5 = 4, d6 = 5, d7 = 16;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// D0=16 - LED1
// D1=5 - LED2
// D2=4 - Piezo

int LED1 = 2;
int LED2 = 14;
int PIEZO = 12;

// Emergency Checker Function
int IsEmergency(String text)
{
  String splitedText = text.substring(0, 10);

  for (int i = 0; i < splitedText.length(); i++)
  {
    splitedText[i] = tolower(splitedText[i]);
  }
  String emergency = "/emergency";

  if (splitedText == emergency)
  {
    return 1;
  }
  return 0;
}

/*------------ Setup Section ------------*/
void setup()
{
  Serial.begin(115200);

  // Connect with Internet
  fiona.wifiConnect(SSID, PASSWORD);

  // Connect with Telegram
  fiona.setTelegramToken(KEY);

  // Check the Internet Connection
  if (fiona.testConnection())
  {
    Serial.println("\nYou are awesome. Thanks for feeding me internet."); // dummy line
    // If Internet Connection is Ok then 'TURN ON' Built In LED
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
  }
  else
  {
    // If Internet Connection is not Ok
    Serial.println("\nOhho! No Internet. I am Hungry."); // dummy line
  }

  // Start LCD
  lcd.begin(16, 2);

  // Declate LED Pins Mode
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(PIEZO, OUTPUT);

  digitalWrite(LED1, LOW);
}

/*------------ Loop Section ------------*/

void loop()
{
  TBMessage notice;

  // Check Incoming Message
  if (CTBotMessageText == fiona.getNewMessage(notice))
  {
    char *ID = (char *)notice.sender.id;

    if (notice.text.equalsIgnoreCase("/start"))
    {
      // Send welcome message to sender
      String welcome = (String) "Hello! Your FionaID is " + notice.sender.id + ". You can use this ID to make your system unique.\n\n After completing the setup, Type '/help' to learn more.\n\n Thanks.";
      fiona.sendMessage(notice.sender.id, welcome);
    }

    if (strcmp(ID, FionaID0) || strcmp(ID, FionaID1) || strcmp(ID, FionaID2))
    {

      /*------------ LEDs, Piezo and LCD ------------*/

      // Get any message? then Turn ON LED1
      digitalWrite(LED1, HIGH);
      c = 1;

      if (c == 1)
      {

        lcd.print(notice.text);

        /*for (int j=0 ; j <= notice.text.length(); j++){
          lcd.setCursor(j,0);
          lcd.print(notice.text);
          delay(200);
          lcd.clear();
        }*/

        Serial.println(notice.text);
      }

      // 1 = notice with /emergency, then turn on second led
      String text = (String)notice.text;
      if ((c == 1) && IsEmergency(text))
      {
        // Turn ON Emergency LED
        digitalWrite(LED2, HIGH);
        // lcd.setCursor(0,0);
        String emergencyText = (String)notice.text;
        int last = emergencyText.length();
        String text1 = emergencyText.substring(11, last);
        lcd.setCursor(1, 0);
        lcd.print(text1);

        // Turn ON Piezo
        digitalWrite(PIEZO, HIGH); // on
      }

      /*------------ Response Message ------------*/

      if (notice.text.equalsIgnoreCase("/help"))
      {
        // Send help page to sender
        String help = (String) "/emergency text - Send notice with emergency priority.\n\text - Send notice without any priority.\n\nnotice_Text must be longer than 10 characters.";
        fiona.sendMessage(notice.sender.id, help);
      }
      else
      {
        if (text.length() < 10)
        {
          String report = (String) "Unable to publish. It is too short or an invalid notice.";
          fiona.sendMessage(notice.sender.id, report);
        }
        else if (IsEmergency(text) == 1 && text.length() < 12)
        {
          // Send notice as False Emergency
          String report = (String) "Invalid notice. Type '/emergency text' to send emergency notice.";
          fiona.sendMessage(notice.sender.id, report);
        }
        else if (IsEmergency(text) == 1 && text.length() > 11)
        {
          // Send notice as emergency report
          String report = (String) "Notice has been published as emergency.";
          fiona.sendMessage(notice.sender.id, report);
        }
        else
        {
          // Send notice delivered report
          String report = (String) "Notice has been published.";
          fiona.sendMessage(notice.sender.id, report);
        }
      }
    }
  }
}
