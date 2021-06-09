// *****************************************
// Demo Reel Rainbow
// *****************************************

boolean gDemoReelCycle = true;
uint8_t gDemoReelHue = 0; 
boolean gDemoReelGlitter = true; 
uint8_t gDemoReelGlitterChance = 80; 
uint8_t gDemoReelDelay = 8; 

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    gLeds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void fDemoReel() 
{
  fill_rainbow( gLeds, NUM_LEDS, gDemoReelHue, 7);

  if (gDemoReelGlitter) addGlitter(gDemoReelGlitterChance);

  if (gDemoReelCycle) gDemoReelHue++; 
  FastLED.delay(gDemoReelDelay);
}


// *****************************************
// Cycle
// *****************************************

String setDemoReelCycle(String value) {
  gDemoReelCycle = value.toInt();
  storageWrite("demoReelCycle");
  broadcastInt("demoReelCycle", gDemoReelCycle);
  return String(gDemoReelCycle);
}

String getDemoReelCycle() {
  return String(gDemoReelCycle);
}

// *****************************************
// Hue
// *****************************************

String setDemoReelHue(String value) {
  gDemoReelHue = value.toInt();
  return String(gDemoReelHue);
}

String getDemoReelHue() {
  return String(gDemoReelHue);
}

// *****************************************
// Glitter
// *****************************************

String setDemoReelGlitter(String value) {
  gDemoReelGlitter = value.toInt();
  storageWrite("demoReelGlitter");
  broadcastInt("demoReelGlitter", gDemoReelGlitter);
  return String(gDemoReelGlitter);
}

String getDemoReelGlitter() {
  return String(gDemoReelGlitter);
}

// *****************************************
// Glitter Chance
// *****************************************

String setDemoReelGlitterChance(String value) {
  gDemoReelGlitterChance = value.toInt();
  storageWrite("demoReelGlitterChance");
  broadcastInt("demoReelGlitterChance", gDemoReelGlitterChance);
  return String(gDemoReelGlitterChance);
}

String getDemoReelGlitterChance() {
  return String(gDemoReelGlitterChance);
}

// *****************************************
// Delay
// *****************************************

String setDemoReelDelay(String value) {
  gDemoReelDelay = value.toInt();
  storageWrite("demoReelGlitterDelay");
  broadcastInt("demoReelGlitterDelay", gDemoReelDelay);
  return String(gDemoReelDelay);
}

String getDemoReelDelay() {
  return String(gDemoReelDelay);
}

