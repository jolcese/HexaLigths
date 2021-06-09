// *****************************************
// Storage Header
// *****************************************

#include <EEPROM.h>

#define STORAGE_MAGICAL           163
#define STORAGE_INIT                0

uint8_t storageWrite(String name);
uint8_t storageRead(String name);
uint8_t setValueFromStorage(String name);
uint8_t initStorageFromValue(String name);
void storageLoadSettings();
