// *****************************************
// Twinkles
// *****************************************

uint8_t gTwinklesDelay = 30; 
uint8_t gTwinklesDensity = 255; 
uint8_t gTwinklesFadeIn = 32; 
uint8_t gTwinklesFadeOut = 20; 
uint8_t gTwinklesPaletteIndex = 15; 

#define STARTING_BRIGHTNESS 64
// #define FADE_IN_SPEED       32
// #define FADE_OUT_SPEED      20
// #define DENSITY            255

enum { GETTING_DARKER = 0, GETTING_BRIGHTER = 1 };

CRGB makeBrighter( const CRGB& color, fract8 howMuchBrighter)
{
  CRGB incrementalColor = color;
  incrementalColor.nscale8( howMuchBrighter);
  return color + incrementalColor;
}

CRGB makeDarker( const CRGB& color, fract8 howMuchDarker)
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
uint8_t  twinklesDirectionFlags[ (NUM_LEDS + 7) / 8];

bool getPixelDirection( uint16_t i)
{
  uint16_t index = i / 8;
  uint8_t  bitNum = i & 0x07;

  uint8_t  andMask = 1 << bitNum;
  return (twinklesDirectionFlags[index] & andMask) != 0;
}

void setPixelDirection( uint16_t i, bool dir)
{
  uint16_t index = i / 8;
  uint8_t  bitNum = i & 0x07;

  uint8_t  orMask = 1 << bitNum;
  uint8_t andMask = 255 - orMask;
  uint8_t value = twinklesDirectionFlags[index] & andMask;
  if ( dir ) {
    value += orMask;
  }
  twinklesDirectionFlags[index] = value;
}

void brightenOrDarkenEachPixel( fract8 fadeUpAmount, fract8 fadeDownAmount)
{
  for ( uint16_t i = 0; i < NUM_LEDS; i++) {
    if ( getPixelDirection(i) == GETTING_DARKER) {
      // This pixel is getting darker
      gLeds[i] = makeDarker( gLeds[i], fadeDownAmount);
    } else {
      // This pixel is getting brighter
      gLeds[i] = makeBrighter( gLeds[i], fadeUpAmount);
      // now check to see if we've maxxed out the brightness
      if ( gLeds[i].r == 255 || gLeds[i].g == 255 || gLeds[i].b == 255) {
        // if so, turn around and start getting darker
        setPixelDirection(i, GETTING_DARKER);
      }
    }
  }
}

void fTwinkles()
{
    // Make each pixel brighter or darker, depending on
    // its 'direction' flag.
    brightenOrDarkenEachPixel( gTwinklesFadeIn, gTwinklesFadeOut);

    // Now consider adding a new random twinkle
    if ( random8() < gTwinklesDensity ) {
        int pos = random16(NUM_LEDS);
        if ( !gLeds[pos]) {
        gLeds[pos] = ColorFromPalette( palettes[gTwinklesPaletteIndex], random8(), STARTING_BRIGHTNESS, NOBLEND);
        setPixelDirection(pos, GETTING_BRIGHTER);
        }
    }
    FastLED.delay(gTwinklesDelay);

}

// *****************************************
// Delay
// *****************************************

String setTwinklesDelay(String value) {
  gTwinklesDelay = value.toInt();
  storageWrite("twinklesDelay");
  broadcastInt("twinklesDelay", gWholeTileDelay);
  return String(gTwinklesDelay);
}

String getTwinklesDelay() {
  return String(gTwinklesDelay);
}

// *****************************************
// Palette
// *****************************************

String setTwinklesPalette(String value)
{
  gTwinklesPaletteIndex = value.toInt();;

  if (gTwinklesPaletteIndex >= PALETTE_COUNT)
    gTwinklesPaletteIndex = 0;

  storageWrite("twinklesPalette");
  broadcastInt("twinklesPalette", gTwinklesPaletteIndex);
  return String(gTwinklesPaletteIndex);

}
String getTwinklesPalette() {
  return String(gTwinklesPaletteIndex);
}

// *****************************************
// Fade In Speed
// *****************************************

String setTwinklesFadeIn(String value) {
  gTwinklesFadeIn = value.toInt();
  storageWrite("twinklesFadeIn");
  broadcastInt("twinklesFadeIn", gTwinklesFadeIn);
  return String(gTwinklesFadeIn);
}

String getTwinklesFadeIn() {
  return String(gTwinklesFadeIn);
}

// *****************************************
// Fade Out Speed
// *****************************************

String setTwinklesFadeOut(String value) {
  gTwinklesFadeOut = value.toInt();
  storageWrite("twinklesFadeOut");
  broadcastInt("twinklesFadeOut", gTwinklesFadeOut);
  return String(gTwinklesFadeOut);
}

String getTwinklesFadeOut() {
  return String(gTwinklesFadeOut);
}

// *****************************************
// Density
// *****************************************

String setTwinklesDensity(String value) {
  gTwinklesDensity = value.toInt();
  storageWrite("twinklesDensity");
  broadcastInt("twinklesDensity", gTwinklesDensity);
  return String(gTwinklesDensity);
}

String getTwinklesDensity() {
  return String(gTwinklesDensity);
}

