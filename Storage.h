#include <EEPROM.h>

#define STORAGE_MAGICAL         167

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
  // EEPROM.write(storageItem, value);
  Serial.printf("EEPROM.write(%d, %d)", storageItem, value);
  Serial.println();

  // EEPROM.commit();
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

    gPowerLed = EEPROM.read(STORAGE_POWER);
    gBrightness = EEPROM.read(STORAGE_BRIGHTNESS);

    gPatternIndex = EEPROM.read(STORAGE_PATTERN);
    // gPatternDelayloop = EEPROM.read(STORAGE_PATTERN_DELAY);
    // gPatternParameter_1 = EEPROM.read(STORAGE_PATTERN_PARAM1);
    // gPatternParameter_2 = EEPROM.read(STORAGE_PATTERN_PARAM2);
    // gPatternParameter_3 = EEPROM.read(STORAGE_PATTERN_PARAM3);
    // gPatternParameter_4 = EEPROM.read(STORAGE_PATTERN_PARAM4);
    gPatternIndex = EEPROM.read(STORAGE_PATTERN);

    // gCycle = EEPROM.read(STORAGE_PATTERN_CYCLE);
    // gIdenticalTiles = EEPROM.read(STORAGE_PATTERN_IDENT);

  } else {
    storageWrite(STORAGE_POWER, gPowerLed);
    storageWrite(STORAGE_BRIGHTNESS, gBrightness);
    storageWrite(STORAGE_PATTERN, gPatternIndex);
    // storageWrite(STORAGE_PATTERN_PARAM1, gPatternParameter_1);
    // storageWrite(STORAGE_PATTERN_PARAM2, gPatternParameter_2);
    // storageWrite(STORAGE_PATTERN_PARAM3, gPatternParameter_3);
    // storageWrite(STORAGE_PATTERN_PARAM4, gPatternParameter_4);
    // storageWrite(STORAGE_PATTERN_CYCLE, gCycle);
    // storageWrite(STORAGE_PATTERN_IDENT, gIdenticalTiles);

    storageWrite(STORAGE_INIT, STORAGE_MAGICAL);    
  }
}

