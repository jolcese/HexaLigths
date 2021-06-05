#include <EEPROM.h>

#define STORAGE_MAGICAL         166

#define STORAGE_INIT              0
#define STORAGE_POWER             1
#define STORAGE_BRIGHTNESS        2
#define STORAGE_PATTERN           3
#define STORAGE_PATTERN_DELAY     4 
#define STORAGE_PATTERN_PARAM1    5
#define STORAGE_PATTERN_PARAM2    6
#define STORAGE_PATTERN_PARAM3    7
#define STORAGE_PATTERN_PARAM4    8
#define STORAGE_PATTERN_CYCLE     9
#define STORAGE_PATTERN_IDENT    10

uint8_t storageWrite(uint8_t storageItem, uint8_t value) {
  EEPROM.write(storageItem, value);
  Serial.printf("EEPROM.write(%d, %d)", storageItem, value);
  Serial.println();

  EEPROM.commit();
}

uint8_t storageRead(uint8_t storageItem) {
  uint8_t value = EEPROM.read(storageItem);
  Serial.printf("EEPROM.read(%d) = %d", storageItem, value);
  Serial.println();
  return value;
}

void storageLoadSettings() {
  EEPROM.begin(512);

  if (storageRead(STORAGE_INIT) == STORAGE_MAGICAL) {

    g_power = EEPROM.read(STORAGE_POWER);
    g_brightness = EEPROM.read(STORAGE_BRIGHTNESS);

    g_pattern_index = EEPROM.read(STORAGE_PATTERN);
    g_pattern_delayloop = EEPROM.read(STORAGE_PATTERN_DELAY);
    g_pattern_parameter_1 = EEPROM.read(STORAGE_PATTERN_PARAM1);
    g_pattern_parameter_2 = EEPROM.read(STORAGE_PATTERN_PARAM2);
    g_pattern_parameter_3 = EEPROM.read(STORAGE_PATTERN_PARAM3);
    g_pattern_parameter_4 = EEPROM.read(STORAGE_PATTERN_PARAM4);
    g_pattern_index = EEPROM.read(STORAGE_PATTERN);

    g_cycle = EEPROM.read(STORAGE_PATTERN_CYCLE);
    g_identical_tiles = EEPROM.read(STORAGE_PATTERN_IDENT);

  } else {
    storageWrite(STORAGE_POWER, g_power);
    storageWrite(STORAGE_BRIGHTNESS, g_brightness);
    storageWrite(STORAGE_PATTERN, g_pattern_index);
    storageWrite(STORAGE_PATTERN_PARAM1, g_pattern_parameter_1);
    storageWrite(STORAGE_PATTERN_PARAM2, g_pattern_parameter_2);
    storageWrite(STORAGE_PATTERN_PARAM3, g_pattern_parameter_3);
    storageWrite(STORAGE_PATTERN_PARAM4, g_pattern_parameter_4);
    storageWrite(STORAGE_PATTERN_CYCLE, g_cycle);
    storageWrite(STORAGE_PATTERN_IDENT, g_identical_tiles);

    storageWrite(STORAGE_INIT, STORAGE_MAGICAL);    
  }
}

