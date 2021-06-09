// *****************************************
// Cylon
// *****************************************

extern uint8_t around[];

uint8_t gCylonDelay = 30; 
boolean gCylonCycleColor = true;
uint8_t gCylonTrail = 200; 

void fadeall() { 
  for(int i = 0; i < NUM_LEDS; i++) 
  { 
    gLeds[i].nscale8(gCylonTrail); 
  } 
}

void fCylon() {
  static uint8_t localHue = 0;
  static uint8_t localIndex = 0;
   
  fadeall();
  // Set the index led to localHue
  gLeds[around[localIndex]] = CHSV(localHue, 255, 255);

  if (gCylonCycleColor) localHue++;
  
  localIndex++;
  if (localIndex == EDGE_LEDS) {
    localIndex = 0;
  }
  FastLED.delay(gCylonDelay);

}

// *****************************************
// Cycle
// *****************************************

String setCylonCycleColor(String value) {
  gCylonCycleColor = value.toInt();
  storageWrite("cylonCycle");
  broadcastInt("cylonCycle", gCylonCycleColor);
  return String(gCylonCycleColor);
}

String getCylonCycleColor() {
  return String(gCylonCycleColor);
}

// *****************************************
// Trail
// *****************************************

String setCylonTrail(String value) {
  gCylonTrail = value.toInt();
  storageWrite("cylonTrail");
  broadcastInt("cylonTrail", gCylonTrail);
  return String(gCylonTrail);
}

String getCylonTrail() {
  return String(gCylonTrail);
}

// *****************************************
// Delay
// *****************************************

String setCylonDelay(String value) {
  gCylonDelay = value.toInt();
  storageWrite("cylonDelay");
  broadcastInt("cylonDelay", gCylonDelay);
  return String(gCylonDelay);
}

String getCylonDelay() {
  return String(gCylonDelay);
}