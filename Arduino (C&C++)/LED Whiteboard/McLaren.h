int o_r = 255, o_g = 50, o_b = 5;
int b_r = 32, b_g = 29, b_b = 255;  
uint32_t previousMillis = 0;
boolean swap = false;

void orange(){
  r_out = o_r;
  g_out = o_g;
  b_out = o_b;
}

void blue(){
  r_out = b_r;
  g_out = b_g;
  b_out = b_b;
}

void McLaren(){
  
  for (int i = 0; i < 2; i++) {
    orange();
    leds[i] = CRGB (r_out, g_out, b_out);
  }
  
  int n = 2;
  for (int c = 0; c < 3; c++) {
    for (int i = n; i < n+3; i++) {
      blue();
      leds[i] = CRGB (r_out, g_out, b_out);
    }
    
    for (int i = n+3; i < n+8; i++) {
      orange();
      leds[i] = CRGB (r_out, g_out, b_out);
    }
    n = n+8;
  }
  
  for (int i = 26; i < 29; i++) {
    blue();
    leds[i] = CRGB (r_out, g_out, b_out);
  }
  
  for (int i = 29; i < 31; i++) {
    orange();
    leds[i] = CRGB (r_out, g_out, b_out);
  }
}
