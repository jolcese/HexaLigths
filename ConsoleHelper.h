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
  Serial.println(" Q - Decrease Speed");
  Serial.println(" W - Increase Speed");
  Serial.println(" D - Decrease Pattern Prameter 2");
  Serial.println(" F - Increase Pattern Prameter 2");
  Serial.println(" C - Current Status");
  Serial.println(" R - Repeat pattern on tiles");
  Serial.println(" T - Cycle effect");
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
  Serial.print(g_pattern_index);
  Serial.print(" - ");
  Serial.println(g_patterns[g_pattern_index].name);
  Serial.print("Brightness = ");
  Serial.println(g_brightness);
  Serial.print("Delay = ");
  Serial.println(g_pattern_delayloop);
  Serial.print("Pattern Parameter 1 = ");
  Serial.println(g_pattern_parameter_1);
  Serial.print("Pattern Parameter 2 = ");
  Serial.println(g_pattern_parameter_2);
  Serial.print("Repeat = ");
  Serial.println(g_identical_tiles);
  Serial.print("Cycle = ");
  Serial.println(g_cycle);
  Serial.print("FPS = ");
  Serial.println(FastLED.getFPS());
  Serial.println();
}
