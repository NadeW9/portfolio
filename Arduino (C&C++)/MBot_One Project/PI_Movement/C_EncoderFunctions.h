//Debounce counter not necessary with 562 capacitor
//Encoder code has not been checked with ESP32

void M1_counter(){
  M1_pulses++;
}

void M2_counter(){
  M2_pulses++;
}

void encoderLoop(){
    currentTime = millis();
    noInterrupts(); //Don't process interrupts during calculations
    M1_rpm = (60 * 1000 / pulsesperturn ) / (currentTime - startTime) * M1_pulses; // calculate rpm
    M2_rpm = (60 * 1000 / pulsesperturn ) / (currentTime - startTime) * M2_pulses;
    M1_realVelocity = M1_rpm * 3.1416 * wheel_diameter * 60 / 1000000; // convert to km/h
    M2_realVelocity = M2_rpm * 3.1416 * wheel_diameter * 60 / 1000000;
}
