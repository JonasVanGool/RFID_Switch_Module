/*
---------------------------------------------------------------
 * RFID
---------------------------------------------------------------
*/
#include <SoftwareSerial.h>
#define RFID_PIN 2
// RFID Reader variables
int readVal = 0; // individual character read from serial
unsigned int readData[10]; // data read from serial
int counter = -1; // counter to keep position in the buffer
char tagId[10]; // final tag ID converted to a string
SoftwareSerial mySerial(RFID_PIN, 11); // RX, TX
boolean badgeAvailable = false;
long lastReadTime = 0;

void RFID_Setup(){
  mySerial.begin(9600); 
}

void RFID_Loop(){
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

      // Set badge available
      badgeAvailable = true;
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

void RFID_StartListening(){
  mySerial.begin(9600); 
}

char* RFID_ReadBadge(){
  lastReadTime = millis();
  badgeAvailable = false;
  return tagId;
}

boolean RFID_BadgeAvailable(){
  if(millis()-lastReadTime < 1500)
    return false;
  return badgeAvailable;
}

//-----------------------
// this function clears the rest of data on the serial, to prevent multiple scans
//-----------------------
void clearSerial() {
  while (Serial.read() >= 0) {
    // do nothing
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
}
