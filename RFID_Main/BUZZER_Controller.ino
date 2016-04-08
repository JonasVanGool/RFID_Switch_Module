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
#define  BUZZER_STARTUP 4
#define  BUZZER_ERROR 5

int BUZZER_Sequence[10][10][2];
boolean m_CmdStart = false;
byte m_Cmd = -1;
boolean m_CmdReady = true; 
byte m_CurrentNote = 0;
long m_StartTimeNote = 0;

void BUZZER_Setup(){

  BUZZER_Sequence[OK][0][0] = 440; //Hz
  BUZZER_Sequence[OK][0][1] = 100; //ms
  BUZZER_Sequence[OK][1][0] = 0; //Hz
  BUZZER_Sequence[OK][1][1] = 50; //ms
  BUZZER_Sequence[OK][2][0] = 440; //Hz
  BUZZER_Sequence[OK][2][1] = 100; //ms
  BUZZER_Sequence[OK][3][0] = -1; //Hz

  BUZZER_Sequence[FAIL][0][0] = 540; //Hz
  BUZZER_Sequence[FAIL][0][1] = 300; //ms
  BUZZER_Sequence[FAIL][1][0] = 340; //Hz
  BUZZER_Sequence[FAIL][1][1] = 500; //ms
  BUZZER_Sequence[FAIL][2][0] = -1; //Hz
  
  BUZZER_Sequence[ADD][0][0] = 440; //Hz
  BUZZER_Sequence[ADD][0][1] = 300; //ms
  BUZZER_Sequence[ADD][1][0] = 440; //Hz
  BUZZER_Sequence[ADD][1][1] = 300; //ms
  BUZZER_Sequence[ADD][2][0] = -1; //Hz

  BUZZER_Sequence[BUZZER_STARTUP][0][0] = 440; //Hz
  BUZZER_Sequence[BUZZER_STARTUP][0][1] = 400; //ms
  BUZZER_Sequence[BUZZER_STARTUP][1][0] = 523; //Hz
  BUZZER_Sequence[BUZZER_STARTUP][1][1] = 400; //ms
  BUZZER_Sequence[BUZZER_STARTUP][2][0] = -1; //Hz

  
  BUZZER_Sequence[BUZZER_ERROR][0][0] = 880; //Hz
  BUZZER_Sequence[BUZZER_ERROR][0][1] = 200; //ms
  BUZZER_Sequence[BUZZER_ERROR][1][0] = 880; //Hz
  BUZZER_Sequence[BUZZER_ERROR][1][1] = 200; //ms
  BUZZER_Sequence[BUZZER_ERROR][2][0] = 880; //Hz
  BUZZER_Sequence[BUZZER_ERROR][2][1] = 200; //ms
  BUZZER_Sequence[BUZZER_ERROR][3][0] = 880; //Hz
  BUZZER_Sequence[BUZZER_ERROR][3][1] = 200; //ms
  BUZZER_Sequence[BUZZER_ERROR][4][0] = 880; //Hz
  BUZZER_Sequence[BUZZER_ERROR][4][1] = 200; //ms
  BUZZER_Sequence[BUZZER_ERROR][5][0] = 880; //Hz
  BUZZER_Sequence[BUZZER_ERROR][5][1] = 200; //ms
  BUZZER_Sequence[BUZZER_ERROR][6][0] = -1; //Hz

}

void BUZZER_Loop(){ 
  if(!m_CmdReady){
    m_CmdStart = false;
    if(millis()> (BUZZER_Sequence[m_Cmd][m_CurrentNote][1] + m_StartTimeNote)){ 
      m_CurrentNote ++;
      if(BUZZER_Sequence[m_Cmd][m_CurrentNote][0] == -1){
         m_CmdReady = true;
         noTone(BUZZER_PIN);
         RFID_StartListening();
      }else{
        noTone(BUZZER_PIN);
        tone(BUZZER_PIN,BUZZER_Sequence[m_Cmd][m_CurrentNote][0]);
        m_StartTimeNote = millis();
      }
    }  
  } 
}

void BUZZER_StartSequence(byte BuzzerSequence){
  if(!m_CmdReady)
    return;
  // Initialize variables
  RFID_StopListening();
  m_Cmd = BuzzerSequence;
  m_CmdStart = true;
  m_CmdReady = false;
  m_CurrentNote = 0;
  // Start first note
  tone(BUZZER_PIN,BUZZER_Sequence[m_Cmd][m_CurrentNote][0]);
  m_StartTimeNote = millis();
}

