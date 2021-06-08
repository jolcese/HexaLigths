// *****************************************
// Color Palette
// *****************************************

uint8_t gColorPaletteDelay = 30; 
boolean gColorPaletteCycle = true;
boolean gColorPaletteIdenticalTiles = true;
TBlendType gColorPaletteBlending = LINEARBLEND; 

void fColorPalette()
{
  static uint8_t startIndex = 0;
  const uint8_t brightness = 255;
  uint8_t numTiles;
  uint8_t numLeds;
  float hueDelta;
 
  if (gColorPaletteCycle == true) startIndex++; /* motion speed */

  if (gColorPaletteIdenticalTiles == true) {
    numTiles = NUM_HEX;
    numLeds = NUM_LEDS_PER_HEX;
    hueDelta = (float) 255 / (NUM_LEDS_PER_HEX - 1);
  }
  else
  {
    numTiles = 1;
    numLeds = NUM_LEDS;    
    hueDelta = (float) 255 / (NUM_LEDS - 1);
  }
  
  for(uint8_t hex = 0; hex < numTiles; hex++) { 
    // cidx = startIndex;
    for(uint8_t dot = 0; dot < numLeds; dot++) { 
      gLeds[dot + hex * numLeds] = ColorFromPalette( palettes[gPaletteIndex], (hueDelta * (uint8_t)(dot + startIndex)), brightness, gColorPaletteBlending);
    }
  }

  FastLED.delay(gColorPaletteDelay);
 }

// *****************************************
// Cycle
// *****************************************

String setColorPaletteCycle(String value) {
 gColorPaletteCycle = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gColorPaletteCycle);
}

String getColorPaletteCycle() {
  return String(gColorPaletteCycle);
}

// *****************************************
// Identical Tiles
// *****************************************

String setColorPaletteIdenticalTiles(String value) {
 gColorPaletteIdenticalTiles = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gColorPaletteIdenticalTiles);
}

String getColorPaletteIdenticalTiles() {
  return String(gColorPaletteIdenticalTiles);
}

// *****************************************
// Delay
// *****************************************

String setColorPaletteDelay(String value) {
 gColorPaletteDelay = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gColorPaletteDelay);
}

String getColorPaletteDelay() {
  return String(gColorPaletteDelay);
}

