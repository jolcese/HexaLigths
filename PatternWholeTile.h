// *****************************************
// Whole Tile
// *****************************************

boolean gWholeTileCycle = true;
uint8_t gWholeTileHue = 0; 
uint8_t gWholeTileStep = 12; 
uint8_t gWholeTileDelay = 40; 

void fWholeTile() 
{
  for(uint8_t hex = 0; hex < NUM_HEX; hex++) { 
    for(uint8_t dot = 0; dot < NUM_LEDS_PER_HEX; dot++) { 
      gLeds[dot + hex * NUM_LEDS_PER_HEX] = CHSV((gWholeTileHue + gWholeTileStep * hex), 255, 255);
    }
  }

  if (gWholeTileCycle) gWholeTileHue++;

  FastLED.delay(gWholeTileDelay);

}

// *****************************************
// Cycle
// *****************************************

String setWholeTileCycle(String value) {
 gWholeTileCycle = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gWholeTileCycle);
}

String getWholeTileCycle() {
  return String(gWholeTileCycle);
}

// *****************************************
// Hue
// *****************************************

String setWholeTileHue(String value) {
 gWholeTileHue = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gWholeTileHue);
}

String getWholeTileHue() {
  return String(gWholeTileHue);
}

// *****************************************
// Step
// *****************************************

String setWholeTileStep(String value) {
 gWholeTileStep = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gWholeTileStep);
}

String getWholeTileStep() {
  return String(gWholeTileStep);
}

// *****************************************
// Delay
// *****************************************

String setWholeTileDelay(String value) {
 gWholeTileDelay = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gWholeTileDelay);
}

String getWholeTileDelay() {
  return String(gWholeTileDelay);
}