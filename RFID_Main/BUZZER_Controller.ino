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
boolean m_CmdStart = false;
byte m_Cmd = -1;
boolean m_CmdReady = true; 
byte m_CurrentNote = 0;
long m_StartTimeNote = 0;

void BUZZER_Setup(){
  BUZZER_Sequence[ADD][0][0] = 440; //Hz
  BUZZER_Sequence[ADD][0][1] = 200; //ms
  BUZZER_Sequence[ADD][1][0] = 640; //Hz
  BUZZER_Sequence[ADD][1][1] = 40; //ms
  BUZZER_Sequence[ADD][2][0] = 440; //Hz
  BUZZER_Sequence[ADD][2][1] = 1000; //ms
  BUZZER_Sequence[ADD][3][0] = -1; //Hz

}

void BUZZER_Loop(){ 
  if(!m_CmdReady){
    m_CmdStart = false;
    if(millis()> (BUZZER_PIN,BUZZER_Sequence[m_Cmd][m_CurrentNote][1] + m_StartTimeNote)){ 
      m_CurrentNote ++;
      if(BUZZER_PIN,BUZZER_Sequence[m_Cmd][m_CurrentNote][0] == -1){
         m_CmdReady = true;
      }else{
        tone(BUZZER_PIN,BUZZER_Sequence[m_Cmd][m_CurrentNote][0]);
        m_StartTimeNote = millis();
      }
    }  
  } 
}

void BUZZER_StartSequence(byte BuzzerSequence){
  // Initialize variables
  m_Cmd = BuzzerSequence;
  m_CmdStart = true;
  m_CmdReady = false;
  m_CurrentNote == 0;
  // Start first note
  tone(BUZZER_PIN,BUZZER_Sequence[m_Cmd][m_CurrentNote][0]);
  m_StartTimeNote = millis();
}

