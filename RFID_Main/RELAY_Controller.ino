/*
---------------------------------------------------------------
 * RELAY 
---------------------------------------------------------------
*/
#define RELAY_PIN 3

void RELAY_Setup(){
  pinMode(RELAY_PIN,OUTPUT);
  RELAY_Off();
}

void RELAY_Loop(){
  
}

void RELAY_On(){
  digitalWrite(RELAY_PIN,HIGH);
}

void RELAY_Off(){
  digitalWrite(RELAY_PIN,LOW);
}

