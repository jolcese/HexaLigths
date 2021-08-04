// *****************************************
// Network
// *****************************************

void WiFiEventHandler(WiFiEvent_t event, system_event_info_t info)
{
  // Comment the following line to log events
  return;

  char *WifiEvents[] = {
    "SYSTEM_EVENT_WIFI_READY",               /**< ESP32 WiFi ready */
    "SYSTEM_EVENT_SCAN_DONE",                /**< ESP32 finish scanning AP */
    "SYSTEM_EVENT_STA_START",                /**< ESP32 station start */
    "SYSTEM_EVENT_STA_STOP",                 /**< ESP32 station stop */
    "SYSTEM_EVENT_STA_CONNECTED",            /**< ESP32 station connected to AP */
    "SYSTEM_EVENT_STA_DISCONNECTED",         /**< ESP32 station disconnected from AP */
    "SYSTEM_EVENT_STA_AUTHMODE_CHANGE",      /**< the auth mode of AP connected by ESP32 station changed */
    "SYSTEM_EVENT_STA_GOT_IP",               /**< ESP32 station got IP from connected AP */
    "SYSTEM_EVENT_STA_LOST_IP",              /**< ESP32 station lost IP and the IP is reset to 0 */
    "SYSTEM_EVENT_STA_WPS_ER_SUCCESS",       /**< ESP32 station wps succeeds in enrollee mode */
    "SYSTEM_EVENT_STA_WPS_ER_FAILED",        /**< ESP32 station wps fails in enrollee mode */
    "SYSTEM_EVENT_STA_WPS_ER_TIMEOUT",       /**< ESP32 station wps timeout in enrollee mode */
    "SYSTEM_EVENT_STA_WPS_ER_PIN",           /**< ESP32 station wps pin code in enrollee mode */
    "SYSTEM_EVENT_AP_START",                 /**< ESP32 soft-AP start */
    "SYSTEM_EVENT_AP_STOP",                  /**< ESP32 soft-AP stop */
    "SYSTEM_EVENT_AP_STACONNECTED",          /**< a station connected to ESP32 soft-AP */
    "SYSTEM_EVENT_AP_STADISCONNECTED",       /**< a station disconnected from ESP32 soft-AP */
    "SYSTEM_EVENT_AP_STAIPASSIGNED",         /**< ESP32 soft-AP assign an IP to a connected station */
    "SYSTEM_EVENT_AP_PROBEREQRECVED",        /**< Receive probe request packet in soft-AP interface */
    "SYSTEM_EVENT_GOT_IP6",                  /**< ESP32 station or ap or ethernet interface v6IP addr is preferred */
    "SYSTEM_EVENT_ETH_START",                /**< ESP32 ethernet start */
    "SYSTEM_EVENT_ETH_STOP",                 /**< ESP32 ethernet stop */
    "SYSTEM_EVENT_ETH_CONNECTED",            /**< ESP32 ethernet phy link up */
    "SYSTEM_EVENT_ETH_DISCONNECTED",         /**< ESP32 ethernet phy link down */
    "SYSTEM_EVENT_ETH_GOT_IP",               /**< ESP32 ethernet got IP from connected AP */
    "SYSTEM_EVENT_MAX"
  };
  
  Serial.println();
  Serial.printf("Event: %d - %s", event, WifiEvents[event]);
  Serial.println();
  
  switch(event) {
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.printf("Reason: %d\n", info.disconnected.reason);
        break;
    }
  Serial.println();
}

void setupWifiManager() {

  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  sprintf(gHostName, "HexaLigths-%02X%02X", mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 1]);

  Serial.println();
  Serial.println("Wifi Manager Setup");
  Serial.println("------------------");
  Serial.printf("MAC address = %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println();
  Serial.printf("Name: %s\n", gHostName );
  Serial.println();
  
  // reset settings - wipe credentials for testing
  // gWifiManager.resetSettings();
  gWifiManager.setConfigPortalBlocking(false);

  //automatically connect using saved credentials if they exist
  //If connection fails it starts an access point with the specified name
  if(gWifiManager.autoConnect(gHostName)){
    Serial.println("Wi-Fi connected");
    gFirstBoot = false;
  }
  else {
    Serial.println("Wi-Fi manager portal running");
  }
}

void broadcastInt(String name, uint8_t value)
{
  String json = "{\"name\":\"" + name + "\",\"value\":" + String(value) + "}";
  gWebSocketsServer.broadcastTXT(json);
}

void broadcastString(String name, String value)
{
  String json = "{\"name\":\"" + name + "\",\"value\":\"" + String(value) + "\"}";
  gWebSocketsServer.broadcastTXT(json);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  // THIS IS UNUSED
  // WEB->ESP32 is REST
  // ESP32->WEB is WS
  switch(type) {

    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED:
      {
        IPAddress ip = gWebSocketsServer.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
    case WStype_BIN:
		case WStype_ERROR:			
		case WStype_FRAGMENT_TEXT_START:
		case WStype_FRAGMENT_BIN_START:
		case WStype_FRAGMENT:
		case WStype_FRAGMENT_FIN:
		  break;
  }
}

void setupRESTEndpoints() {
      // *****************************************
  // File Browse/Edit
  // *****************************************

  //list directory
  gWebServer.on("/status", HTTP_GET, []() {
    Serial.println("/status(GET)");
    fsBrowserHandleStatus();
  });
  //list directory
  gWebServer.on("/list", HTTP_GET, []() {
    Serial.println("/list(GET)");
    fsBrowserHandleFileList();
  });
  //load editor
  gWebServer.on("/edit", HTTP_GET, []() {
    Serial.println("/edit(GET)");
    if (!fsBrowserHandleFileRead("/edit.htm")) gWebServer.send(404, "text/plain", "FileNotFound");
  });
  //create file
  gWebServer.on("/edit", HTTP_PUT, []() {
    Serial.println("/edit(PUT)");
    fsBrowserHandleFileCreate();
  });
  //delete file
  gWebServer.on("/edit", HTTP_DELETE, []() {
    Serial.println("/edit(DELETE)");
    fsBrowserHandleFileDelete();
  });
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  gWebServer.on("/edit", HTTP_POST, []() {
    Serial.println("/edit(POST)");
    gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
    gWebServer.send(200, "text/plain", "");
  }, fsBrowserHandleFileUpload);

  // *****************************************
  // Static Serve / REST endpoints
  // *****************************************

  gWebServer.on("/all", HTTP_GET, []() {
    Serial.println("/all(GET)");
    String json = getFieldsJson(fields, FIELD_COUNT);
    gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
    gWebServer.send(200, "application/json", json);
  });

  gWebServer.on("/patternByName", HTTP_POST, []() {
    Serial.println("/patternByName(POST)");
    String value = gWebServer.arg("value");
    setPatternByName(value);
    gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
    gWebServer.send(200, "text/plain", String(gPatternIndex));
  });

  gWebServer.on("/fieldValue", HTTP_POST, []() {
    Serial.println("/fieldValue(POST)");
    String name = gWebServer.arg("name");
    String value = gWebServer.arg("value");
    String newValue = setFieldValue(name, value, fields, FIELD_COUNT);
    gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
    gWebServer.send(200, "text/json", newValue);
  });

  gWebServer.on("/configure", HTTP_POST, []() {
    Serial.println("/configure(POST)");
    String service = gWebServer.arg("name");
    String action = gWebServer.arg("value");

    if (service == "wifi" and action == "reset") {
      gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
      gWebServer.send(200, "text/plain", "");
      delay(500);
      gWifiManager.resetSettings();
      ESP.restart();
    }
    if (service == "eeprom" and action == "reset") {
      EEPROM.write(0, ~STORAGE_MAGICAL);
      EEPROM.commit();
      gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
      gWebServer.send(200, "text/plain", "");
      delay(500);
      ESP.restart();
    }
    gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
    gWebServer.send(404, "text/plain", "");
  });

  gWebServer.serveStatic("/", SPIFFS, "/", "max-age=86400");

  gWebServer.on("/patterns/solidColor", HTTP_POST, []() {
    Serial.println("/patterns/solidColor(POST)");

    setSolidRed(gWebServer.arg("r"));
    setSolidGreen(gWebServer.arg("g"));
    setSolidBlue(gWebServer.arg("b"));
    setPatternByName("Solid");

    print_led_status();

    gWebServer.send(200, "text/plain", gWebServer.arg("r") + "," + gWebServer.arg("g") + "," + gWebServer.arg("b"));
    gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
  });

  // gWebServer.on("/updateFirnmware", HTTP_POST, []() {
  //   gWebServer.sendHeader("Connection", "close");
  //   gWebServer.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
  //   ESP.restart();
  // }, []() {
  //   HTTPUpload& upload = gWebServer.upload();
  //   if (upload.status == UPLOAD_FILE_START) {
  //     Serial.printf("Update: %s\n", upload.filename.c_str());
  //     if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
  //       Update.printError(Serial);
  //     }
  //   } else if (upload.status == UPLOAD_FILE_WRITE) {
  //     /* flashing firmware to ESP*/
  //     if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
  //       Update.printError(Serial);
  //     }
  //   } else if (upload.status == UPLOAD_FILE_END) {
  //     if (Update.end(true)) { //true to set the size to the current progress
  //       Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
  //     } else {
  //       Update.printError(Serial);
  //     }
  //   }
  // });
}

