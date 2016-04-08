/*
---------------------------------------------------------------
 * EEPROM
---------------------------------------------------------------
*/
#include <EEPROM.h>



#define MAX_NR_BADGES 10
#define BADGE_LENGTH 11
char m_MasterBadge[BADGE_LENGTH] = {48,48,48,48,54,70,55,56,56,65,'\0'};
char m_EmptyBadge[BADGE_LENGTH] = {0,0,0,0,0,0,0,0,0,0,'\0'};
char m_StoredBadges[MAX_NR_BADGES][BADGE_LENGTH];
char testBadge[BADGE_LENGTH] = {1,2,3,4,5,6,7,8,9,10,'\0'};

void EEPROM_Setup(){

}

void EEPROM_Loop(){
}

boolean EEPROM_DeleteAllBadges(){
  for (int i = 0; i< MAX_NR_BADGES; i++){
    strncpy(m_StoredBadges[i], m_EmptyBadge, BADGE_LENGTH);   
  }
  EEPROM.put(0,m_StoredBadges);
}
boolean EEPROM_DeleteBadgeFromMemory(char* badge){
  for (int i = 0; i< MAX_NR_BADGES; i++){
    if(EEPROM_CompareBadges(badge, m_StoredBadges[i])){
       strncpy(m_StoredBadges[i], m_EmptyBadge, BADGE_LENGTH);
       EEPROM.put(0,m_StoredBadges);
       return true;
    }    
  }
  return false;
}

boolean EEPROM_ReadBadgesFromMemory(){
  EEPROM.get(0,m_StoredBadges);
  return true;
}

boolean EEPROM_WriteBadgeToMemory(char* badge){
  char emptyLocation = EERPROM_GetEmptyBadgeLocation();
  if(emptyLocation != -1){
    strncpy(m_StoredBadges[emptyLocation], badge, BADGE_LENGTH);
    EEPROM.put(0,m_StoredBadges);
    return true;
  }
  return false;
}

char EERPROM_GetEmptyBadgeLocation(){
  int i;
  for(i = 0; i<MAX_NR_BADGES; i++){
    if(EEPROM_CompareBadges(m_EmptyBadge, m_StoredBadges[i]))
      return i;
  }
  return -1;
}

BadgeType EEPROM_GetBadgeType(char* badge){
  if(EEPROM_CompareBadges(badge,m_MasterBadge)){
    return MASTER_BADGE;
  }else{
    for (int i = 0; i< MAX_NR_BADGES; i++){
      if(EEPROM_CompareBadges(badge, m_StoredBadges[i]))
        return KNOWN_BADGE;
    }
    return UNKNOWN_BADGE;
  }
}

boolean EEPROM_CompareBadges(char* badge1, char* badge2){
  Serial.println("Compare:");
  Serial.println(badge1);
  Serial.println(badge2);
  return strcmp(badge1, badge2)==0;
}

void EEPROM_PrintBadges(){
  int i,j;
  for(i = 0; i<MAX_NR_BADGES; i++){
    Serial.print("Badge ");
    Serial.print(i);
    Serial.print(": ");
    for(j = 0; j<BADGE_LENGTH; j++){
       Serial.print(m_StoredBadges[i][j],DEC);
       Serial.print(",");
    }
    Serial.println("");
  }
}

