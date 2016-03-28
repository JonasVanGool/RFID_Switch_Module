#include "FastLED.h"
#include <SoftwareSerial.h>

#define LED_PIN 4
#define BUZZER_PIN 6
#define RELAY_PIN 5
#define RFID_PIN 3

#define BRIGHTNESS  100
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[1];


// variables to keep state
int readVal = 0; // individual character read from serial
unsigned int readData[10]; // data read from serial
int counter = -1; // counter to keep position in the buffer
char tagId[11]; // final tag ID converted to a string
SoftwareSerial mySerial(RFID_PIN, 11); // RX, TX



void setup() {
  // Init sotware serial on RFID pin
  Serial.begin(9600);
  mySerial.begin(9600);
  
  // Set pin mode relay pin
  pinMode(RELAY_PIN,OUTPUT);
  digitalWrite(RELAY_PIN,LOW);
  
  // Init fastled library
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, 1).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  
  // Run IO test sequnece
  //IOTestSequence();
}

void loop() {
  
  if (mySerial.available() > 0) {
    // read the incoming byte:
    readVal = mySerial.read();
    
    // a "2" signals the beginning of a tag
    if (readVal == 2) {
      counter = 0; // start reading
    } 
    // a "3" signals the end of a tag
    else if (readVal == 3) {
      // Parse tag
      parseTag();
      
      // Print tag
      printTag();
      
      // clear serial to prevent multiple reads
      clearSerial();
      
      // reset reading state
      counter = -1;
    }
    // if we are in the middle of reading a tag
    else if (counter >= 0) {
      // save valuee
      readData[counter] = readVal;
      
      // increment counter
      ++counter;
    } 
  }
 
}

void IOTestSequence(){
  for(int i=0;i<255;i++){
    digitalWrite(RELAY_PIN,i%70);
    leds[0] = CHSV( i, 255, 255);
    FastLED.show();
    tone(BUZZER_PIN, i*8);
    delay(10);
  }
  noTone(BUZZER_PIN);
  leds[0] = CHSV( 0, 0, 0);
  FastLED.show();
}

// this function clears the rest of data on the serial, to prevent multiple scans
void clearSerial() {
  while (Serial.read() >= 0) {
    ; // do nothing
  }
}

void printTag() {
  Serial.print("Tag value: ");
  Serial.println(tagId);
}

void parseTag() {
  int i;
  for (i = 0; i < 10; ++i) {
    tagId[i] = readData[i];
  }
  tagId[10] = 0;
}
