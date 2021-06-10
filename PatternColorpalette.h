// *****************************************
// Color Palette
// *****************************************

uint8_t gColorPaletteDelay = 30; 
boolean gColorPaletteCycle = true;
boolean gColorPaletteIdenticalTiles = false;
TBlendType gColorPaletteBlending = LINEARBLEND; 
uint8_t gColorPalettePaletteIndex = 3; 

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
      gLeds[dot + hex * numLeds] = ColorFromPalette( palettes[gColorPalettePaletteIndex], (hueDelta * (uint8_t)(dot + startIndex)), brightness, gColorPaletteBlending);
    }
  }

  FastLED.delay(gColorPaletteDelay);
 }

// *****************************************
// Cycle
// *****************************************

String setColorPaletteCycle(String value) {
  gColorPaletteCycle = value.toInt();
  storageWrite("colorPaletteCycle");
  broadcastInt("colorPaletteCycle", gColorPaletteCycle);
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
  storageWrite("colorPaletteIdenticalTiles");
  broadcastInt("colorPaletteIdenticalTiles", gColorPaletteIdenticalTiles);
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
  storageWrite("colorPaletteDelay");
  broadcastInt("colorPaletteDelay", gColorPaletteDelay);
  return String(gColorPaletteDelay);
}

String getColorPaletteDelay() {
  return String(gColorPaletteDelay);
}

// *****************************************
// Palette
// *****************************************

String setColorPalettePalette(String value)
{
  gColorPalettePaletteIndex = value.toInt();;

  if (gColorPalettePaletteIndex >= PALETTE_COUNT)
    gColorPalettePaletteIndex = 0;

  storageWrite("colorPalettePalette");
  broadcastInt("colorPalettePalette", gColorPalettePaletteIndex);
  return String(gColorPalettePaletteIndex);

}
String getColorPalettePalette() {
  return String(gColorPalettePaletteIndex);
}