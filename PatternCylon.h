
// *****************************************
// 4 - Cylon
// *****************************************

extern uint8_t around[];

void fadeall() { 
  for(int i = 0; i < NUM_LEDS; i++) 
  { 
    g_leds[i].nscale8(g_pattern_parameter_2); 
  } 
}

void cylon() {
  static uint8_t localHue = 0;
  static uint8_t localIndex = 0;
   
  fadeall();
  // Set the index led to localHue
  g_leds[around[localIndex]] = CHSV(localHue, 255, 255);
  // Show the leds
  FastLED.show(); 

  if (g_cycle) localHue++;
  
  localIndex++;
  if (localIndex == EDGE_LEDS) {
    localIndex = 0;
  }

  // Wait a little bit before we loop around and do it again
  delay(g_pattern_delayloop);

}

