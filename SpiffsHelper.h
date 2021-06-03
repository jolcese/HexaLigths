// *****************************************
// SPIFFS Helper
// *****************************************

void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

void printSpiffsContents() {
  Serial.println();
  Serial.println("SPIFFS Contents");
  Serial.println("---------------");
  
  if(SPIFFS.begin()){
    listDir(SPIFFS, "/", 0);
  } else {
    Serial.println("SPIFFS Mount Failed!");\
  }
  Serial.println();
}    

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
