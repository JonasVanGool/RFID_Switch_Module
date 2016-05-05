/*
---------------------------------------------------------------
 * FLOW
---------------------------------------------------------------
*/

MAIN_CONTROLLER_STATE MainCurrentState = STARTUP;
MAIN_CONTROLLER_STATE MainPreviouseState = STARTUP;
char* tempBadge;
 
void FLOW_Setup(){
   #ifdef debug
    Serial.begin(9600);
   #endif
}

void FLOW_Loop(){
  switch(MainCurrentState){
    case STARTUP:
      LED_SetState(LED_STARTUP);
      BUZZER_StartSequence(BUZZER_STARTUP);
      FLOW_SetState(WAIT_FOR_SCAN_STARTUP);
      break;
    case WAIT_FOR_SCAN_STARTUP:
      if(RFID_BadgeAvailable()){
        tempBadge = RFID_ReadBadge();
        switch(EEPROM_GetBadgeType(tempBadge)){
          case MASTER_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_ADD);  
            RELAY_On();
            FLOW_SetState(WAIT_FOR_SCAN_ADD);
            break;
          case UNKNOWN_BADGE:
            FLOW_SetState(STEP_UNKNOWN_BADGE);
            break;
          case KNOWN_BADGE:
            if(!EEPROM_LogIn(tempBadge)){
              BUZZER_StartSequence(BUZZER_ERROR);
              LED_SetState(LED_ERROR);
              FLOW_SetState(ERROR_STATE);
            }
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
        tempBadge = RFID_ReadBadge();
        switch(EEPROM_GetBadgeType(tempBadge)){
          case MASTER_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_NORMAL);
            FLOW_SetState(WAIT_FOR_SCAN_NORMAL);    
            break;
          case UNKNOWN_BADGE:
            if(!EEPROM_LogOut(false)){
              BUZZER_StartSequence(BUZZER_ERROR);
              LED_SetState(LED_ERROR);
              FLOW_SetState(ERROR_STATE);
            }
            FLOW_SetState(STEP_UNKNOWN_BADGE);
            break;
          case KNOWN_BADGE:
            if(EEPROM_CompareBadges(EEPROM_GetLoggedInBadge(),tempBadge)){
              if(!EEPROM_LogOut(false)){
                BUZZER_StartSequence(BUZZER_ERROR);
                LED_SetState(LED_ERROR);
                FLOW_SetState(ERROR_STATE);
              }
              BUZZER_StartSequence(OK);
              LED_SetState(LED_OK,LED_STARTUP);
              RELAY_Off();
              FLOW_SetState(WAIT_FOR_SCAN_STARTUP);
              break;
            }else{
              if(!EEPROM_LogOut(true)){
                BUZZER_StartSequence(BUZZER_ERROR);
                LED_SetState(LED_ERROR);
                FLOW_SetState(ERROR_STATE);
              }
              if(!EEPROM_LogIn(tempBadge)){
                BUZZER_StartSequence(BUZZER_ERROR);
                LED_SetState(LED_ERROR);
                FLOW_SetState(ERROR_STATE);
              }
              BUZZER_StartSequence(OK);
              LED_SetState(LED_OK,LED_NORMAL);
              FLOW_SetState(WAIT_FOR_SCAN_NORMAL);
            break;
            }
          default:
            break;
        }    
      } 
      break;
    case WAIT_FOR_SCAN_ADD:
      if(RFID_BadgeAvailable()){
        tempBadge = RFID_ReadBadge();
        switch(EEPROM_GetBadgeType(tempBadge)){
          case MASTER_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_DELETE);
            FLOW_SetState(WAIT_FOR_SCAN_DELETE);    
            break;
          case UNKNOWN_BADGE:
            if(EEPROM_WriteBadgeToMemory(tempBadge)){
              BUZZER_StartSequence(OK);
              LED_SetState(LED_OK,LED_ADD);
              FLOW_SetState(WAIT_FOR_SCAN_ADD);
            }else{
              BUZZER_StartSequence(FAIL);
              LED_SetState(LED_FAIL,LED_ADD);
              FLOW_SetState(WAIT_FOR_SCAN_ADD);
            }
            break;
          case KNOWN_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_ADD);
            FLOW_SetState(WAIT_FOR_SCAN_ADD);
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
            LED_SetState(LED_OK,LED_TRANSMIT);
            TRANSMIT_On();
            FLOW_SetState(WAIT_FOR_SCAN_TRANSMIT);  
            break;
          case UNKNOWN_BADGE:
            BUZZER_StartSequence(FAIL);
            LED_SetState(LED_FAIL,LED_DELETE);
            FLOW_SetState(WAIT_FOR_SCAN_DELETE); 
            break;
          case KNOWN_BADGE:
            if(EEPROM_DeleteBadgeFromMemory(tempBadge)){
              BUZZER_StartSequence(OK);
              LED_SetState(LED_OK,LED_DELETE);
              FLOW_SetState(WAIT_FOR_SCAN_DELETE); 
            }else{
              BUZZER_StartSequence(FAIL);
              LED_SetState(LED_FAIL,LED_DELETE);
              FLOW_SetState(WAIT_FOR_SCAN_DELETE); 
            }
            break;
          default:
            break;
        }    
      } 
      break;
    case WAIT_FOR_SCAN_TRANSMIT:
      if(RFID_BadgeAvailable()){
        TRANSMIT_Off();
        char* tempBadge = RFID_ReadBadge();
        switch(EEPROM_GetBadgeType(tempBadge)){
          case MASTER_BADGE:
            FLOW_SetState(STEP_UNKNOWN_BADGE);
            break;
          case UNKNOWN_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_TRANSMIT);
            TRANSMIT_On();
            FLOW_SetState(WAIT_FOR_SCAN_TRANSMIT); 
            break;
          case KNOWN_BADGE:
            BUZZER_StartSequence(OK);
            LED_SetState(LED_OK,LED_TRANSMIT);
            TRANSMIT_On();
            FLOW_SetState(WAIT_FOR_SCAN_TRANSMIT); 
            break;
          default:
            break;
        }    
      } 
    case STEP_UNKNOWN_BADGE:
      BUZZER_StartSequence(FAIL);
      LED_SetState(LED_FAIL,LED_STARTUP);
      RELAY_Off();
      FLOW_SetState(WAIT_FOR_SCAN_STARTUP);
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
