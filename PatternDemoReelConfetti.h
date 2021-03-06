// *****************************************
// Demo Reel Confetti
// *****************************************

boolean gConfettiCycle = false;
uint8_t gConfettiHue = 170; 
uint8_t gConfettiNumberDots = 3; 
uint8_t gConfettiDelay = 50; 

void fConfetti()
{
    // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( gLeds, NUM_LEDS, 10);

  for (uint8_t i = 0 ; i < gConfettiNumberDots; i++) {
    int pos = random16(NUM_LEDS);
    gLeds[pos] += CHSV( gConfettiHue + random8(64), 200, 255);
  }

  // do some periodic updates
  if (gConfettiCycle) gConfettiHue++; 
  FastLED.delay(gConfettiDelay);
}

// *****************************************
// Cycle
// *****************************************

String setConfettiCycle(String value) {
  gConfettiCycle = value.toInt();
  storageWrite("confettiCycle");
  broadcastInt("confettiCycle", gConfettiCycle);  
  return String(gConfettiCycle);
}

String getConfettiCycle() {
  return String(gConfettiCycle);
}

// *****************************************
// Hue
// *****************************************

String setConfettiHue(String value) {
  gConfettiHue = value.toInt();
  storageWrite("confettiHue");
  broadcastInt("confettiHue", gConfettiHue);
  return String(gConfettiHue);
}

String getConfettiHue() {
  return String(gConfettiHue);
}

// *****************************************
// Number Dots
// *****************************************

String setConfettiNumberDots(String value) {
  gConfettiNumberDots = value.toInt();
  storageWrite("confettiNumberDots");
  broadcastInt("confettiNumberDots", gConfettiNumberDots);
  return String(gConfettiNumberDots);
}

String getConfettiNumberDots() {
  return String(gConfettiNumberDots);
}

// *****************************************
// Delay
// *****************************************

String setConfettiDelay(String value) {
  gConfettiDelay = value.toInt();
  storageWrite("confettiDelay");
  broadcastInt("confettiDelay", gConfettiDelay);
  return String(gConfettiDelay);
}

String getConfettiDelay() {
  return String(gConfettiDelay);
}
