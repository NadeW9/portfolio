//#### MOTOR DRIVERS ####
//GPIO pins
//        startstop   speed       direction   hall
const int M1_EL = 32, M1_VR = 22, M1_ZF = 33, M1_Signal = 34;
const int M2_EL = 19, M2_VR = 23, M2_ZF = 18, M2_Signal = 35;

//PWM Settings
const int PWM_freq = 5000, PWM_reso = 8;
const int M1_PWM_pin = 0, M2_PWM_pin = 5;

//Initial Settings
int steps = 10000;
volatile int M1_pos = 0, M2_pos = 0;
float targetSpeed = ; //TO BE DECLARED, in km/h. Maximum 5km/h with ESP32 and 36V battery

//Boolean Declarations
bool M1_STATE = M1_pos<steps;
bool M2_STATE = M2_pos<steps;

//#### MOTOR ENCODERS ####
//GPIO pins
const int M1_encoder_pin = 5, M2_encoder_pin = ;

//Initial Settings
unsigned int M1_rpm = 0, M2_rpm = 0;
float M1_realVelocity = 0, M2_realVelocity = 0;
float M1_currentSpeed, M2_currentSpeed;
float M1_PI_error, M2_PI_error;
volatile byte M1_pulses = 0, M2_pulses = 0;
unsigned long currentTime, startTime;

unsigned int pulsesperturn = 45; // Number of slots in my encoder
const int wheel_diameter = 159;   // Diameter of WHEEL in mm (actually 15.874)

//#### JOINT ENCODER ####
