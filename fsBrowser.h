//holds the current upload
File fsUploadFile;

// //format bytes
// String formatBytes(size_t bytes){
//   if (bytes < 1024){
//     return String(bytes)+"B";
//   } else if(bytes < (1024 * 1024)){
//     return String(bytes/1024.0)+"KB";
//   } else if(bytes < (1024 * 1024 * 1024)){
//     return String(bytes/1024.0/1024.0)+"MB";
//   } else {
//     return String(bytes/1024.0/1024.0/1024.0)+"GB";
//   }
// }

String getContentType(String filename){
  if(g_webServer.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void fsBrowserHandleStatus(){
  Serial.println("fsBrowserHandleStatus:");
  // FSInfo fs_info;
  String json;
  json.reserve(128);

  json = "{\"type\":\"";
  // json += fsName;
  json += "SPIFFS";
  json += "\", \"isOk\":";
  // if (fsOK) {
  //   fileSystem->info(fs_info);
    json += F("\"true\", \"totalBytes\":\"");
    json += SPIFFS.totalBytes();
    json += F("\", \"usedBytes\":\"");
    json += SPIFFS.usedBytes();
    json += "\"";
  // } else {
  //   json += "\"false\"";
  // }
  json += F(",\"unsupportedFiles\":\"");
  // json += unsupportedFiles;
  // json += "NA";
  json += "\"}";

  g_webServer.send(200, "application/json", json);
}


bool fsBrowserHandleFileRead(String path){
  Serial.println("fsBrowserHandleFileRead: " + path);
  if(path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = g_webServer.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void fsBrowserHandleFileUpload(){
  if(g_webServer.uri() != "/edit") return;
  HTTPUpload& upload = g_webServer.upload();
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Serial.print("fsBrowserHandleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if(upload.status == UPLOAD_FILE_WRITE){
    //Serial.print("fsBrowserHandleFileUpload Data: "); Serial.println(upload.currentSize);
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile)
      fsUploadFile.close();
    Serial.print("fsBrowserHandleFileUpload Size: "); Serial.println(upload.totalSize);
  }
}

void fsBrowserHandleFileDelete(){
  if(g_webServer.args() == 0) return g_webServer.send(500, "text/plain", "BAD ARGS");
  String path = g_webServer.arg(0);
  Serial.println("fsBrowserHandleFileDelete: " + path);
  if(path == "/")
    return g_webServer.send(500, "text/plain", "BAD PATH");
  if(!SPIFFS.exists(path))
    return g_webServer.send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
  g_webServer.send(200, "text/plain", "");
  path = String();
}

void fsBrowserHandleFileCreate(){
  if(g_webServer.args() == 0)
    return g_webServer.send(500, "text/plain", "BAD ARGS");
  String path = g_webServer.arg(0);
  Serial.println("fsBrowserHandleFileCreate: " + path);
  if(path == "/")
    return g_webServer.send(500, "text/plain", "BAD PATH");
  if(SPIFFS.exists(path))
    return g_webServer.send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
  if(file)
    file.close();
  else
    return g_webServer.send(500, "text/plain", "CREATE FAILED");
  g_webServer.send(200, "text/plain", "");
  path = String();
}

void fsBrowserHandleFileList() {
  if(!g_webServer.hasArg("dir")) {g_webServer.send(500, "text/plain", "BAD ARGS"); return;}
  
  String path = g_webServer.arg("dir");
  Serial.println("fsBrowserHandleFileList: " + path);
  
  File root = SPIFFS.open(path);

  File file = root.openNextFile();

  String output = "[";
  while(file) {
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir)?"dir":"file";
    output += "\",\"name\":\"";
    output += String(file.name()).substring(1);
    output += "\",\"size\":\"";
    output += String(file.size());
    output += "\"}";
    file.close();
    
    file = root.openNextFile();
  }  
  output += "]";
  g_webServer.send(200, "text/json", output);
}
