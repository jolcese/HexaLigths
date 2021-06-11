// *****************************************
// Network Header
// *****************************************

#define WL_MAC_ADDR_LENGTH 6

void WiFiEventHandler(WiFiEvent_t event, system_event_info_t info);
void setupWifiManager();

void broadcastInt(String name, uint8_t value);
void broadcastString(String name, String value);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void setupRESTEndpoints();


  