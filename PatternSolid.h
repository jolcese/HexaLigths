// *****************************************
// Solid
// *****************************************

void solid_background() 
{
  static int16_t edge_index = 0;
  static boolean clockwise_dir = false;
  static uint8_t localHue = 0; 

  // param_1 = 3 makes random dot directon
  if ((g_pattern_parameter_1 == 3) && (random(100) < g_pattern_parameter_2)) { 
    clockwise_dir = !clockwise_dir;
  }
  if (clockwise_dir == true) {
    edge_index--;
  }
  else 
  {
    edge_index++;
  }

  if (edge_index >= EDGE_LEDS) {
    edge_index = 0;
  }
  if (edge_index < 0) {
    edge_index = EDGE_LEDS - 1;
  }

  // param_1 = 1 makes color controlled by param_2
  if (g_pattern_parameter_1 == 1) {
    uint8_t r = g_pattern_parameter_2;
    uint8_t g = g_pattern_parameter_3;
    uint8_t b = g_pattern_parameter_4;
    fill_solid(g_leds, NUM_LEDS, CRGB(r, g, b));
  } else {
    fill_solid(g_leds, NUM_LEDS, CHSV(localHue, 255, 255));
  }
  

  // param_1 = 2 or 3 makes dot loop around the edge
  if (g_pattern_parameter_1 == 2 || g_pattern_parameter_1 == 3) {
    g_leds[around[edge_index]] = CHSV(localHue + 128, 255, 255);
  }

  FastLED.show();  
  delay(g_pattern_delayloop);
  
  EVERY_N_MILLISECONDS( g_pattern_delayloop ) { 
    localHue++; 
  } 

}
