// *****************************************
// Hexa Ligths
// *****************************************

// Based on ESP8266 Fastled Webserver by Jason Coon: https://github.com/jasoncoon/esp8266-fastled-webserver
// Based on ColorTwinkles by Mark Kriegsman: https://gist.github.com/kriegsman/5408ecd397744ba0393e
// Based on TwinkleFOX by Mark Kriegsman: https://gist.github.com/kriegsman/756ea6dcae8e30845b5a

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
#include "WifiManagerHelper.h"

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

// *****************************************
// setup
// *****************************************

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

  while (Serial.available() > 0) { 

    int incomingByte = Serial.read();
    if (incomingByte != 10) Serial.println(char(incomingByte));
    
    switch (incomingByte) {
      // Print help
      case 'H':
      case 'h':
        print_help();
        break;

      // Show status
      case 'C':
      case 'c':
        print_led_status();
        break;

      // Previous pattern
      case 'Z':
      case 'z':
        if (gPatternIndex == 0) {
          gPatternIndex = PATTERNS_TOTAL - 1;
        } else {
          gPatternIndex--;        
        }
        Serial.print(gPatternIndex);
        Serial.print(" - ");
        Serial.println(g_patterns[gPatternIndex].name);
        break;

      // Next pattern
      case 'X':
      case 'x':
        if (gPatternIndex == PATTERNS_TOTAL - 1) {
          gPatternIndex = 0;
        } else {
          gPatternIndex++;  
        }
        Serial.print(gPatternIndex);
        Serial.print(" - ");
        Serial.println(g_patterns[gPatternIndex].name);
        break;

      // Decrease Brigthness
      case 'A':
      case 'a':
        gBrightness = FastLED.getBrightness() * (1 - BRIGHTNESS_STEP);
        if (gBrightness < 0) gBrightness = 0;
        Serial.print("Brightness = ");
        Serial.println(gBrightness);
        FastLED.setBrightness( (uint8_t) gBrightness );
        break;

      // Increase Brightness
      case 'S':
      case 's':
        if ((uint8_t)gBrightness == (uint8_t)(FastLED.getBrightness() * (1 + BRIGHTNESS_STEP))) {
          gBrightness++;
        } else {
          gBrightness = FastLED.getBrightness() * (1 + BRIGHTNESS_STEP);          
        }
        if (gBrightness > BRIGTHNESS_FULL) gBrightness = BRIGTHNESS_FULL;
        Serial.print("Brightness = ");
        Serial.println(gBrightness);
        FastLED.setBrightness( (uint8_t) gBrightness );
        break;

    }
    if (incomingByte != 10) {
      Serial.print("> ");
    }
  }
  
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
