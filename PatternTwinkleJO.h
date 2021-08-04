// *****************************************
// TwinkleJO
// *****************************************

uint8_t gTwinkleJODelay = 30; 
uint8_t gTwinkleJODotsPerCycle = 5; // 1 - 10? 
uint8_t gTwinkleJOSpread = 64; // 1 - 255? 
uint8_t gTwinkleJOFadeIn = 30; 
uint8_t gTwinkleJOFadeOut = 20; 
uint8_t gTwinkleJOStartBrightness = 100; 
uint8_t gTwinkleJOBaseHue = 170; 

enum { TJO_GETTING_DARKER = 0, TJO_GETTING_BRIGHTER = 1 };

CRGB tJOMakeBrighter( const CRGB& color, fract8 howMuchBrighter)
{
  CRGB incrementalColor = color;
  incrementalColor.nscale8( howMuchBrighter);
  return color + incrementalColor;
}

CRGB tJOMakeDarker( const CRGB& color, fract8 howMuchDarker)
{
  CRGB newcolor = color;
  newcolor.nscale8( 255 - howMuchDarker);
  return newcolor;
}

// Compact implementation of
// the directionFlags array, using just one BIT of RAM
// per pixel.  This requires a bunch of bit wrangling,
// but conserves precious RAM.  The cost is a few
// cycles and about 100 bytes of flash program memory.
uint8_t  twinklejoDirectionFlags[ (NUM_LEDS + 7) / 8];

bool tJOgetPixelDirection( uint16_t i)
{
  uint16_t index = i / 8;
  uint8_t  bitNum = i & 0x07;

  uint8_t  andMask = 1 << bitNum;
  return (twinklejoDirectionFlags[index] & andMask) != 0;
}

void tJOsetPixelDirection( uint16_t i, bool dir)
{
  uint16_t index = i / 8;
  uint8_t  bitNum = i & 0x07;

  uint8_t  orMask = 1 << bitNum;
  uint8_t andMask = 255 - orMask;
  uint8_t value = twinklejoDirectionFlags[index] & andMask;
  if ( dir ) {
    value += orMask;
  }
  twinklejoDirectionFlags[index] = value;
}

void tJObrightenOrDarkenEachPixel( fract8 fadeUpAmount, fract8 fadeDownAmount)
{
  for ( uint16_t i = 0; i < NUM_LEDS; i++) {
    if ( tJOgetPixelDirection(i) == TJO_GETTING_DARKER) {
      // This pixel is getting darker
      gLeds[i] = tJOMakeDarker( gLeds[i], fadeDownAmount);
    } else {
      // This pixel is getting brighter
      gLeds[i] = tJOMakeBrighter( gLeds[i], fadeUpAmount);
      // now check to see if we've maxxed out the brightness
      if ( gLeds[i].r == 255 || gLeds[i].g == 255 || gLeds[i].b == 255) {
        // if so, turn around and start getting darker
        tJOsetPixelDirection(i, TJO_GETTING_DARKER);
      }
    }
  }
}

void fTwinkleJO()
{
    // Make each pixel brighter or darker, depending on
    // its 'direction' flag.
    tJObrightenOrDarkenEachPixel( gTwinkleJOFadeIn, gTwinkleJOFadeOut);

    // Now consider adding a new random twinkle
    for ( uint16_t i = 0 ; i < gTwinkleJODotsPerCycle ; i++) {
        int pos = random16(NUM_LEDS);
        if ( !gLeds[pos]) {
            //gLeds[pos] = ColorFromPalette( palettes[gTwinkleJOPaletteIndex], random8(), gTwinkleJOStartBrightness, NOBLEND);

            gLeds[pos] = CHSV( gTwinkleJOBaseHue + random8(gTwinkleJOSpread), 255, gTwinkleJOStartBrightness);
            tJOsetPixelDirection(pos, TJO_GETTING_BRIGHTER);
        }
    }
    FastLED.delay(gTwinkleJODelay);

}

// *****************************************
// Delay
// *****************************************

String setTwinkleJODelay(String value) {
  gTwinkleJODelay = value.toInt();
  storageWrite("twinkleJODelay");
  broadcastInt("twinkleJODelay", gTwinkleJODelay);
  return String(gTwinkleJODelay);
}

String getTwinkleJODelay() {
  return String(gTwinkleJODelay);
}

// *****************************************
// Fade In Speed
// *****************************************

String setTwinkleJOFadeIn(String value) {
  gTwinkleJOFadeIn = value.toInt();
  storageWrite("twinkleJOFadeIn");
  broadcastInt("twinkleJOFadeIn", gTwinkleJOFadeIn);
  return String(gTwinkleJOFadeIn);
}

String getTwinkleJOFadeIn() {
  return String(gTwinkleJOFadeIn);
}

// *****************************************
// Fade Out Speed
// *****************************************

String setTwinkleJOFadeOut(String value) {
  gTwinkleJOFadeOut = value.toInt();
  storageWrite("twinkleJOFadeOut");
  broadcastInt("twinkleJOFadeOut", gTwinkleJOFadeOut);
  return String(gTwinkleJOFadeOut);
}

String getTwinkleJOFadeOut() {
  return String(gTwinkleJOFadeOut);
}

// *****************************************
// StartBrightness
// *****************************************

String setTwinkleJOStartBrightness(String value) {
  gTwinkleJOStartBrightness = value.toInt();
  storageWrite("twinkleJOStartBrightness");
  broadcastInt("twinkleJOStartBrightness", gTwinkleJOStartBrightness);
  return String(gTwinkleJOStartBrightness);
}

String getTwinkleJOStartBrightness() {
  return String(gTwinkleJOStartBrightness);
}

// *****************************************
// DotsPerCycle
// *****************************************

String setTwinkleJODotsPerCycle(String value) {
  gTwinkleJODotsPerCycle = value.toInt();
  storageWrite("twinkleJODotsPerCycle");
  broadcastInt("twinkleJODotsPerCycle", gTwinkleJODotsPerCycle);
  return String(gTwinkleJODotsPerCycle);
}

String getTwinkleJODotsPerCycle() {
  return String(gTwinkleJODotsPerCycle);
}

// *****************************************
// Spread
// *****************************************

String setTwinkleJOSpread(String value) {
  gTwinkleJOSpread = value.toInt();
  storageWrite("twinkleJOSpread");
  broadcastInt("twinkleJOSpread", gTwinkleJOSpread);
  return String(gTwinkleJOSpread);
}

String getTwinkleJOSpread() {
  return String(gTwinkleJOSpread);
}

// *****************************************
// Hue
// *****************************************

String setTwinkleJOBaseHue(String value) {
  gTwinkleJOBaseHue = value.toInt();
  storageWrite("twinkleJOBaseHue");
  broadcastInt("twinkleJOBaseHue", gTwinkleJOBaseHue);
  return String(gTwinkleJOBaseHue);
}

String getTwinkleJOBaseHue() {
  return String(gTwinkleJOBaseHue);
}

