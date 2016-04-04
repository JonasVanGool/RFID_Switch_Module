/*
---------------------------------------------------------------
 * RELAY 
---------------------------------------------------------------
*/
#define RELAY_PIN 3

void RELAY_Setup(){
  pinMode(RELAY_PIN,OUTPUT);
  digitalWrite(RELAY_PIN,LOW);
}

void RELAY_Loop(){
  
}
