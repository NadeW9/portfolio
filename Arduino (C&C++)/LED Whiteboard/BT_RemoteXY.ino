#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <RemoteXY.h>
#define REMOTEXY_BLUETOOTH_NAME "DefinitelyNothingShady"

#include <FastLED.h>

#define LED_PIN     15
#define NUM_LEDS    31
#define BRIGHTNESS  255
#define VOLTS       5
#define MAX_MA      2000
uint8_t r_out, g_out, b_out;
//CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;

#include "Pride.h"
#include "TwinkleFox.h"
#include "McLaren.h"

// RemoteXY configure
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 43 bytes
  { 255,5,0,0,0,36,0,16,8,1,6,0,28,43,31,31,2,26,2,1,
  37,4,22,11,2,26,31,31,79,78,0,79,70,70,0,3,4,9,42,9,
  34,2,26 };
  
// this structure defines all the variables and events of your control interface 
struct {

  // input variables
  uint8_t rgb_change_r; // =0..255 Red color value 
  uint8_t rgb_change_g; // =0..255 Green color value 
  uint8_t rgb_change_b; // =0..255 Blue color value 
  uint8_t onoff_switch; // =1 if switch ON and =0 if OFF 
  uint8_t select; // =0 if select position A, =1 if position B, =2 if position C, ... 
    
  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

void setup() 
{
  RemoteXY_Init();
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip).setDither(BRIGHTNESS < 255);
  FastLED.setBrightness(BRIGHTNESS);
  
  chooseNextColorPalette(gTargetPalette);
  
}

void loop() 
{ 
  RemoteXY_Handler ();

  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 
  
  if(RemoteXY.onoff_switch == 0){
    for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB (0, 0, 0);
    }
  }

  else{
    if(RemoteXY.select == 0){
      r_out = RemoteXY.rgb_change_r;
      g_out = RemoteXY.rgb_change_g;
      b_out = RemoteXY.rgb_change_b;
  
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB (r_out, g_out, b_out);
      }
    }

    else if(RemoteXY.select == 1){
      pride();
    }

    else if(RemoteXY.select == 2){
      TwinkleFox();
    }

    else if(RemoteXY.select == 3){
      McLaren();
    }
  }
  
  FastLED.show();
}
