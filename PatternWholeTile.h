// *****************************************
// Whole Tile
// *****************************************

void whole_tile() 
{
  uint8_t n_tiles;
  uint8_t n_leds;
  static uint8_t localHue = 0; 
  static uint8_t rotateLocal = 0;

  // EVERY_N_MILLISECONDS( g_pattern_delayloop ) { 

    for(uint8_t hex = 0; hex < NUM_HEX; hex++) { 
      for(uint8_t dot = 0; dot < NUM_LEDS_PER_HEX; dot++) { 
        g_leds[dot + hex * NUM_LEDS_PER_HEX] = CHSV((localHue + g_pattern_parameter_2 * hex), 255, 255);
      }
    }
    localHue++;
  // }

  // FastLED.show();
  // delay(g_pattern_delayloop);

}
