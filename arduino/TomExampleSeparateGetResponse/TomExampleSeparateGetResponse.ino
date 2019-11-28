/*
  Simple GET client for ArduinoHttpClient library
  Connects to server once every five seconds, sends a GET request

  created 14 Feb 2016
  modified 22 Jan 2019
  by Tom Igoe

  this example is in the public domain
*/
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <Arduino_JSON.h>

#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char serverAddress[] = "10.23.11.246";  // server address
int port = 8080;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

long lastRequest = 0;
long interval = 5000;

int statusCode = -1;

boolean haveStatusCode = false;

// Declare variables to store values from the server
int myPlayState;

void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

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

  if (millis() - lastRequest > interval) {
    Serial.println("making a request");
    // assemble the path for the GET message:
    String pathState = "/playState";
    client.get(pathState);
    lastRequest = millis();

    // read the status code and body of the response
    statusCode = client.responseStatusCode();
    haveStatusCode = true;
    Serial.print("Status code:\t");
    Serial.println(statusCode);
  }

  if (statusCode >= 0 && haveStatusCode == true) { // basically if the server sends any valid http response code
    Serial.println("Send up the white smoke! We have a response!");
    String response = client.responseBody();
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
    // parse the string into a JSONVar object:
    JSONVar myObject = JSON.parse(response);
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
    haveStatusCode = false;
  }

  //  Serial.println("Wait five seconds");
  //  delay(5000);
}
