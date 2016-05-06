/*
---------------------------------------------------------------
 * LED 
---------------------------------------------------------------
*/
#include "FastLED.h"

#define LED_PIN 5
#define BRIGHTNESS  100
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[1];
byte currentSaturation,previouseSaturation,setPointSaturation;
byte currentHue,previouseHue,setPointHue;
byte currentValue,previouseValue,setPointValue;
int saturationFadeTime,hueFadeTime,valueFadeTime;
long saturationStartTime,hueStartTime,valueStartTime;
long preTime = 0;
LED_CONTROLLER_STATE LedCurrentState = LED_STARTUP;
LED_CONTROLLER_STATE LedPreviouseState = LED_STARTUP;
LED_CONTROLLER_STATE LedNextState = LED_STARTUP;
byte LedCounter = 0;

void LED_Setup(){
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, 1).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
}

void LED_Loop(){
  switch(LedCurrentState){
    case LED_STARTUP:
      if((millis() - preTime)>500){
        LED_SetSaturation(255,0);
        LED_SetHue(32,0);
        preTime = millis();
        if(setPointValue == 255){
          LED_SetValue(0,500);
        }else{
          LED_SetValue(255,500);
        }
      }
      break;
    case LED_ADD:
        if((millis() - preTime)>1000){
          LED_SetSaturation(255,0);
          LED_SetHue(160,0);
          preTime = millis();
          if(setPointValue == 255){
            LED_SetValue(0,1000);
          }else{
            LED_SetValue(255,1000);
          }
        }
      break;
    case LED_ERROR:
        if((millis() - preTime)>250){
          LED_SetSaturation(255,0);
          LED_SetHue(0,0);
          preTime = millis();
          if(setPointValue == 255){
            LED_SetValue(0,250);
          }else{
            LED_SetValue(255,250);
          }
        }
      break;
  case LED_OK:
    if(((millis() - preTime)>150)){
      if(LedCounter == 4)
        LED_SetState(LedNextState);
      LedCounter ++;
      LED_SetSaturation(255,0);
      LED_SetHue(96,0);
      preTime = millis();
      if(setPointValue == 255){
        LED_SetValue(0,0);
      }else{
        LED_SetValue(255,0);
      }
    }
    break;
 case LED_FAIL:
  if(((millis() - preTime)>150)){
    if(LedCounter == 4)
      LED_SetState(LedNextState);
    LedCounter ++;
    LED_SetSaturation(255,0);
    LED_SetHue(0,0);
    preTime = millis();
    if(setPointValue == 255){
      LED_SetValue(0,0);
    }else{
      LED_SetValue(255,0);
    }
  }
  break;    
 case LED_NORMAL:
    if((millis() - preTime)>1000){
      LED_SetSaturation(255,0);
      LED_SetHue(96,0);
      preTime = millis();
      if(setPointValue == 255){
        LED_SetValue(0,1000);
      }else{
        LED_SetValue(255,1000);
      }
    }
  break; 
 case LED_DELETE:
    if((millis() - preTime)>1000){
      LED_SetSaturation(255,0);
      LED_SetHue(224,0);
      preTime = millis();
      if(setPointValue == 255){
        LED_SetValue(0,1000);
      }else{
        LED_SetValue(255,1000);
      }
    }
  break;  
   case LED_TRANSMIT:
    if((millis() - preTime)>100){
      LED_SetSaturation(255,0);
      LED_SetHue(128,0);
      preTime = millis();
      if(setPointValue == 255){
        LED_SetValue(0,100);
      }else{
        LED_SetValue(255,100);
      }
    }
  break;  
    default:
      break;
  } 

  if(millis()<(saturationStartTime+saturationFadeTime)){
    currentSaturation = previouseSaturation + (setPointSaturation - previouseSaturation)*( (float)(millis()-saturationStartTime)/(float)saturationFadeTime); 
  }
  if(millis()<(hueStartTime+hueFadeTime)){
    currentHue = previouseHue + (setPointHue - previouseHue)*( (float)(millis()-hueStartTime)/(float)hueFadeTime);  
  }
  if(millis()<(valueStartTime+valueFadeTime)){
    currentValue = previouseValue + (setPointValue - previouseValue)*( (float)(millis()-valueStartTime)/(float)valueFadeTime); 
  }
  leds[0]  = CHSV( currentHue, currentSaturation, currentValue);
  FastLED.show();
}

void LED_SetState(LED_CONTROLLER_STATE ledState){
  LedPreviouseState = LedCurrentState;
  LedCurrentState = ledState;
}

void LED_SetState(LED_CONTROLLER_STATE ledState,LED_CONTROLLER_STATE nextLedState ){
  LedPreviouseState = LedCurrentState;
  LedCurrentState = ledState;
  LedNextState = nextLedState;
  LedCounter = 0;
}

void LED_SetSaturation(byte saturation, int fadeTime){
  if(fadeTime<30){
    previouseSaturation = currentSaturation;
    currentSaturation = saturation;
  }
  previouseSaturation = currentSaturation;
  setPointSaturation = saturation;
  saturationFadeTime = fadeTime;
  saturationStartTime = millis(); 
}

void LED_SetHue(byte hue, int fadeTime){
  if(fadeTime<30){
    previouseHue = currentHue;
    currentHue = hue;
  }
  previouseHue = currentHue;
  setPointHue = hue;
  hueFadeTime = fadeTime; 
  hueStartTime = millis(); 
}
void LED_SetValue(byte value, int fadeTime){
  if(fadeTime<30){
    previouseValue = currentValue;
    currentValue = value;
  }
  previouseValue = currentValue;
  setPointValue = value;
  valueFadeTime = fadeTime; 
  valueStartTime = millis();
}
