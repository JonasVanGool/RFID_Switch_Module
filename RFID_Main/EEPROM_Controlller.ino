/*
---------------------------------------------------------------
 * EEPROM
---------------------------------------------------------------
*/
#include <EEPROM.h>

#define MAX_NR_BADGES 10
#define BADGE_LENGTH 11
#define TWO_MINUTES 120000
char m_MasterBadge[BADGE_LENGTH] = {48,48,48,48,54,70,55,56,56,65,'\0'};
char m_EmptyBadge[BADGE_LENGTH] = {0,0,0,0,0,0,0,0,0,0,'\0'};
char m_StoredBadges[MAX_NR_BADGES][BADGE_LENGTH];

#define HISTORY_SIZE 5
byte m_LoginBadgeHistory[HISTORY_SIZE];
byte m_LoginTimeHistory[HISTORY_SIZE];



#define MEMORY_PARTITIONS 8 // =128 byte/partition = 10*11(storedbadges) + 5*2(history)+ 8(memorycontrol)
int MEMORY_BLOCKSIZE = sizeof(m_LoginBadgeHistory) + sizeof(m_LoginTimeHistory) + sizeof(m_StoredBadges);
byte m_MemoryControl[MEMORY_PARTITIONS];
int adressStoredBadges = 0;
int adressLoginBadgeHistory = 0;
int adressLoginTimeHistory = 0;
long loginTimer = 0;
byte loginTimerValue = 0;
void EEPROM_Setup(){
  //EEPROM_InitAllMemory();
  EEPROM_ReadAllData();
}

void EEPROM_Loop(){
  // Only count when logged in
  if((millis()-loginTimer>TWO_MINUTES) && m_LoginTimeHistory[0] == 255 /*to prevent over flow*/){
    loginTimer = millis();
    loginTimerValue++;
  }
}

void EEPROM_InitAllMemory(){
  // Write memory management array
  for(byte i = 0; i<MEMORY_PARTITIONS;i++)
   m_MemoryControl[i]=i;
  // Write badges array
  for (int i = 0; i< MAX_NR_BADGES; i++){
    strncpy(m_StoredBadges[i], m_EmptyBadge, BADGE_LENGTH);   
  }
  //Write history
  for(int i=0; i<HISTORY_SIZE;i++){
    m_LoginBadgeHistory[i]=0;
    m_LoginTimeHistory[i]=0;
  }
  
  EEPROM.put(0,m_MemoryControl);
  for(int adress = sizeof(m_MemoryControl); adress < EEPROM.length();adress+=MEMORY_BLOCKSIZE){
    EEPROM.put(adress,m_LoginBadgeHistory);
    EEPROM.put(adress + sizeof(m_LoginBadgeHistory) ,m_LoginTimeHistory);
    EEPROM.put(adress + sizeof(m_LoginBadgeHistory) + sizeof(m_LoginTimeHistory) ,m_StoredBadges);
  } 
}

void EEPROM_SaveAllData(){
  int adress = sizeof(m_MemoryControl) + MEMORY_BLOCKSIZE * EEPROM_GetNewPartition();
  EEPROM.put(adress,m_LoginBadgeHistory);
  EEPROM.put(adress + sizeof(m_LoginBadgeHistory),m_LoginTimeHistory);
  EEPROM.put(adress + sizeof(m_LoginBadgeHistory) + sizeof(m_LoginTimeHistory),m_StoredBadges);
}

void EEPROM_ReadAllData(){
  int adress = sizeof(m_MemoryControl) + MEMORY_BLOCKSIZE * EEPROM_GetReadPartition();
  EEPROM.get(adress,m_LoginBadgeHistory);
  EEPROM.get(adress + sizeof(m_LoginBadgeHistory),m_LoginTimeHistory);
  EEPROM.get(adress + sizeof(m_LoginBadgeHistory) + sizeof(m_LoginTimeHistory),m_StoredBadges);
}

char EEPROM_GetReadPartition(){
  EEPROM.get(0,m_MemoryControl);
  for(int i = 0 ; i < MEMORY_PARTITIONS-1; i++){
    if(m_MemoryControl[i+1] != m_MemoryControl[i]+1){
      return i;
    }
  }
  return MEMORY_PARTITIONS-1;
}

boolean EEPROM_LogIn(char* badge){
  //shift memory
  for(int i=0; i<HISTORY_SIZE-1;i++){
    m_LoginBadgeHistory[i+1] = m_LoginBadgeHistory[i];
    m_LoginTimeHistory[i+1] = m_LoginTimeHistory[i];
  }
  byte badgeNumber = EEPROM_GetBadgeNumber(badge);
  if(badgeNumber == 255)
    return false;
  m_LoginBadgeHistory[0] = badgeNumber;
  m_LoginTimeHistory[0] = 255;
  loginTimerValue = 0;
  // Save all the data
  EEPROM_SaveAllData();
}

boolean EEPROM_LogOut(boolean followedByLogin){
   m_LoginTimeHistory[0] = loginTimerValue;
   loginTimerValue = 0;
   if(!followedByLogin)
    EEPROM_SaveAllData();
   return true;
}

byte EEPROM_GetBadgeNumber(char* badge){
  for(int i = 0; i<MAX_NR_BADGES; i ++){
    if(EEPROM_CompareBadges(badge,m_StoredBadges[i])
      return i;
  }
  return 255;
}

byte EEPROM_GetNewPartition(){
  EEPROM.get(0,m_MemoryControl);
  for(byte i = 1 ; i < MEMORY_PARTITIONS; i++){
    if((m_MemoryControl[i-1] + 1) != m_MemoryControl[i]){
      m_MemoryControl[i] = m_MemoryControl[i-1] + 1;
      EEPROM.put(i,m_MemoryControl[i]);
      return i;
    }
  }
  if(m_MemoryControl[0] == 0){
    m_MemoryControl[0] = MEMORY_PARTITIONS;
  }else{
    m_MemoryControl[0] = 0;
  }
  EEPROM.put(0,m_MemoryControl[0]);
  return 0;
}

boolean EEPROM_DeleteBadgeFromMemory(char* badge){
  for (int i = 0; i< MAX_NR_BADGES; i++){
    if(EEPROM_CompareBadges(badge, m_StoredBadges[i])){
       strncpy(m_StoredBadges[i], m_EmptyBadge, BADGE_LENGTH);
       return true;
    }    
  }
  return false;
}

boolean EEPROM_WriteBadgeToMemory(char* badge){
  char emptyLocation = EERPROM_GetEmptyBadgeLocation();
  if(emptyLocation != -1){
    strncpy(m_StoredBadges[emptyLocation], badge, BADGE_LENGTH);
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
  #ifdef debug
  Serial.println("Compare:");
  Serial.println(badge1);
  Serial.println(badge2);
  #endif
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

