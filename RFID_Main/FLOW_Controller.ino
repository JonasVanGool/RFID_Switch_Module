/*
---------------------------------------------------------------
 * FLOW
---------------------------------------------------------------
*/

MAIN_CONTROLLER_STATE MainCurrentState = STARTUP;
MAIN_CONTROLLER_STATE MainPreviouseState = STARTUP;

void FLOW_Setup(){
   #ifdef debug
    Serial.begin(9600);
   #endif
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
      LED_SetState(LED_STARTUP);
      BUZZER_StartSequence(BUZZER_STARTUP);
      FLOW_SetState(WAIT_FOR_SCAN_STARTUP);
      break;
    case WAIT_FOR_SCAN_STARTUP:
      if(RFID_BadgeAvailable()){
        switch(EEPROM_GetBadgeType(RFID_ReadBadge())){
          case MASTER_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_NORMAL);
            RELAY_On();
            FLOW_SetState(WAIT_FOR_SCAN_NORMAL);
          break;
          case UNKNOWN_BADGE:
            RELAY_Off();
            LED_SetState(LED_FAIL,LED_STARTUP);
            BUZZER_StartSequence(FAIL);
            FLOW_SetState(WAIT_FOR_SCAN_STARTUP);
          break;
          case KNOWN_BADGE:
            RELAY_On();
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_NORMAL);
            FLOW_SetState(WAIT_FOR_SCAN_NORMAL);
          break;
          default:
          break;
        }    
      } 
      break;
    case WAIT_FOR_SCAN_NORMAL:
      if(RFID_BadgeAvailable()){
        switch(EEPROM_GetBadgeType(RFID_ReadBadge())){
          case MASTER_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_ADD);
            FLOW_SetState(WAIT_FOR_SCAN_ADD);    
          break;
          case UNKNOWN_BADGE:
            RELAY_Off();
            BUZZER_StartSequence(FAIL);
            LED_SetState(LED_FAIL,LED_STARTUP);
            FLOW_SetState(WAIT_FOR_SCAN_STARTUP);
          break;
          case KNOWN_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_NORMAL);
            FLOW_SetState(WAIT_FOR_SCAN_NORMAL);
          break;
          default:
          break;
        }    
      } 
      break;
    case WAIT_FOR_SCAN_ADD:
      if(RFID_BadgeAvailable()){
        char* tempBadge = RFID_ReadBadge();
        switch(EEPROM_GetBadgeType(tempBadge)){
          case MASTER_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_DELETE);
            FLOW_SetState(WAIT_FOR_SCAN_DELETE);    
          break;
          case UNKNOWN_BADGE:
            if(EEPROM_WriteBadgeToMemory(tempBadge)){
              BUZZER_StartSequence(OK);
              LED_SetState(LED_OK,LED_NORMAL);
              FLOW_SetState(WAIT_FOR_SCAN_NORMAL);
            }else{
              BUZZER_StartSequence(FAIL);
              LED_SetState(LED_FAIL,LED_NORMAL);
              FLOW_SetState(WAIT_FOR_SCAN_NORMAL);
            }
          break;
          case KNOWN_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_NORMAL);
            FLOW_SetState(WAIT_FOR_SCAN_NORMAL);
          break;
          default:
          break;
        }    
      } 
      break;    
    case WAIT_FOR_SCAN_DELETE:
      if(RFID_BadgeAvailable()){
        char* tempBadge = RFID_ReadBadge();
        switch(EEPROM_GetBadgeType(tempBadge)){
          case MASTER_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_NORMAL);
            FLOW_SetState(WAIT_FOR_SCAN_NORMAL);  
          break;
          case UNKNOWN_BADGE:
            BUZZER_StartSequence(FAIL);
            LED_SetState(LED_FAIL,LED_NORMAL);
            FLOW_SetState(WAIT_FOR_SCAN_NORMAL);
          break;
          case KNOWN_BADGE:
            if(EEPROM_DeleteBadgeFromMemory(tempBadge)){
              BUZZER_StartSequence(OK);
              LED_SetState(LED_OK,LED_NORMAL);
              FLOW_SetState(WAIT_FOR_SCAN_NORMAL);
            }else{
              BUZZER_StartSequence(FAIL);
              LED_SetState(LED_FAIL,LED_NORMAL);
              FLOW_SetState(WAIT_FOR_SCAN_NORMAL);
            }
          break;
          default:
          break;
        }    
      } 
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
  #ifdef debug
  Serial.print(MainPreviouseState);
  Serial.print("-->");
  Serial.println(MainCurrentState);
  #endif
}
