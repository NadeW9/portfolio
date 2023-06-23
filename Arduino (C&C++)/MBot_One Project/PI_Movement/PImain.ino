//Main code
/* The motion portion is riding on justmovegoddammit6, even though that hasnt been tested and verified properly yet.
//Aim: Simple PI of VELOCITY based on pre-keyed inputs
  // When 0 OR 255 is reached, have to stop increasing/decreasing (because doesnt make sense!! ESP32 will be confused).
  // If let's say the upper cap of 255 is reached, but the desired speed hasnt been reached, Serial.print an error message
// Make 1 PI using optical encoders, and another for hall sensor (separate program)
*/
/*
 * Psuedo code:
 * 
 * float targetSpeed = fixed_value_I_want;
 * float currentSpeed = getInputFromEncoders;
 * float error;
 * 
 * loop:
 *  error = targetSpeed - currentSpeed; //may or may not need to add a multiplier
 *  currentSpeed = currentSpeed + error
 *  drive_wheel_at_currentSpeed
 *  
 *  #######################3
 *  
 *  Need to relate voltages (for battery, 12V-36V) and input 0-255 to speed of wheel
 *    > Then when first keyed in raw speed, write the wheel speed at this pre-determined 0-255
 *    > But then +-(k*1) as needed based on feedback loop; k is multiplier
 *  OR
 *    > Start at 0 no matter what, increase by 1 until desired speed reached
 */
#include <dummy.h>
#include "A_GlobalVariables.h" 
#include "B_MotorFunctions.h"
#include "C_EncoderFunctions.h"

void setup() {
  Serial.begin(115200);

  //#### MOTOR DRIVERS ####
  ledcSetup(M1_PWM_pin, PWM_freq, PWM_reso); ledcSetup(M2_PWM_pin, PWM_freq, PWM_reso);
  ledcAttachPin(M1_VR, M1_PWM_pin);          ledcAttachPin(M2_VR, M2_PWM_pin);
  pinMode(M1_EL, OUTPUT);                    pinMode(M2_EL, OUTPUT);
  pinMode(M1_Signal, INPUT);                 pinMode(M2_Signal, INPUT);
  pinMode(M1_ZF, OUTPUT);                    pinMode(M2_ZF, OUTPUT);
  attachInterrupt(M1_Signal, M1_plus, CHANGE);  attachInterrupt(M2_Signal, M2_plus, CHANGE);

  digitalWrite(M1_EL, LOW); digitalWrite(M2_EL, LOW);
  digitalWrite(M1_ZF, HIGH); digitalWrite(M2_ZF, HIGH);
  
  //#### MOTOR ENCODERS ####
  pinMode(M1_encoder_pin, INPUT); pinMode(M2_encoder_pin, INPUT);
  attachInterrupt(M1_encoder_pin, M1_counter, RISING);
  attachInterrupt(M2_encoder_pin, M2_counter, RISING);
  M1_pulses = 0; M2_pulses = 0;
  M1_rpm = 0; M2_rpm = 0;
  startTime = millis();

  // write default values based on experiment --- INCLUDE THIS PART IF DO NOT START FROM 0
  ledcWrite(M1_VR, exp_default);
  ledcWrite(M2_VR, exp_default);
}

void loop() {
  if(currentTime - startTime >= 500){ // Check every 0.5s
    encoderLoop; //gets real_velocity
    M1_currentSpeed = M1_realVelocity;
    M1_PI_error = targetSpeed - M1_currentSpeed;
    M1_currentSpeed = M1_currentSpeed + M1_PI_error;
    //write this currentSpeed to wheel;

    M2_currentSpeed = M2_realVelocity;
    M2_PI_error = targetSpeed - M2_currentSpeed;
    M2_currentSpeed = M2_currentSpeed + M2_PI_error;
    //write this currentSpeed to wheel
    
    M1_pulses = 0; 
    M2_pulses = 0;

    Serial.println(M1_realVelocity);
    
    interrupts(); // Restart the interrupt processing
    startTime = currentTime;
  }
}
