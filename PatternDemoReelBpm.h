// *****************************************
// Demo Reel BPM
// *****************************************

boolean gBpmCycle = true;
uint8_t gBpmHue = 0; 
uint8_t gBpmBeatsPerMinute = 62; 
uint8_t gBpmDelay = 25; 

void fBPM()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  CRGBPalette16 localPalette = PartyColors_p;
  uint8_t beat = beatsin8( gBpmBeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    gLeds[i] = ColorFromPalette(localPalette, gBpmHue+(i*2), beat-gBpmHue+(i*10));
  }

  // do some periodic updates
  if (gBpmCycle) gBpmHue++; 
  FastLED.delay(gBpmDelay);
}


// *****************************************
// Cycle
// *****************************************

String setBpmCycle(String value) {
 gBpmCycle = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gBpmCycle);
}

String getBpmCycle() {
  return String(gBpmCycle);
}

// *****************************************
// Hue
// *****************************************

String setBpmHue(String value) {
  gBpmHue = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gBpmHue);
}

String getBpmHue() {
  return String(gBpmHue);
}

// *****************************************
// Beats Per Minute
// *****************************************

String setBpmBeatsPerMinute(String value) {
  gBpmBeatsPerMinute = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gBpmBeatsPerMinute);
}

String getBpmBeatsPerMinute() {
  return String(gBpmBeatsPerMinute);
}

// *****************************************
// Delay
// *****************************************

String setBpmDelay(String value) {
 gBpmDelay = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gBpmDelay);
}

String getBpmDelay() {
  return String(gBpmDelay);
}
