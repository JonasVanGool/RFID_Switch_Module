//-----------------------
// Setup
//-----------------------
void setup() {
  RFID_Setup();
  LED_Setup();
  EEPROM_Setup();
  BUZZER_Setup();
  RELAY_Setup();
  FLOW_Setup();
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





