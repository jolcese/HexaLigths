// *****************************************
// 3 - Demo Reel 100
// *****************************************

// #define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
// //#define FRAMES_PER_SECOND  120

// // List of patterns to cycle through.  Each is defined as a separate function below.
// typedef void (*SimplePatternList[])();
// SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

 uint8_t gHue = 0; // rotating "base color" used by many of the patterns
// uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( g_leds, NUM_LEDS, gHue, 7);

  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(g_pattern_delayloop); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  // FastLED's built-in rainbow generator
  fill_rainbow( g_leds, NUM_LEDS, gHue, 7);
  addGlitter(80);

  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(g_pattern_delayloop); 
  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    g_leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( g_leds, NUM_LEDS, 10);

  for (uint8_t i = 0 ; i < g_pattern_parameter_2; i++) {
    int pos = random16(NUM_LEDS);
    g_leds[pos] += CHSV( gHue + random8(64), 200, 255);
  }

  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(g_pattern_delayloop); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( g_leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  g_leds[pos] += CHSV( gHue, 255, 192);

  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(g_pattern_delayloop); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( g_leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    g_leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(g_pattern_delayloop); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    g_leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(g_pattern_delayloop); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}
