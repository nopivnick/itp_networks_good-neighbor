/*
  Attributions:

  - Tom Igoe's HttpClientGetJson.ino example code:
    https://github.com/tigoe/Wifi101_examples/tree/master/HttpClientGetJSON

*/
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <Arduino_JSON.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// TODO: configure DNS for use with Glitch
//IPAddress dns(8, 8, 8, 8);  // Google's primary DNS server

//const char serverAddress[] = "itp-good-neighbor.glitch.me";  // server address @ glitch
const char serverAddress[] = "10.23.11.246";                 // server address @ localhost on sandbox
//const char serverAddress[] = "192.168.1.244";                // server address @ localhost on flame
int port = 8080;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

// Declare variables to store values from the server
int myPlayState;
int myDecibels;
int myWarning;
int myVolume;
int myTrack;
int myDatetime;

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
}

void loop() {
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
  Serial.print("playState:\t");
  Serial.println(myObject["playState"]);
  Serial.print("decibels:\t");
  Serial.println(myObject["decibels"]);
  Serial.print("warning:\t");
  Serial.println(myObject["warning"]);
  Serial.print("volume:\t\t");
  Serial.println(myObject["volume"]);
  Serial.print("track:\t\t");
  Serial.println(myObject["track"]);
  Serial.print("datetime:\t");
  Serial.println(myObject["datetime"]);

  // myObject.keys() can be used to get an array
  // of all the keys in the object
  JSONVar keys = myObject.keys();
  Serial.println();
  Serial.println("→ reading key values from the JSON response");
  // read over the object, one key at a time:
  for (int i = 0; i < keys.length(); i++) {
    JSONVar value = myObject[keys[i]];
    // what's the key:
    Serial.print(keys[i]);
    Serial.print("\t");
    // and what's the value of the key:
    Serial.print(value);
    // and what's the type of the value:
    Serial.print("\ttype: ");
    Serial.println(JSON.typeof(value));
  }

  // assign values got from the server to corresponding variables on client
  Serial.println();
  Serial.println("→ assigning server values to variables on client");

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
  Serial.print("myPlayState:\t");
  Serial.println(myPlayState);

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
  Serial.print("myDecibels:\t");
  Serial.println(myDecibels);

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
  Serial.print("myWarning:\t");
  Serial.println(myWarning);

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
  Serial.print("myVolume:\t");
  Serial.println(myVolume);

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
  Serial.print("myTrack:\t");
  Serial.println(myTrack);

  myDatetime = myObject["datetime"];
  Serial.print("myDatetime:\t");
  Serial.println(myDatetime);

  Serial.println("Wait ten seconds\n");
  delay(10000);
}
