/*
  Attributions:

  - Tom Igoe's HttpClientGetJson.ino example code:
    https://github.com/tigoe/Wifi101_examples/tree/master/HttpClientGetJSON

*/
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <Arduino_JSON.h>
// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// pins used on the MKR series for mp3 player features not implemented
//#define CLK 9       // SPI Clock, shared with SD card
//#define MISO 10      // Input data, from VS1053/SD card
//#define MOSI 8      // Output data, to VS1053/SD card

//breakout
#define BREAKOUT_RESET  5      // VS1053 reset pin (output)
#define BREAKOUT_CS     3     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    7      // VS1053 Data/command select pin (output)
#define CARDCS 6     // Card chip select pin
#define DREQ A1       // VS1053 Data request, ideally an Interrupt pin

// TODO: configure DNS for use with Glitch
//IPAddress dns(8, 8, 8, 8);  // Google's primary DNS server

//const char serverAddress[] = "itp-good-neighbor.glitch.me";  // server address @ glitch
const char serverAddress[] = "10.23.11.246";                 // server address @ localhost on sandbox
//const char serverAddress[] = "192.168.1.244";                // server address @ localhost on flame
int port = 8080;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

//instanciate a musicPlayer object
Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);

// Declare variables to store values from the server
int myPlayState;
int myDecibels;
int myWarning;
int myVolume;
int myTrack;
int myDatetime;

// Declare variables for the mp3 musicPlayer
const int buttonPin = 1;      // pin for the pushbutton
int lastButtonState = HIGH;   // previous state of the pushbutton

const char soundFile[] = "track001.MP3";

const int ledPin1 =  13; // volume indicater LED pin - low
const int ledPin2 =  12; // volume indicater LED pin -medium
const int ledPin3 =  11; // volume indicater LED pin - high


void setup() {
  Serial.begin(9600);
  while (!Serial);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);     // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi device's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // TODO: configure DNS for use with Glitch
  //  // print your DNS server address:
  //  IPAddress dns = WiFi.setDNS(8.8.8.8);
  //  Serial.print("Dns configured: ");
  //  Serial.println(dns);

  /*
    mp3 stuff
  */

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(BREAKOUT_RESET, OUTPUT);
  digitalWrite(BREAKOUT_RESET, LOW);
  delay(10);
  digitalWrite(BREAKOUT_RESET, HIGH);

  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // call function to read directory contents on the sd card
  printDirectory(SD.open("/"), 0);

  // Set volume for left and right channels.
  // 0 = loudest, 100 = silent:
  musicPlayer.setVolume(10, 10);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
  musicPlayer.startPlayingFile("/track001.mp3");
  Serial.println("playing");

}


void loop() {

  /*
    mp3 player stuff
  */

  // read a potentiometer (0-1023 readings) and
  // map to a range from 100 to 0:
  int loudness = map(analogRead(A0), 0, 1023, 100, 0);
  // set the volume:
  musicPlayer.setVolume(loudness, loudness);

  if ((loudness >= 0) && (loudness <= 33)) {
    digitalWrite(13, HIGH);
  } else if ( loudness > 33) {
    digitalWrite(13, LOW);
  }

  if ((loudness >= 34) && (loudness <= 66)) {
    digitalWrite(12, HIGH);
  } else if ( loudness > 66) {
    digitalWrite(12, LOW);
  }

  if ((loudness >= 67) && (loudness <= 100)) {
    digitalWrite(11, HIGH);
  } else if ( loudness > 100) {
    digitalWrite(11, LOW);
  }

  // Check if file is playing in the background
  if (musicPlayer.stopped()) {
    Serial.println("Done playing music");
    while (1) {
      delay(10);  // we're done! do nothing...
    }
  }

  // read a pushbutton for play/pause:
  int buttonState = digitalRead(buttonPin);
  Serial.print("buttonState\t");
  Serial.println(buttonState);
  // if the button has changed:
  if (buttonState != lastButtonState) {
    // if the button is low:
    if (buttonState == LOW) {
      // switch play/pause state:
      if (musicPlayer.paused()) {
        musicPlayer.pausePlaying(false);
      } else {
        musicPlayer.pausePlaying(true);
      }
    }
  }
  // save current button state for comparison next time:
  lastButtonState = buttonState;

  /*
     Server-client stuff
  */

  // assemble the path for the GET message:
  String pathState = "/playState";
  // String pathDecibels = "/decibels";
  // String pathWarning = "/warning";
  // String pathVolume = "/volume";
  // String pathTrack = "/track";
  // String pathDatetime = "/datetime";

  // send the GET request
  Serial.println("→ making GET request");
  client.get(pathState);
  // client.get(pathDecibels);
  // client.get(pathWarning);
  // client.get(pathVolume);
  // client.get(pathTrack);
  // client.get(pathDatetime);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  // parse the string into a JSONVar object:
  JSONVar myObject = JSON.parse(response);

  // if you know the object's keys, you can ask for their values:
  //  Serial.print("playState:\t");
  //  Serial.println(myObject["playState"]);
  //  Serial.print("decibels:\t");
  //  Serial.println(myObject["decibels"]);
  //  Serial.print("warning:\t");
  //  Serial.println(myObject["warning"]);
  //  Serial.print("volume:\t\t");
  //  Serial.println(myObject["volume"]);
  //  Serial.print("track:\t\t");
  //  Serial.println(myObject["track"]);
  //  Serial.print("datetime:\t");
  //  Serial.println(myObject["datetime"]);

  // myObject.keys() can be used to get an array
  // of all the keys in the object
  JSONVar keys = myObject.keys();
  //  Serial.println();
  //  Serial.println("→ reading key values from the JSON response");
  // read over the object, one key at a time:
  for (int i = 0; i < keys.length(); i++) {
    JSONVar value = myObject[keys[i]];
    // what's the key:
    //    Serial.print(keys[i]);
    //    Serial.print("\t");
    // and what's the value of the key:
    //    Serial.print(value);
    // and what's the type of the value:
    //    Serial.print("\ttype: ");
    //    Serial.println(JSON.typeof(value));
  }

  // assign values got from the server to corresponding variables on client
  //  Serial.println();
  //  Serial.println("→ assigning server values to variables on client");

  if (JSON.typeof(myObject["playState"]) == "string") {
    // convert to int
    String temp = JSON.stringify(myObject["playState"]);
    //    Serial.println(temp);
    temp = temp.substring(1, -1);
    myPlayState = temp.toInt();
    //    Serial.println("it's a string!");
  }
  if (JSON.typeof(myObject["playState"]) == "number") {
    // it's already an int
    myPlayState = myObject["playState"];
    //    Serial.println("it's a number!");
  }
  //  Serial.print("myPlayState:\t");
  //  Serial.println(myPlayState);

  if (JSON.typeof(myObject["decibels"]) == "string") {
    // convert to int
    String temp = JSON.stringify(myObject["decibels"]);
    temp = temp.substring(1, -1);
    myDecibels = temp.toInt();
  }
  if (JSON.typeof(myObject["decibels"]) == "number") {
    // it's already an int
    myDecibels = myObject["decibels"];
  }
  //  Serial.print("myDecibels:\t");
  //  Serial.println(myDecibels);

  if (JSON.typeof(myObject["warning"]) == "string") {
    // convert to int
    String temp = JSON.stringify(myObject["warning"]);
    temp = temp.substring(1, -1);
    myWarning = temp.toInt();
  }
  if (JSON.typeof(myObject["warning"]) == "number") {
    // it's already an int
    myWarning = myObject["warning"];
  }
  //  Serial.print("myWarning:\t");
  //  Serial.println(myWarning);

  if (JSON.typeof(myObject["volume"]) == "string") {
    // convert to int
    String temp = JSON.stringify(myObject["volume"]);
    temp = temp.substring(1, -1);
    myVolume = temp.toInt();
  }
  if (JSON.typeof(myObject["volume"]) == "number") {
    // it's already an int
    myVolume = myObject["volume"];
  }
  //  Serial.print("myVolume:\t");
  //  Serial.println(myVolume);

  if (JSON.typeof(myObject["track"]) == "string") {
    // convert to int
    String temp = JSON.stringify(myObject["track"]);
    temp = temp.substring(1, -1);
    myTrack = temp.toInt();
  }
  if (JSON.typeof(myObject["track"]) == "number") {
    // it's already an int
    myTrack = myObject["track"];
  }
  //  Serial.print("myTrack:\t");
  //  Serial.println(myTrack);

  myDatetime = myObject["datetime"];
  //  Serial.print("myDatetime:\t");
  //  Serial.println(myDatetime);

  Serial.println("Wait one second\n");
  delay(1000);

}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      //Serial.println("**nomorefiles**");
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
