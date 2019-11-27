
// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// pins used
//#define CLK 9       // SPI Clock, shared with SD card
//#define MISO 10      // Input data, from VS1053/SD card
//#define MOSI 8      // Output data, to VS1053/SD card

//breakout
#define BREAKOUT_RESET  5      // VS1053 reset pin (output)
#define BREAKOUT_CS     3     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    7      // VS1053 Data/command select pin (output)
#define CARDCS 6     // Card chip select pin
#define DREQ A1       // VS1053 Data request, ideally an Interrupt pin


//call out the musicPlayer
Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);

const int buttonPin = 1;      // pin for the pushbutton
int lastButtonState = HIGH;   // previous state of the pushbutton

const char soundFile[] = "track001.MP3";

const int ledPin1 =  13;// the number of the LED pinn
const int ledPin2 =  12;
const int ledPin3 =  11;

void setup() {
  Serial.begin(9600);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  while (!Serial) {
    ;
  }

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

  printDirectory(SD.open("/"), 0);

  // Set volume for left and right channels.
  // 0 = loudest, 100 = silent:
  musicPlayer.setVolume(10, 10);

  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  musicPlayer.startPlayingFile("/track001.mp3");
  Serial.println("playing");

}

void loop() {


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

  if ((loudness >= 33) && (loudness <= 66)) {
    digitalWrite(12, HIGH);
  } else if ( loudness > 66) {
    digitalWrite(12, LOW);
  }

  if ((loudness >= 66) && (loudness <= 100)) {
    digitalWrite(11, HIGH);
  } else if ( loudness > 100) {
    digitalWrite(11, LOW);
  }


  // File is playing in the background
  if (musicPlayer.stopped()) {
    Serial.println("Done playing music");
    while (1) {
      delay(10);  // we're done! do nothing...
    }
  }


  // read a pushbutton for play/pause:
  int buttonState = digitalRead(buttonPin);
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
