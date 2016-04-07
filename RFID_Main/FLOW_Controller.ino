/*
---------------------------------------------------------------
 * FLOW
---------------------------------------------------------------
*/

MAIN_CONTROLLER_STATE MainCurrentState = STARTUP;
MAIN_CONTROLLER_STATE MainPreviouseState = STARTUP;

void FLOW_Setup(){
    Serial.begin(9600);
}

void FLOW_Loop(){
  switch(MainCurrentState){
    case STARTUP:
      if(!EEPROM_ReadBadgesFromMemory()){
        LED_SetState(LED_ERROR);
        BUZZER_StartSequence(BUZZER_ERROR);
        FLOW_SetState(ERROR_STATE);
        break;
      }
      LED_SetState(LED_INIT);
      BUZZER_StartSequence(BUZZER_STARTUP);
      FLOW_SetState(WAIT_FOR_SCAN_STARTUP);
      break;
    case WAIT_FOR_SCAN_STARTUP:
      if(RFID_BadgeAvailable()){
        switch(EEPROM_GetBadgeType(RFID_ReadBadge())){
          case MASTER_BADGE:
          break;
          case UNKNOWN_BADGE:
            LED_SetState(LED_ADD);
            BUZZER_StartSequence(FAIL);
            FLOW_SetState(STARTUP);
          break;
          case KNOWN_BADGE:
          break;
          default:
          break;
        }    
      } 
      break;
    case WAIT_FOR_SCAN_NORMAL:
      break;
    case WAIT_FOR_SCAN_ADD:
      break;
    case WAIT_FOR_SCAN_DELETE:
      break;
    case ERROR_STATE:
      break;
    default:
      break;
  }
}

void FLOW_SetState(MAIN_CONTROLLER_STATE newState){
  MainPreviouseState = MainCurrentState;
  MainCurrentState = newState;
  Serial.print(MainPreviouseState);
  Serial.print("-->");
  Serial.println(MainCurrentState);
}
