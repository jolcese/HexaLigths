// *****************************************
// Solid
// *****************************************

boolean gSolidEffectsCycleBackground = true;
boolean gSolidEffectsDot = true;
boolean gSolidEffectsDotRandom = false;
uint8_t gSolidEffectsDelay = 20; 

void fSolidEffects() 
{
  static int16_t edge_index = 0;
  static boolean clockwise_dir = false;
  static uint8_t localHue = 0; 

  // param_1 = 3 makes random dot directon
  if (gSolidEffectsDotRandom == true && random(100) < 10) { 
    clockwise_dir = !clockwise_dir;
  }
  if (clockwise_dir == true) {
    edge_index--;
  }
  else 
  {
    edge_index++;
  }

  if (edge_index >= EDGE_LEDS) {
    edge_index = 0;
  }
  if (edge_index < 0) {
    edge_index = EDGE_LEDS - 1;
  }

  fill_solid(gLeds, NUM_LEDS, CHSV(localHue, 255, 255));

  // param_1 = 2 or 3 makes dot loop around the edge
  if (gSolidEffectsDot == true) {
    gLeds[around[edge_index]] = CHSV(localHue + 128, 255, 255);
  }

  if (gSolidEffectsCycleBackground) localHue++;

  FastLED.delay(gSolidEffectsDelay);

}

// *****************************************
// Cycle
// *****************************************

String setSolidEffectsCycle(String value) {
 gSolidEffectsCycleBackground = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gSolidEffectsCycleBackground);
}

String getSolidEffectsCycle() {
  return String(gSolidEffectsCycleBackground);
}

// *****************************************
// Dot
// *****************************************

String setSolidEffectsDot(String value) {
 gSolidEffectsDot = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gSolidEffectsDot);
}

String getSolidEffectsDot() {
  return String(gSolidEffectsDot);
}

// *****************************************
// Dot random
// *****************************************

String setSolidEffectsDotRandom(String value) {
 gSolidEffectsDotRandom = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gSolidEffectsDotRandom);
}

String getSolidEffectsDotRandom() {
  return String(gSolidEffectsDotRandom);
}

// *****************************************
// Delay
// *****************************************

String setSolidEffectsDelay(String value) {
 gSolidEffectsDelay = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gSolidEffectsDelay);
}

String getSolidEffectsDelay() {
  return String(gSolidEffectsDelay);
}