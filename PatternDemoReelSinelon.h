// *****************************************
// Demo Reel Sinelon
// *****************************************

boolean gSinelonCycle = true;
uint8_t gSinelonHue = 0; 
// uint8_t gSinelonBeatsPerMinute = 62; 
uint8_t gSinelonDelay = 8; 

void fSinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( gLeds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  gLeds[pos] += CHSV( gSinelonHue, 255, 192);

  // FastLED.show();  
  // // insert a delay to keep the framerate modest
  // FastLED.delay(gPatternDelayloop); 

  if (gSinelonCycle) gSinelonHue++; 
  FastLED.delay(gSinelonDelay);
}

// *****************************************
// Cycle
// *****************************************

String setSinelonCycle(String value) {
  gSinelonCycle = value.toInt();
  storageWrite("sinelonCycle");
  broadcastInt("sinelonCycle", gSinelonCycle);
  return String(gSinelonCycle);
}

String getSinelonCycle() {
  return String(gSinelonCycle);
}

// *****************************************
// Delay
// *****************************************

String setSinelonDelay(String value) {
  gSinelonDelay = value.toInt();
  storageWrite("sinelonDelay");
  broadcastInt("sinelonDelay", gSinelonDelay);
  return String(gSinelonDelay);
}

String getSinelonDelay() {
  return String(gSinelonDelay);
}
