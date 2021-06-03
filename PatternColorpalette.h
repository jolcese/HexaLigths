
// *****************************************
// 2 - Color Palette
// *****************************************

extern uint8_t g_pattern_parameter_1;

void ChangePalette(uint8_t param);
void FillLEDsFromPaletteColors( uint8_t colorIndex);
void SetupPurpleAndGreenPalette();
void SetupTotallyRandomPalette();
void SetupBlackAndWhiteStripedPalette();


CRGBPalette16 currentPalette = RainbowColors_p;
TBlendType currentBlending = LINEARBLEND;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
  
void color_palette()
{
  static uint8_t startIndex = 0;

  ChangePalette(g_pattern_parameter_1);
 
  
  // slowly cycle the "base color" through the rainbow
  // EVERY_N_MILLISECONDS( g_pattern_delayloop ) {
    if (g_cycle == true) startIndex++; /* motion speed */
    FillLEDsFromPaletteColors(startIndex);
  // } 

  // FastLED.show();
  // FastLED.delay(g_pattern_delayloop);
 }

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  uint8_t n_tiles;
  uint8_t n_leds;
  uint8_t cidx;
  float gfHueDelta;

  if (g_repeat == true) {
    n_tiles = NUM_HEX;
    n_leds = NUM_LEDS_PER_HEX;
    gfHueDelta = (float) 255 / (NUM_LEDS_PER_HEX - 1);
  }
  else
  {
    n_tiles = 1;
    n_leds = NUM_LEDS;    
    gfHueDelta = (float) 255 / (NUM_LEDS - 1);
  }
  
  for(uint8_t hex = 0; hex < n_tiles; hex++) { 
    cidx = colorIndex;
    for(uint8_t dot = 0; dot < n_leds; dot++) { 
      g_leds[dot + hex * n_leds] = ColorFromPalette( currentPalette, (gfHueDelta * (uint8_t)(dot + colorIndex)), brightness, currentBlending);
//      Serial.print(dot + hex * n_leds);
//      Serial.print('-');
//      Serial.print((gfHueDelta * (uint8_t)(dot + colorIndex)));
//      Serial.print(' / ');

    }
  }
//  Serial.println();
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalette(uint8_t param)
{
  if( param ==  1)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
  if( param ==  2)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
  if( param ==  3)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
  if( param ==  4)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
  if( param ==  5)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
  if( param ==  6)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
  if( param ==  7)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
  if( param ==  8)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
  if( param ==  9)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
  if( param == 10)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
  if( param == 11)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
