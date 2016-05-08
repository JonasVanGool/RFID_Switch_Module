/*
---------------------------------------------------------------
 * Transmit 
---------------------------------------------------------------
*/

#define TRANSMIT_PIN 6
#define DECOUPLING_MASK 0b11001010 
// BAUDRATE 600bps
#define delay1 1532
#define delay2 1534
#define TRANSMIT_INVERVAL 5000
byte transmitData = false;
byte delimiter[5] = {0,1,0,1,0};
long previouseTransmitTime = 0;

void TRANSMIT_Setup(){
  pinMode(TRANSMIT_PIN,OUTPUT);
}

void TRANSMIT_Loop(){
  if(transmitData && (millis()-previouseTransmitTime > TRANSMIT_INVERVAL)){
    LED_HardWriteColor(255,128,255);
    previouseTransmitTime = millis();
    // transmit start frame 
    TRANSMIT_transmitArray(5,(uint8_t*)delimiter);
    delay(50); 
    for(int i = 0; i< MAX_NR_BADGES; i++){
       TRANSMIT_transmitArray(BADGE_LENGTH,(uint8_t*)m_StoredBadges[i]);
       delay(10);
    }
    delay(50); 
    TRANSMIT_transmitArray(HISTORY_SIZE,(uint8_t*)m_LoginBadgeHistory);
    delay(50); 
    TRANSMIT_transmitArray(HISTORY_SIZE,(uint8_t*)m_LoginTimeHistory);
  }
}

void TRANSMIT_On(){
  previouseTransmitTime = millis();
  transmitData = true;
}

void TRANSMIT_Off(){
  transmitData = false;
}

void TRANSMIT_transmitArray(uint8_t numBytes, uint8_t *data)
{
  // Send 14 0's
  for( int16_t i = 0; i < 14; i++) //send capture pulses
    sendZero(); //end of capture pulses
 
  // Send a single 1
  sendOne(); //start data pulse
 
  // Send the user data
  for (uint8_t i = 0; i < numBytes; i++)
  {
    uint16_t mask = 0x01; //mask to send bits
    uint8_t d = data[i] ^ DECOUPLING_MASK;
    for (uint8_t j = 0; j < 8; j++)
    {
      if ((d & mask) == 0)
        sendZero();
      else
        sendOne();
      mask <<= 1; //get next bit
    }//end of byte
  }//end of data

  // Send 2 terminatings 0's to correctly terminate the previous bit and to turn the transmitter off
  sendZero(); 
  sendZero();
}//end of send the data


void sendZero(void)
{
  delayMicroseconds(delay1);
  digitalWrite(TRANSMIT_PIN, HIGH);

  delayMicroseconds(delay2);
  digitalWrite(TRANSMIT_PIN, LOW);
}//end of send a zero


void sendOne(void)
{
  delayMicroseconds(delay1);
  digitalWrite(TRANSMIT_PIN, LOW);

  delayMicroseconds(delay2);
  digitalWrite(TRANSMIT_PIN, HIGH);
}//end of send one
