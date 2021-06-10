// *****************************************
// Console Helper Functions
// *****************************************

void print_help() {
  Serial.println();
  Serial.println("HexaLights v1");
  Serial.println("------------");
  Serial.println(" Z - Previous Pattern");
  Serial.println(" X - Next Pattern");
  Serial.println(" A - Decrease Brightness");
  Serial.println(" S - Increase Brightness");
  Serial.println(" C - Current Status");
  Serial.println(" H - Help");
  Serial.println();
}

void print_system_status() {
  Serial.println();
  Serial.println("System Status");
  Serial.println("-------------");
  Serial.print( F("Heap: ") ); Serial.println(system_get_free_heap_size());
  //Serial.print( F("Boot Vers: ") ); Serial.println(system_get_boot_version());
  //Serial.print( F("CPU: ") ); Serial.println(system_get_cpu_freq());
  Serial.print( F("SDK: ") ); Serial.println(system_get_sdk_version());
  //Serial.print( F("Chip ID: ") ); Serial.println(system_get_chip_id());
  //Serial.print( F("Flash ID: ") ); Serial.println(spi_flash_get_id());
  //Serial.print( F("Flash Size: ") ); Serial.println(ESP.getFlashChipRealSize());
  //Serial.print( F("Vcc: ") ); Serial.println(ESP.getVcc());
  Serial.println();
}
void print_led_status() {
  Serial.println();
  Serial.println("Led Status");
  Serial.println("----------");  Serial.print("Pattern = ");
  Serial.print(gPatternIndex);
  Serial.print(" - ");
  Serial.println(g_patterns[gPatternIndex].label);
  Serial.print("Brightness = ");
  Serial.println(gBrightness);
  Serial.print("FPS = ");
  Serial.println(FastLED.getFPS());
  Serial.println();
}

void consoleLoop() {
    while (Serial.available() > 0) { 

    int incomingByte = Serial.read();
    if (incomingByte != 10) Serial.println(char(incomingByte));
    
    switch (incomingByte) {
      // Print help
      case 'H':
      case 'h':
        print_help();
        break;

      // Show status
      case 'C':
      case 'c':
        print_led_status();
        break;

      // Previous pattern
      case 'Z':
      case 'z':
        if (gPatternIndex == 0) {
          gPatternIndex = PATTERNS_TOTAL - 1;
        } else {
          gPatternIndex--;        
        }
        Serial.print(gPatternIndex);
        Serial.print(" - ");
        Serial.println(g_patterns[gPatternIndex].name);
        break;

      // Next pattern
      case 'X':
      case 'x':
        if (gPatternIndex == PATTERNS_TOTAL - 1) {
          gPatternIndex = 0;
        } else {
          gPatternIndex++;  
        }
        Serial.print(gPatternIndex);
        Serial.print(" - ");
        Serial.println(g_patterns[gPatternIndex].name);
        break;

      // Decrease Brigthness
      case 'A':
      case 'a':
        gBrightness = FastLED.getBrightness() * (1 - BRIGHTNESS_STEP);
        if (gBrightness < 0) gBrightness = 0;
        Serial.print("Brightness = ");
        Serial.println(gBrightness);
        FastLED.setBrightness( (uint8_t) gBrightness );
        break;

      // Increase Brightness
      case 'S':
      case 's':
        if ((uint8_t)gBrightness == (uint8_t)(FastLED.getBrightness() * (1 + BRIGHTNESS_STEP))) {
          gBrightness++;
        } else {
          gBrightness = FastLED.getBrightness() * (1 + BRIGHTNESS_STEP);          
        }
        if (gBrightness > BRIGTHNESS_FULL) gBrightness = BRIGTHNESS_FULL;
        Serial.print("Brightness = ");
        Serial.println(gBrightness);
        FastLED.setBrightness( (uint8_t) gBrightness );
        break;

    }
    if (incomingByte != 10) {
      Serial.print("> ");
    }
  }
}

