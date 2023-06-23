/*  Ellipsoidal Speed Control Generation
 *   Adapted from Youssef's original code
 *  - Sine velocity speed control via number of steps in specified time intervals
 *  - Originally "hardwired" for 1/16 microstepping
 *  
 *  Objective: ellipsoidal speed control for 1/256 microstepping w/ TMC5160 driver
 *  Next objective: generate values for any microstepping/steps intervals
 *  VERSION: 1.3 ; 27/09/22
 *  
 *  ~~~Author~~~
 *  Nadine W.
 */

#include <TMCStepper.h>

//Pin definitions-------------------------------------------------------------
#define EN_PIN           7 // Enable
#define DIR_PIN          8 // Direction
#define STEP_PIN         9 // Step
#define CS_PIN           10 // Chip select
#define MOSI_PIN         11 // SDI/MOSI for Nano
#define MISO_PIN         12 // SDO/MISO for Nano
#define SCK_PIN          13 // CLK/SCK for Nano
#define SWITCH           2 // TOGGLE SWITCH INPUT TO ACTIVATE/DEACTIVATE TREMOR MOTION

#define R_SENSE 0.075f // TMC5160 necessary value

// Parameters-------------------------------------------------------------
#define min_pulse 50 // MINIMUM PULSE DURATION LIMIT FOR THE STEPPER DRIVER IN MICROSECONDS
#define STEPS 3072 // Total number of steps for 21.6 deg travel
int halfSTEPS = STEPS/2;
/* 192/3200 == 21.6 so arm travels about 20 deg total (+10, -20, +10, -20...)
 * 32 microstep -> 192*2
 * 8 microstep -> 192/2
 */

//#define DEBUG // UNCOMMENT THIS LINE FOR SERIAL DEBUG
​
// Global variables declarations----------------------------------------------
int pos = STEPS >> 1; // INTERNAL ANGULAR POSTIION VARIABLE, INITIALISED AT MID POSITION
int dir = 1; // STEPPER MOTOR DIRECTION - 1 FOR CLOCKWISE, -1 FOR ANTI-CLOCKWISE
boolean switch_state = 0; // TOGGLE SWITCH STATE FLAG

// QUARTER TREMOR PERIODS IN CLOCK CYCLES FOR EACH FREQUENCY BAND. USE THESE VALUES TO FINE TUNE TREMOR FREQUNCIES (1314773)
unsigned long hz_3_prd = (4367428 + 18999999) / 4; // Adjust this to set the 1Hz freqncy
//unsigned long hz_5_prd = 2035200 / 4;
//unsigned long hz_7_prd = (1124352 + 465000) / 4;// adjusted to achieve 7.9Hz 
//unsigned long hz_9_prd = 654720 / 4;
//unsigned long hz_11_prd = 370176 / 4;
//................................................................
​
float prd = hz_3_prd; // TREMOR PERIOD IN CLOCK CYCLES. THIS VALAUE IS CALCULATED EVERY STEP CYCLE


//.................................................................
​
void setup() {
  // Pin mode defintions------------------------------------------------------
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH); // deactivate driver (LOW active)
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW); // direction: LOW or HIGH
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(STEP_PIN, LOW);

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  pinMode(MOSI_PIN, OUTPUT);
  digitalWrite(MOSI_PIN, LOW);
  pinMode(MISO_PIN, INPUT);
  digitalWrite(MISO_PIN, HIGH);
  pinMode(SCK_PIN, OUTPUT);
  digitalWrite(SCK_PIN, LOW);

  pinMode(SWITCH, INPUT_PULLUP);
​
  // TIMER 1 INIT-------------------------------------------------------------
  TCCR1A = 0;
  TCCR1B = 1; // NORMAL 16-BIT MODE, NO PRESCALER, NO WAVEFORM OUTPUT
​ 
  // Other Initialisations
  Serial.begin(9600);
  Serial.println("\nStart...");

  driver.begin();
  driver.toff(4);                 // off time
  driver.blank_time(24);          // blank time

  driver.rms_current(1500);        // Set motor RMS current (mA), up to 3000mA
  driver.microsteps(256);          // Set microstep resolution

  driver.en_pwm_mode(true);       // Toggle stealthChop on TMC160
  //driver.en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
  driver.pwm_autoscale(true);     // Needed for stealthChop

  // Pre-calculated Inverse Sum Theta value
  float invSumTheta = getInvSum();

  digitalWrite(EN_PIN, LOW); // motor outputs on
}
​
​
void loop() {
  // Main operating loop------------------------------------------------------
  if (digitalRead(SWITCH)) { // CHECK STATE OF TOGGLE SWITCH
    digitalWrite(EN_PIN, LOW); // turn on the motor
    switch_state = 1; // SET SWITCH STATE FLAG
​
    // TOGGLE DIRECTION FLAG AT THE LIMITS OF MOTION
    if (pos == 0 ) {
      digitalWrite(DIR_PIN, HIGH); 
      dir = 1; 
    } else if (pos == STEPS) {
      digitalWrite(DIR_PIN, LOW);
      dir = -1;
    }
    //..............................................
​
    step_pulse(); // FUNCTION TO SEND A SINGLE STEP PULSE TO THE STEPPER DRIVER WITH A DEFINED PULSE LENGTH
    pos += dir; // INCREMENT/DECREMENT ANGULAR POSITION VALUE BASED ON THE DIRECTION FLAG
    unsigned int temp_pos = 0; // DECLARE A VARIABLE TO HOLD RESULT OF CALCULATED PERIOD VALUE
​
    // SET DIRECTION OF ITERATION THROUGH THE interval ARRAY BASED ON ANGULAR POSITION 
    if (pos < halfSTEPS) {
      temp_pos = pos;
    } else {
      temp_pos = STEPS - pos;
    }
    //.................................................................................
    
    unsigned int tik = 0; // INITIALISE VARIABLE TO HOLD TIMER1 VALUE DURING WAIT PERIOD
    // unsigned int del = prd * !!!interval[temp_pos]!!!; // CALCULATE PERIOD BASED ON ANGULAR POSITION AND SET TREMOR FREQUENCY
    float theta = getTheta(temp_pos);
    float y_inv = theta * invSumTheta;
    unsigned int del = prd * y_inv;
    
    
    
    // CLEAR TIMER1 REGISTER
    TCNT1H = 0; 
    TCNT1L = 0; 
    //......................
​
    // LOOP TO WAIT FOR SET TIMER PERIOD (IDLE DELAY)
    while (tik < del) { 
      tik = TCNT1H << 8;
      tik |= TCNT1L;
    }
    //..............................................
    
  } else { // PARKING ROUTINE WHEN TOGGLE SWITCH IS USED TO DEACTIVATE TREMOR MOTION
    if (switch_state) { // THIS IS USED TO CHECK FOR THE CHANGE OF THE TOGGLE SWITCH FROM ACTIVE TO INACTIVE. RUNS ONLY ONCE WHEN SWITCH IS TOGGLED
      if (pos < (STEPS >> 1)) { // CHECK IF ANGULAR POSITION IS LESS THAN OR GREATER THAN THE ZERO POSITION. SET DIRECTION FLAG AND PIN ACCORDINGLY
        digitalWrite(DIR_PIN, HIGH);
        dir = 1;
      } else {
        digitalWrite(DIR_PIN, LOW);
        dir = -1;
      }
      while (pos != (STEPS >> 1)) { // LOOP TO STEP THE MOTOR TO THE PARKED POSITION
        step_pulse();
        pos += dir;
        delayMicroseconds(2000);
      }
      switch_state = 0; // CLEAR SWITCH STATE FLAG
      digitalWrite(EN_PIN, HIGH); // turn off the motor
    }
  }
}
​
// Step pulse function-----------------------------------------------------------
void step_pulse() { // A function to send a step pulse to the motor driver
  digitalWrite(STEP_PIN, HIGH); // Set pulse pin to high
  delayMicroseconds(min_pulse); // Wait for a specified minimum pulse length
  digitalWrite(STEP_PIN, LOW); // Set pulse pin to low
}

// Get theta---------------------------------------------------------------------
float getTheta(int x){
  float theta = 1/sqrt(2*x*halfSTEPS - x*x);
  return theta;
}

// Get inverse sum theta---------------------------------------------------------
float getInvSum(){
  float sum = 0;
  float res = 0;
  for(int i =1; i <= halfSTEPS; i++){
    res = getTheta(i);
    sum += res;
  }
  float inv_sum = 1/sum;
  return inv_sum;
}
