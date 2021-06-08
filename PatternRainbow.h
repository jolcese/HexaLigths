// *****************************************
// Rainbow
// *****************************************

uint8_t gRainbowDelay = 100; 
boolean gRainbowCycle = true;
boolean gRainbowIdenticalTiles = true;

void fRainbow()
{
  uint8_t numTiles;
  uint8_t numLeds;
  float hueDelta;
  static uint8_t rotateLocal = 0;
  
  if (gRainbowIdenticalTiles == true) {
    numTiles = NUM_HEX;
    hueDelta = (float) 255 / (NUM_LEDS_PER_HEX - 1);
    numLeds = NUM_LEDS_PER_HEX;
  }
  else
  {
    numTiles = 1;
    hueDelta = (float) 255 / (NUM_LEDS - 1);
    numLeds = NUM_LEDS;    
  }

  if (gRainbowCycle == true) {
    rotateLocal++;
    if (rotateLocal > numLeds - 1) rotateLocal = 0;
  }
  
  for(uint8_t hex = 0; hex < numTiles; hex++) { 
    for(uint8_t dot = 0; dot < numLeds; dot++) { 

      uint16_t idx = dot + rotateLocal;
      if (idx > numLeds - 1) idx -= numLeds;
      gLeds[dot + hex * numLeds] = CHSV((hueDelta * idx), 255, 255);
    }
  }
  FastLED.delay(gRainbowDelay);

}

// *****************************************
// Cycle
// *****************************************

String setRainbowCycle(String value) {
 gRainbowCycle = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gRainbowCycle);
}

String getRainbowCycle() {
  return String(gRainbowCycle);
}

// *****************************************
// Identical Tiles
// *****************************************

String setRainbowIdenticalTiles(String value) {
 gRainbowIdenticalTiles = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gRainbowIdenticalTiles);
}

String getRainbowIdenticalTiles() {
  return String(gRainbowIdenticalTiles);
}

// *****************************************
// Delay
// *****************************************

String setRainbowDelay(String value) {
 gRainbowDelay = value.toInt();
  //storageWrite(STORAGE_PATTERN_CYCLE, gPowerLed);
  return String(gRainbowDelay);
}

String getRainbowDelay() {
  return String(gRainbowDelay);
}
