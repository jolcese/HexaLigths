// *****************************************
// Palette Edge
// *****************************************


boolean gPaletteEdgeCycle = true;
uint8_t gPaletteEdgeDelay = 8; 
TBlendType gPaletteEdgeBlending = LINEARBLEND; 
uint8_t gPaletteEdgePaletteIndex = 3; 

void fPaletteEdge()
{
  static uint8_t startIndex = 0;
  const uint8_t brightness = 255;

  float hueDelta;
  static uint8_t rotateLocal = 0;
  
  // hueDelta = (float) 255 / (EDGE_LEDS - 1);
  hueDelta = (float) 256 / EDGE_LEDS;

  if (gPaletteEdgeCycle == true) startIndex++; /* motion speed */

  // if (gPaletteEdgeCycle == true) {
  //   rotateLocal++;
  //   if (rotateLocal > EDGE_LEDS - 1) rotateLocal = 0;
  // }


  for(uint8_t dot = 0; dot < EDGE_LEDS; dot++) { 

    // uint16_t idx = dot + rotateLocal;
    // if (idx > EDGE_LEDS - 1) idx -= EDGE_LEDS;
    // gLeds[around[dot]] = CHSV((hueDelta * idx), 255, 255);
    gLeds[around[dot]] = ColorFromPalette( palettes[gPaletteEdgePaletteIndex], (hueDelta * (uint8_t)(dot + startIndex)), brightness, gColorPaletteBlending);

  }

  FastLED.delay(gPaletteEdgeDelay);

}

// *****************************************
// Cycle
// *****************************************

String setPaletteEdgeCycle(String value) {
  gPaletteEdgeCycle = value.toInt();
  storageWrite("paletteEdgeCycle");
  broadcastInt("paletteEdgeCycle", gPaletteEdgeCycle);
  return String(gPaletteEdgeCycle);
}

String getPaletteEdgeCycle() {
  return String(gPaletteEdgeCycle);
}

// *****************************************
// Delay
// *****************************************

String setPaletteEdgeDelay(String value) {
  gPaletteEdgeDelay = value.toInt();
  storageWrite("paletteEdgeDelay");
  broadcastInt("paletteEdgeDelay", gPaletteEdgeDelay);
  return String(gPaletteEdgeDelay);
}

String getPaletteEdgeDelay() {
  return String(gPaletteEdgeDelay);
}

// *****************************************
// Palette
// *****************************************

String setPaletteEdgePalette(String value)
{
  gPaletteEdgePaletteIndex = value.toInt();;

  if (gPaletteEdgePaletteIndex >= PALETTE_COUNT)
    gPaletteEdgePaletteIndex = 0;

  storageWrite("paletteEdgePalette");
  broadcastInt("paletteEdgePalette", gPaletteEdgePaletteIndex);
  return String(gPaletteEdgePaletteIndex);

}
String getPaletteEdgePalette() {
  return String(gPaletteEdgePaletteIndex);
}
