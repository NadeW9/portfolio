//GPIO pins
const int EL_Start_Stop = 32;
const int VR_Speed = 22;
const int ZF_Direction = 33;
const int Signal_Hall = 34;

//PWM settings
const int PWM_freq = 5000;
const int PWM_reso = 8;
const int PWM_pin = 0;

//Initial Settings
volatile int pos = 0;
int steps = 18000;
int SPEED = 255;

void IRAM_ATTR plus(){
  pos++;
}
