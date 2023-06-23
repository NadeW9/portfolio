#include "OpticalEncoder.h"
#include "Motor.h"
#include "Pot.h"

void setup(){
  Serial.begin(115200); 
  pinMode(encoder_pin, INPUT);
  attachInterrupt(encoder_pin, counter, RISING); //change should be rising 
  startTime = millis();

  Serial.print("Seconds ");
  Serial.print("RPM ");
  Serial.print("Pulses ");
  Serial.println("Velocity[Km/h]");

  ledcSetup(PWM_pin, PWM_freq, PWM_reso);
  ledcAttachPin(VR_Speed, PWM_pin);

  pinMode(EL_Start_Stop, OUTPUT);
  pinMode(Signal_Hall, INPUT);
  pinMode(ZF_Direction, OUTPUT);
  attachInterrupt(Signal_Hall, plus, CHANGE);

  digitalWrite(EL_Start_Stop, LOW);
  digitalWrite(ZF_Direction, HIGH);
  ledcWrite(PWM_pin, SPEED);
  delay(1000);
/*
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    potValue = 0;
  }
*/
}

void loop(){
  currentTime = millis();
  //getPotValue();
  if(pos<steps){
    digitalWrite(EL_Start_Stop, HIGH);
    if(currentTime - startTime >= 1000){ 
      gpio_intr_disable(encoder_pin); //Don't process interrupts during calculations
      rpm = (60 * 1000 / pulsesperturn ) / (currentTime - startTime) * pulses; // calculate rpm
      velocity = rpm * 3.1416 * wheel_diameter * 60 / 1000000; // convert to km/h
      //Serial.print(currentTime/1000); Serial.print("       ");
      //Serial.print(rpm,DEC); Serial.print("   ");
      //Serial.println(pulses,DEC);// Serial.print("     ");
      Serial.println(velocity,2); 
      pulses = 0; 
      gpio_intr_enable(encoder_pin); // Restart the interrupt processing
      startTime = currentTime;
   }
   else{}
  }
  else{
    ledcWrite(PWM_pin, 0);
    digitalWrite(EL_Start_Stop, LOW);
  }
}
