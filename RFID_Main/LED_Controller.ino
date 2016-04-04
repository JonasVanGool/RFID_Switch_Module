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

// Led controller states
enum LED_CONTROLLER_STATE{
  LED_INIT,
  LED_NORMAL,
  LED_ADD,
  LED_DELETE
};
LED_CONTROLLER_STATE LedCurrentState = LED_INIT;
LED_CONTROLLER_STATE LedPreviouseState = LED_INIT;

void LED_Setup(){
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, 1).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
}

void LED_Loop(){
  
}
