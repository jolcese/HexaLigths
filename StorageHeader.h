#include <EEPROM.h>

#define STORAGE_MAGICAL           165

#define STORAGE_INIT                0
#define STORAGE_POWER               1
#define STORAGE_BRIGHTNESS          2
#define STORAGE_PATTERN             3
#define STORAGE_AUTOPLAY            4 
#define STORAGE_AUTOPLAY_SECONDS    5
#define STORAGE_PALETTE             6

uint8_t storageWrite(uint8_t storageItem, uint8_t value);
uint8_t storageRead(uint8_t storageItem);
void storageLoadSettings();