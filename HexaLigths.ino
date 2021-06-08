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
String gHostNameString;
char gHostNameChar[64];

#include <WebServer.h>
WebServer gWebServer(80);

// #include <WebSockets2_Generic.h>
// using namespace websockets2_generic;
// WebsocketsServer gWebSocketServer;

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

uint8_t gPatternIndex;
// int16_t gPatternDelayloop;
// uint8_t gPatternParameter_1;
// uint8_t gPatternParameter_2;
// uint8_t gPatternParameter_3;
// uint8_t gPatternParameter_4;
boolean gPowerLed = true;
// boolean gIdenticalTiles = true;
// boolean gCycle = true;
int16_t gBrightness = BRIGTHNESS_FULL;

// *****************************************
// Modules
// *****************************************
#include "Palettes.h"
#include "PatternAround.h"

#include "PatternColorPalette.h"
#include "PatternCylon.h"
#include "PatternDemoReel.h"
#include "PatternDemoReelConfetti.h"
#include "PatternDemoReelSinelon.h"
#include "PatternDemoReelJuggle.h"
#include "PatternDemoReelBpm.h"
#include "PatternPacifica.h"
#include "PatternRainbow.h"
#include "PatternSolid.h"
#include "PatternSolidEffects.h"
#include "PatternWholeTile.h"

#include "PatternDefinition.h"

#include "Storage.h"

#include "FieldTypes.h"
#include "Fields.h"

// *****************************************
// setup
// *****************************************

#include "ConsoleHelper.h"
#include "WifiManagerHelper.h"

void setup() {
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(0xFFD0F0);
  FastLED.addLeds<LED_TYPE,STRIP_DATA_PIN,COLOR_ORDER>(gLeds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_STORAGE_POWER_MILLIAMPS);
  FastLED.setBrightness( gBrightness );
  FastLED.setDither(BINARY_DITHER); 

  gPatternIndex = 0;
  // gPatternDelayloop = g_patterns[gPatternIndex].delayloop;
  // gPatternParameter_1 = g_patterns[gPatternIndex].parameter1;
  // gPatternParameter_2 = g_patterns[gPatternIndex].parameter2;
  // gPatternParameter_3 = 0;
  // gPatternParameter_4 = 0;
  
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

  // gWebServer.on("/patterns/solidColor", HTTP_POST, []() {
  //   Serial.println("/patterns/solidColor(POST)");

  //   String r = gWebServer.arg("r");
  //   String g = gWebServer.arg("g");
  //   String b = gWebServer.arg("b");
    
  //   CRGB color = CRGB(r.toInt(), g.toInt(), b.toInt());
  //   gPatternParameter_2 = r.toInt();
  //   gPatternParameter_3 = g.toInt();
  //   gPatternParameter_4 = b.toInt();
  
  //   gCycle = false;
  //   gPatternParameter_1 = 1;
  //   gPatternIndex = 21;

  //   print_led_status();

  //   gWebServer.send(200, "text/plain", String(color.r) + "," + String(color.g) + "," + String(color.b));
  //   gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
  // });

  gWebServer.begin();
  Serial.println("HTTP web server started");

  // gWebSocketServer.listen(81);
  // Serial.println("Web Socket server started");

  if(!MDNS.begin(gHostNameChar)) {
      Serial.println("Error starting mDNS!");
  } else {
    Serial.println("MDNS started");
    MDNS.addService("http", "tcp", 80);
  }

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

      // // Repeat pattern on every Tile
      // case 'R':
      // case 'r':
      //   if (gIdenticalTiles == true) {
      //     gIdenticalTiles = false;
      //     Serial.println("Pattern will be done across all LEDS");
      //   } else {
      //     gIdenticalTiles = true;
      //     Serial.println("Pattern will be repeated on every Tile");
      //   }
      //   break;

      // // Rotate effect
      // case 'T':
      // case 't':
      //   if (gCycle == true) {
      //     gCycle = false;
      //     Serial.println("Pattern will not cycle");
      //   } else {
      //     gCycle = true;
      //     Serial.println("Pattern will cycle");
      //   }
      //   break;

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
        // gPatternDelayloop = g_patterns[gPatternIndex].delayloop;
        // gPatternParameter_1 = g_patterns[gPatternIndex].parameter1;
        // gPatternParameter_2 = g_patterns[gPatternIndex].parameter2;
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
        // gPatternDelayloop = g_patterns[gPatternIndex].delayloop;
        // gPatternParameter_1 = g_patterns[gPatternIndex].parameter1;
        // gPatternParameter_2 = g_patterns[gPatternIndex].parameter2;
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

      // // Decrease Speed
      // case 'Q':
      // case 'q':
      //   gPatternDelayloop += ((gPatternDelayloop * DELAYLOOP_STEP) > 1 ? (gPatternDelayloop * DELAYLOOP_STEP) : 1);
      //   if (gPatternDelayloop > MAX_DELAYLOOP) gPatternDelayloop = MAX_DELAYLOOP;
      //   Serial.print("Delay = ");
      //   Serial.println(gPatternDelayloop);
      //   break;

      // // Increase Speed
      // case 'W':
      // case 'w':
      //   gPatternDelayloop -= ((gPatternDelayloop * DELAYLOOP_STEP) > 1 ? (gPatternDelayloop * DELAYLOOP_STEP) : 1);
      //   if (gPatternDelayloop < 0) gPatternDelayloop = 1;
      //   Serial.print("Delay = ");
      //   Serial.println(gPatternDelayloop);
      //   break;

      // // Decrease Parameter 2
      // case 'D':
      // case 'd':
      //   gPatternParameter_2 -= 1;
      //   Serial.print("Pattern Parameter 2 = ");
      //   Serial.println(gPatternParameter_2);
      //   break;

      // // Increase Parameter 2
      // case 'F':
      // case 'f':
      //   gPatternParameter_2 += 1;
      //   Serial.print("Pattern Parameter 2 = ");
      //   Serial.println(gPatternParameter_2);
      //   break;
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

  // WebsocketsClient wsClient = gWebSocketServer.accept();
 
  // if (wsClient.available())
  // {
  //   //WebsocketsMessage msg = client.readNonBlocking();
  //   WebsocketsMessage msg = wsClient.readBlocking();

  //   // log
  //   Serial.print("Got Message: ");
  //   Serial.println(msg.data());

  //   // return echo
  //   wsClient.send("Echo: " + msg.data());

  //   // close the connection
  //   // wsClient.close();
  // }

   if (gPowerLed == false) {
    fill_solid(gLeds, NUM_LEDS, CRGB::Black);
    // FIX
    FastLED.delay(1000 / FRAMES_PER_SECOND);

  } else {
    g_patterns[gPatternIndex].function();
  }
}
