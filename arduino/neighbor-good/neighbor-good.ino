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

//const char serverAddress[] = "itp-good-neighbor.glitch.me";  // server address @ glitch
//const char serverAddress[] = "10.23.11.246";                 // server address @ nyu (localhost)
const char serverAddress[] = "192.168.1.244";                // server address @ home (localhost)
int port = 8080;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

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

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  // assemble the path for the GET message:
  String path = "/decibels/";

  // send the GET request
  Serial.println("making GET request");
  client.get(path);

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
  Serial.print("decibels:\t");
  Serial.println(myObject["decibels"]);
  Serial.print("patience:\t");
  Serial.println(myObject["patience"]);
  Serial.print("volume:\t\t");
  Serial.println(myObject["volume"]);
  Serial.print("track:\t\t");
  Serial.println(myObject["track"]);
  Serial.print("datetime:\t");
  Serial.println(myObject["datetime"]);

  // myObject.keys() can be used to get an array
  // of all the keys in the object
  JSONVar keys = myObject.keys();
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

  Serial.println("Wait ten seconds\n");
  delay(10000);
}
