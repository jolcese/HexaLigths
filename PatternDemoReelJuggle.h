// *****************************************
// Demo Reel Juggle
// *****************************************

boolean gJuggleCycle = true;
// uint8_t gJuggleHue = 0; 
// uint8_t gJuggleBeatsPerMinute = 62; 
uint8_t gJuggleDelay = 5; 

void fJuggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( gLeds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    gLeds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }

  FastLED.delay(gJuggleDelay);
}

// *****************************************
// Cycle
// *****************************************

String setJuggleCycle(String value) {
 gJuggleCycle = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gJuggleCycle);
}

String getJuggleCycle() {
  return String(gJuggleCycle);
}

// *****************************************
// Delay
// *****************************************

String setJuggleDelay(String value) {
 gJuggleDelay = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gJuggleDelay);
}

String getJuggleDelay() {
  return String(gJuggleDelay);
}
