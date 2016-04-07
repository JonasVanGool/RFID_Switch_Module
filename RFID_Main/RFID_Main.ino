#include "types.h"
//-----------------------
// Setup
//-----------------------
void setup() {
  FLOW_Setup();
  RFID_Setup();
  LED_Setup();
  EEPROM_Setup();
  BUZZER_Setup();
  RELAY_Setup();
}

//-----------------------
// Controller
//-----------------------
void loop() {
  RFID_Loop();
  LED_Loop();
  EEPROM_Loop();
  BUZZER_Loop();
  RELAY_Loop();
  FLOW_Loop();
}





