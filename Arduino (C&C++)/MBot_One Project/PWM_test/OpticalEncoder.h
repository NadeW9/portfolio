//Tested and Working 23/02/21
#include <dummy.h>

#define encoder_pin GPIO_NUM_5 //do NOT use D2           
unsigned int rpm = 0;        
float velocity = 0;              
volatile int pulses = 0;  //Number of pulses

unsigned long currentTime;
unsigned long startTime;

unsigned int pulsesperturn = 30; // Number of slots in my encoder
const int wheel_diameter = 159;   // Diameter of wheel in mm

static volatile unsigned long debounce = 0;
portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR counter(){
  if(digitalRead(encoder_pin) && (micros()-debounce > 100) && digitalRead(encoder_pin)) { 
    portENTER_CRITICAL(&synch);
    debounce = micros();
    pulses++;
    portEXIT_CRITICAL(&synch);
  }
  else{
  }
}
