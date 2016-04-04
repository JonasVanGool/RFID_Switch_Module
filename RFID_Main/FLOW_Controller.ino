/*
---------------------------------------------------------------
 * FLOW
---------------------------------------------------------------
*/
// Main controller states
enum MAIN_CONTROLLER_STATE{
  STARTUP,
  WAIT_FOR_SCAN_STARTUP,
  WAIT_FOR_SCAN_NORMAL,
  WAIT_FOR_SCAN_ADD,
  WAIT_FOR_SCAN_DELETE,
  WAIT_FOR_SCAN 
};
MAIN_CONTROLLER_STATE MainCurrentState = STARTUP;
MAIN_CONTROLLER_STATE MainPreviouseState = STARTUP;

void FLOW_Setup(){
    Serial.begin(9600);
}

void FLOW_Loop(){
  
}
