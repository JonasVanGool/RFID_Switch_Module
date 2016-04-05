/*
---------------------------------------------------------------
 * EEPROM
---------------------------------------------------------------
*/
#include <EEPROM.h>

#define MAX_NR_BADGES 10
#define BADGE_LENGTH 10
char m_MasterBadge[BADGE_LENGTH] = {0,0,0,0,0,0,0,0,0,0};
char m_StoredBadges[MAX_NR_BADGES][BADGE_LENGTH];
 
void EEPROM_Setup(){
  EEPROM_ReadBadgesFromMemory();  
}

void EEPROM_Loop(){
  
}

boolean EEPROM_ReadBadgesFromMemory(){
  EEPROM.get(0,m_StoredBadges);
}

