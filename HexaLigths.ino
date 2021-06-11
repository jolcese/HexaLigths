// *****************************************
// Hexa Ligths
// *****************************************

// Based on ESP8266 Fastled Webserver by Jason Coon: https://github.com/jasoncoon/esp8266-fastled-webserver
// Based on ColorTwinkles by Mark Kriegsman: https://gist.github.com/kriegsman/5408ecd397744ba0393e
// Based on TwinkleFOX by Mark Kriegsman: https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
// Based on DemoReel100 by FastLED: https://github.com/FastLED/FastLED/blob/master/examples/DemoReel100/DemoReel100.ino

// *****************************************
// HW Definition
// *****************************************
// MH ET Mini ESP32
// https://forum.mhetlive.com/topic/8/mh-et-live-minikit-for-esp32/5

// *****************************************
// Led
// *****************************************

#define FASTLED_ESP32_I2S
#include <FastLED.h>

#define STRIP_DATA_PIN      16
#define NUM_LEDS_PER_HEX    36
#define NUM_HEX             5
#define NUM_LEDS            NUM_LEDS_PER_HEX * NUM_HEX

#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB

#define FRAMES_PER_SECOND  120

// *****************************************
// Wifi
// *****************************************

#include <WiFiManager.h> 
WiFiManager gWifiManager;
char gHostName[64];

#include <WebServer.h>
WebServer gWebServer(80);

#include <WebSocketsServer.h>
WebSocketsServer gWebSocketsServer = WebSocketsServer(81);

//#include <FS.h>
#include <SPIFFS.h>
#include "spiffshelper.h"
#include "fsBrowser.h"

#include <ESPmDNS.h>

boolean gFirstBoot = true;


// *****************************************
// Global variables
// *****************************************

#define BRIGTHNESS_FULL     255
#define BRIGHTNESS_STEP     0.15 //15 percent;
#define MAX_DELAYLOOP       1500 //1.5 sec;
#define DELAYLOOP_STEP      0.15 //15 percent;

CRGB gLeds[NUM_LEDS];

uint8_t gPatternIndex = 3;
boolean gPowerLed = true;
int16_t gBrightness = BRIGTHNESS_FULL;
boolean gAutoplay = false;
uint8_t gAutoplaySeconds = 10;
uint32_t gAutoplayTimeout = 0;

// *****************************************
// Modules
// *****************************************
#include "NetworkHeader.h"
#include "StorageHeader.h"

#include "Palettes.h"
#include "PatternAround.h"
// #include "PatternRainbow.h"
#include "PatternColorPalette.h"
#include "PatternCylon.h"
#include "PatternDemoReel.h"
#include "PatternDemoReelConfetti.h"
#include "PatternDemoReelSinelon.h"
#include "PatternDemoReelJuggle.h"
#include "PatternDemoReelBpm.h"
#include "PatternPacifica.h"
#include "PatternSolid.h"
#include "PatternSolidEffects.h"
#include "PatternWholeTile.h"
#include "PatternTwinkles.h"
#include "PatternTwinkleFox.h"

#include "PatternDefinition.h"

#include "FieldTypes.h"
#include "Fields.h"

#include "ConsoleHelper.h"
#include "Storage.h"
#include "Network.h"

// *****************************************
// setup
// *****************************************


void setup() {
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(0xFFD0F0);
  FastLED.addLeds<LED_TYPE,STRIP_DATA_PIN,COLOR_ORDER>(gLeds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_STORAGE_POWER_MILLIAMPS);
  FastLED.setBrightness( gBrightness );
  FastLED.setDither(BINARY_DITHER); 
  
  Serial.begin(115200);

  print_system_status();

  // *****************************************
  // Wifi
  // *****************************************

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP   
  WiFi.onEvent(WiFiEventHandler);
  
  printSpiffsContents();
  setupWifiManager();

  setupRESTEndpoints();

  gWebServer.begin();
  Serial.println("HTTP web server started");

  gWebSocketsServer.begin();
  gWebSocketsServer.onEvent(webSocketEvent);
  Serial.println("Web Socket server started");

  if(!MDNS.begin(gHostName)) {
      Serial.println("Error starting mDNS!");
  } else {
    Serial.println("MDNS started");
    MDNS.addService("http", "tcp", 80);
  }

  gAutoplayTimeout = millis() + (gAutoplaySeconds * 1000);

  storageLoadSettings();
  
  print_led_status();
  print_help();
  
}

// *****************************************
// loop
// *****************************************
void loop() {

  consoleLoop();

  while (WiFi.status() != WL_CONNECTED && gFirstBoot == false )
  {
    Serial.println("Reconnect Wifi!");

    setupWifiManager();
  }

  gWifiManager.process();
  gWebServer.handleClient();
  // MDNS.update();

  gWebSocketsServer.loop();

   if (gPowerLed == false) {
    fill_solid(gLeds, NUM_LEDS, CRGB::Black);
    // FIX
    FastLED.delay(1000 / FRAMES_PER_SECOND);

  } else {

    if (gAutoplay == true && (millis() > gAutoplayTimeout)) { 
      gAutoplayTimeout = millis() + (gAutoplaySeconds * 1000);
      setPattern(String(getPattern().toInt() + 1));
    }

    g_patterns[gPatternIndex].function();
  }
}
