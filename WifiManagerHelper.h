// *****************************************
// Wifi Manager Helper
// *****************************************

#define WL_MAC_ADDR_LENGTH 6

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

  // Do a little work to get a unique-ish name. Get the
  // last two bytes of the MAC (HEX'd)":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  //FIX here//
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();

  g_hostNameString = "HexaLigths-" + macID;

  //char g_hostNameChar[g_hostNameString.length() + 1];
  memset(g_hostNameChar, 0, g_hostNameString.length() + 1);

  for (int i = 0; i < g_hostNameString.length(); i++)
    g_hostNameChar[i] = g_hostNameString.charAt(i);

  Serial.println();
  Serial.println("Wifi Manager Setup");
  Serial.println("------------------");
  Serial.printf("MAC address = %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println();
  Serial.printf("Name: %s\n", g_hostNameChar );
  Serial.println();
  
  // reset settings - wipe credentials for testing
  // g_wifiManager.resetSettings();
  g_wifiManager.setConfigPortalBlocking(false);

  //automatically connect using saved credentials if they exist
  //If connection fails it starts an access point with the specified name
  if(g_wifiManager.autoConnect(g_hostNameChar)){
    Serial.println("Wi-Fi connected");
    gFirstBoot = false;
  }
  else {
    Serial.println("Wi-Fi manager portal running");
  }
}
