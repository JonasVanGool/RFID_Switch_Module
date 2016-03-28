// Include libraries
#include "FastLED.h"
#include <SoftwareSerial.h>

// Pin configuration
#define LED_PIN 4
#define BUZZER_PIN 6
#define RELAY_PIN 5
#define RFID_PIN 3

// Fast led defines
#define BRIGHTNESS  100
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[1];

// Main controller states
enum MAIN_CONTROLLER_STATE{
  STARTUP,
  WAIT_FOR_SCAN_STARTUP,
  WAIT_FOR_SCAN_NORMAL,
  WAIT_FOR_SCAN_ADD,
  WAIT_FOR_SCAN_DELETE,
  WAIT_FOR_SCAN 
};
MAIN_CONTROLLER_STATE MainCurrentState = STARTUP;
MAIN_CONTROLLER_STATE MainPreviouseState = STARTUP;

// Led controller states
enum LED_CONTROLLER_STATE{
  LED_INIT,
  LED_NORMAL,
  LED_ADD,
  LED_DELETE
};
LED_CONTROLLER_STATE LedCurrentState = LED_INIT;
LED_CONTROLLER_STATE LedPreviouseState = LED_INIT;

// RFID Reader variables
int readVal = 0; // individual character read from serial
unsigned int readData[10]; // data read from serial
int counter = -1; // counter to keep position in the buffer
char tagId[11]; // final tag ID converted to a string
SoftwareSerial mySerial(RFID_PIN, 11); // RX, TX


//-----------------------
// Setup
//-----------------------
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

//-----------------------
// Controller
//-----------------------
void loop() {
  // Run rfid controller
  rfidController();
  // Run led controller
  ledController();
  // Run main controller
  mainController();
}

//-----------------------
// MAIN controller
//-----------------------
void mainController(){
  
}

//-----------------------
// LED controller
//-----------------------
void ledController(){
  
}

//-----------------------
// RFID controller
//-----------------------
void rfidController(){
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

//-----------------------
// Test sequence to test all IO
//-----------------------
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


//-----------------------
// this function clears the rest of data on the serial, to prevent multiple scans
//-----------------------
void clearSerial() {
  while (Serial.read() >= 0) {
    ; // do nothing
  }
}

//-----------------------
//Prints the readed tag to the main serial output
//-----------------------
void printTag() {
  Serial.print("Tag value: ");
  Serial.println(tagId);
}

//-----------------------
//Conver raw readed data to normal data
//-----------------------
void parseTag() {
  int i;
  for (i = 0; i < 10; ++i) {
    tagId[i] = readData[i];
  }
  tagId[10] = 0;
}
