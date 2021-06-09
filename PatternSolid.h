// *****************************************
// Solid Controlled
// *****************************************

uint8_t gSolidRed = 0; 
uint8_t gSolidGreen = 0; 
uint8_t gSolidBlue = 255; 

void fSolid() 
{
  fill_solid(gLeds, NUM_LEDS, CRGB(gSolidRed, gSolidGreen, gSolidBlue));

  FastLED.delay(10);

}

// *****************************************
// Red
// *****************************************

String setSolidRed(String value) {
  gSolidRed = value.toInt();
  storageWrite("solidRed");
  broadcastInt("solidRed", gSolidRed);
  return String(gSolidRed);
}

String getSolidRed() {
  return String(gSolidRed);
}

// *****************************************
// Green
// *****************************************

String setSolidGreen(String value) {
  gSolidGreen = value.toInt();
  storageWrite("solidGreen");
  broadcastInt("solidGreen", gSolidGreen);
  return String(gSolidGreen);
}

String getSolidGreen() {
  return String(gSolidGreen);
}

// *****************************************
// Blue
// *****************************************

String setSolidBlue(String value) {
  gSolidBlue = value.toInt();
  storageWrite("solidBlue");
  broadcastInt("solidBlue", gSolidBlue);
  return String(gSolidBlue);
}

String getSolidBlue() {
  return String(gSolidBlue);
}
