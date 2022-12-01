# Fiona: User Manual
Documentation for using the Fiona Smart Assistant. To test this bot visit ![FionaTheBot](https://t.me/FionaTheBot).

###  Table of Contents
- [Setup](#setup)
- [Command](#command)


## Setup <a name="setup"/>
Get the ![setup code](https://github.com/aratheunseen/fiona/blob/main/setup.ino) and change this part as your connection.

    CTBot fiona;

    // Set Your Wi-Fi SSID Name and Password
    String SSID = "";
    String PASSWORD = "";

    // Secret Code (Telegram API Key)
    String KEY = "";

    // Set FionaID
    char *FionaID1 = "";
    char *FionaID2 = "";
    char *FionaID3 = "";

## Command <a name="command"/>
![/start]() - Start the bot and get FionaID.<br>
![/help]() - To know the syntex of message.<br>
![/emergency]() - To Sent notice as emergency priority.<br>
![any_text]() - To Sent notice without any priority.
