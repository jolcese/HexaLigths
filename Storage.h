// *****************************************
// Storage
// *****************************************

uint8_t storageWrite(uint8_t storageItem, uint8_t value) {
  EEPROM.write(storageItem, value);
  EEPROM.commit();

  Serial.printf("EEPROM.write(%d, %d)", storageItem, value);
  Serial.println();
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

    setPower(String(EEPROM.read(STORAGE_POWER)));
    setBrightness(String(EEPROM.read(STORAGE_BRIGHTNESS)));
    setPattern(String(EEPROM.read(STORAGE_PATTERN)));
    setAutoplay(String(EEPROM.read(STORAGE_AUTOPLAY)));
    setAutoplaySeconds(String(EEPROM.read(STORAGE_AUTOPLAY_SECONDS)));
    setPalette(String(EEPROM.read(STORAGE_PALETTE)));

  } else {
    storageWrite(STORAGE_POWER, getPower().toInt());
    storageWrite(STORAGE_BRIGHTNESS, getBrightness().toInt());
    storageWrite(STORAGE_PATTERN, getPattern().toInt());
    storageWrite(STORAGE_AUTOPLAY, getAutoplay().toInt());
    storageWrite(STORAGE_AUTOPLAY_SECONDS, getAutoplaySeconds().toInt());
    storageWrite(STORAGE_PALETTE, getPalette().toInt());

    storageWrite(STORAGE_INIT, STORAGE_MAGICAL);    
  }
}

