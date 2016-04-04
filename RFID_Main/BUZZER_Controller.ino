/*
---------------------------------------------------------------
 * BUZZER 
---------------------------------------------------------------
*/
#define BUZZER_PIN 4

#define  ADD 0
#define  DELETE 1
#define  OK 2
#define  FAIL 3


int BUZZER_Sequence[4][10][2];

void BUZZER_Setup(){
  BUZZER_Sequence[ADD][0][0] = 440; //Hz
  BUZZER_Sequence[ADD][0][1] = 200; //ms
  BUZZER_Sequence[ADD][1][0] = 640; //Hz
  BUZZER_Sequence[ADD][1][1] = 40; //ms
  BUZZER_Sequence[ADD][2][0] = 440; //Hz
  BUZZER_Sequence[ADD][2][1] = 1000; //ms
}

void BUZZER_Loop(){
  
}

