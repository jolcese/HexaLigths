// *****************************************
// 0 - Rotating Rainbow
// *****************************************

void rotating_rainbow()
{
  uint8_t n_tiles;
  uint8_t n_leds;
  float gfHueDelta;
  static uint8_t rotateLocal = 0;
  
  if (g_repeat == true) {
    n_tiles = NUM_HEX;
    gfHueDelta = (float) 255 / (NUM_LEDS_PER_HEX - 1);
    n_leds = NUM_LEDS_PER_HEX;
  }
  else
  {
    n_tiles = 1;
    gfHueDelta = (float) 255 / (NUM_LEDS - 1);
    n_leds = NUM_LEDS;    
  }

  if (g_cycle == true) {
    rotateLocal++;
    if (rotateLocal > n_leds - 1) rotateLocal = 0;
  }
  
  for(uint8_t hex = 0; hex < n_tiles; hex++) { 
    for(uint8_t dot = 0; dot < n_leds; dot++) { 

      uint16_t idx = dot + rotateLocal;
      if (idx > n_leds - 1) idx -= n_leds;
      g_leds[dot + hex * n_leds] = CHSV((gfHueDelta * idx), 255, 255);
    }
  }
  FastLED.show();
  delay(g_pattern_delayloop);
}
