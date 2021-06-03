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
WiFiManager g_wifiManager;
String g_nameString;

#include <WebServer.h>
WebServer g_webServer(80);

#include <SPIFFS.h>
#include "spiffshelper.h"
#include "fsBrowser.h"

//#include "FS.h"

// *****************************************
// Global variables
// *****************************************

#define BRIGTHNESS_FULL     255
#define BRIGHTNESS_STEP     0.15 //15 percent;
#define MAX_DELAYLOOP       1500 //1.5 sec;
#define DELAYLOOP_STEP      0.15 //15 percent;

CRGB g_leds[NUM_LEDS];

uint8_t g_pattern_index;
int16_t g_pattern_delayloop;
uint8_t g_pattern_parameter_1;
uint8_t g_pattern_parameter_2;
uint8_t g_pattern_parameter_3;
uint8_t g_pattern_parameter_4;
boolean g_power = true;
boolean g_identical_tiles = true;
boolean g_cycle = true;
int16_t g_brightness = BRIGTHNESS_FULL;

// *****************************************
// Modules
// *****************************************
#include "PatternAround.h"
#include "PatternColorpalette.h"
#include "PatternCylon.h"
#include "PatternDemoReel.h"
#include "PatternPacifica.h"
#include "PatternRainbow.h"
#include "PatternSolid.h"
#include "PatternWholeTile.h"

#include "PatternDefinition.h"
#include "FieldTypes.h"
#include "Fields.h"

// *****************************************
// setup
// *****************************************

#include "ConsoleHelper.h"
#include "WifiManagerHelper.h"

void setup() {
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(0xFFD0F0);
  FastLED.addLeds<LED_TYPE,STRIP_DATA_PIN,COLOR_ORDER>(g_leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);
  FastLED.setBrightness( g_brightness );
  FastLED.setDither(BINARY_DITHER); 

  g_pattern_index = 0;
  g_pattern_delayloop = g_patterns[g_pattern_index].delayloop;
  g_pattern_parameter_1 = g_patterns[g_pattern_index].parameter1;
  g_pattern_parameter_2 = g_patterns[g_pattern_index].parameter2;
  g_pattern_parameter_3 = 0;
  g_pattern_parameter_4 = 0;
  
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
  g_webServer.on("/list", HTTP_GET, []() {
    Serial.println("/list(GET)");
    fsBrowserHandleFileList();
  });
  //load editor
  g_webServer.on("/edit", HTTP_GET, []() {
    Serial.println("/edit(GET)");
    if (!fsBrowserHandleFileRead("/edit.htm")) g_webServer.send(404, "text/plain", "FileNotFound");
  });
  //create file
  g_webServer.on("/edit", HTTP_PUT, []() {
    Serial.println("/edit(PUT)");
    fsBrowserHandleFileCreate();
  });
  //delete file
  g_webServer.on("/edit", HTTP_DELETE, []() {
    Serial.println("/edit(DELETE)");
    fsBrowserHandleFileDelete();
  });
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  g_webServer.on("/edit", HTTP_POST, []() {
    Serial.println("/edit(POST)");
    g_webServer.sendHeader("Access-Control-Allow-Origin", "*");
    g_webServer.send(200, "text/plain", "");
  }, fsBrowserHandleFileUpload);

  // *****************************************
  // Static Serve / REST endpoints
  // *****************************************

  g_webServer.on("/all", HTTP_GET, []() {
    Serial.println("/all(GET)");
    String json = getFieldsJson(fields, FIELD_COUNT);
    g_webServer.sendHeader("Access-Control-Allow-Origin", "*");
    g_webServer.send(200, "application/json", json);
  });

  g_webServer.on("/patternByName", HTTP_POST, []() {
    Serial.println("/patternByName(POST)");
    String value = g_webServer.arg("value");
    setPatternByName(value);
    g_webServer.sendHeader("Access-Control-Allow-Origin", "*");
    g_webServer.send(200, "text/plain", String(g_pattern_index));
  });

  g_webServer.on("/fieldValue", HTTP_POST, []() {
    String name = g_webServer.arg("name");
    String value = g_webServer.arg("value");
    String newValue = setFieldValue(name, value, fields, FIELD_COUNT);
    g_webServer.sendHeader("Access-Control-Allow-Origin", "*");
    g_webServer.send(200, "text/json", newValue);
  });

  g_webServer.serveStatic("/", SPIFFS, "/", "max-age=86400");

  g_webServer.on("/patterns/solidColor", HTTP_POST, []() {
    Serial.println("/patterns/solidColor(POST)");

    String r = g_webServer.arg("r");
    String g = g_webServer.arg("g");
    String b = g_webServer.arg("b");
    
    CRGB color = CRGB(r.toInt(), g.toInt(), b.toInt());
    g_pattern_parameter_2 = r.toInt();
    g_pattern_parameter_3 = g.toInt();
    g_pattern_parameter_4 = b.toInt();
  
    g_cycle = false;
    g_pattern_parameter_1 = 1;
    g_pattern_index = 21;

    print_led_status();

    g_webServer.send(200, "text/plain", String(color.r) + "," + String(color.g) + "," + String(color.b));
    g_webServer.sendHeader("Access-Control-Allow-Origin", "*");
  });

  // MDNS.begin(nameChar);
  // MDNS.setHostname(nameChar);

  g_webServer.begin();
  Serial.println("HTTP web server started");

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

      // Repeat pattern on every Tile
      case 'R':
      case 'r':
        if (g_identical_tiles == true) {
          g_identical_tiles = false;
          Serial.println("Pattern will be done across all LEDS");
        } else {
          g_identical_tiles = true;
          Serial.println("Pattern will be repeated on every Tile");
        }
        break;

      // Rotate effect
      case 'T':
      case 't':
        if (g_cycle == true) {
          g_cycle = false;
          Serial.println("Pattern will not cycle");
        } else {
          g_cycle = true;
          Serial.println("Pattern will cycle");
        }
        break;

      // Previous pattern
      case 'Z':
      case 'z':
        if (g_pattern_index == 0) {
          g_pattern_index = PATTERNS_TOTAL - 1;
        } else {
          g_pattern_index--;        
        }
        Serial.print(g_pattern_index);
        Serial.print(" - ");
        Serial.println(g_patterns[g_pattern_index].name);
        g_pattern_delayloop = g_patterns[g_pattern_index].delayloop;
        g_pattern_parameter_1 = g_patterns[g_pattern_index].parameter1;
        g_pattern_parameter_2 = g_patterns[g_pattern_index].parameter2;
          break;

      // Next pattern
      case 'X':
      case 'x':
        if (g_pattern_index == PATTERNS_TOTAL - 1) {
          g_pattern_index = 0;
        } else {
          g_pattern_index++;  
        }
        Serial.print(g_pattern_index);
        Serial.print(" - ");
        Serial.println(g_patterns[g_pattern_index].name);
        g_pattern_delayloop = g_patterns[g_pattern_index].delayloop;
        g_pattern_parameter_1 = g_patterns[g_pattern_index].parameter1;
        g_pattern_parameter_2 = g_patterns[g_pattern_index].parameter2;
        break;

      // Decrease Brigthness
      case 'A':
      case 'a':
        g_brightness = FastLED.getBrightness() * (1 - BRIGHTNESS_STEP);
        if (g_brightness < 0) g_brightness = 0;
        Serial.print("Brightness = ");
        Serial.println(g_brightness);
        FastLED.setBrightness( (uint8_t) g_brightness );
        break;

      // Increase Brightness
      case 'S':
      case 's':
        if ((uint8_t)g_brightness == (uint8_t)(FastLED.getBrightness() * (1 + BRIGHTNESS_STEP))) {
          g_brightness++;
        } else {
          g_brightness = FastLED.getBrightness() * (1 + BRIGHTNESS_STEP);          
        }
        if (g_brightness > BRIGTHNESS_FULL) g_brightness = BRIGTHNESS_FULL;
        Serial.print("Brightness = ");
        Serial.println(g_brightness);
        FastLED.setBrightness( (uint8_t) g_brightness );
        break;

      // Decrease Speed
      case 'Q':
      case 'q':
        g_pattern_delayloop += ((g_pattern_delayloop * DELAYLOOP_STEP) > 1 ? (g_pattern_delayloop * DELAYLOOP_STEP) : 1);
        if (g_pattern_delayloop > MAX_DELAYLOOP) g_pattern_delayloop = MAX_DELAYLOOP;
        Serial.print("Delay = ");
        Serial.println(g_pattern_delayloop);
        break;

      // Increase Speed
      case 'W':
      case 'w':
        g_pattern_delayloop -= ((g_pattern_delayloop * DELAYLOOP_STEP) > 1 ? (g_pattern_delayloop * DELAYLOOP_STEP) : 1);
        if (g_pattern_delayloop < 0) g_pattern_delayloop = 1;
        Serial.print("Delay = ");
        Serial.println(g_pattern_delayloop);
        break;

      // Decrease Parameter 2
      case 'D':
      case 'd':
        g_pattern_parameter_2 -= 1;
        Serial.print("Pattern Parameter 2 = ");
        Serial.println(g_pattern_parameter_2);
        break;

      // Increase Parameter 2
      case 'F':
      case 'f':
        g_pattern_parameter_2 += 1;
        Serial.print("Pattern Parameter 2 = ");
        Serial.println(g_pattern_parameter_2);
        break;
    }
    if (incomingByte != 10) {
      Serial.print("> ");
    }
  }
  
  while (WiFi.status() != WL_CONNECTED)
  {
    setupWifiManager();
    // MDNS.begin(nameChar);
    // MDNS.setHostname(nameChar);
  }

  g_wifiManager.process();
  g_webServer.handleClient();
  // MDNS.update();

   if (g_power == false) {
    fill_solid(g_leds, NUM_LEDS, CRGB::Black);
    // FIX
    FastLED.delay(1000 / FRAMES_PER_SECOND);

  } else {
    g_patterns[g_pattern_index].function();
    FastLED.delay(g_pattern_delayloop);
  }
  // FastLED.delay(1000 / FRAMES_PER_SECOND);

}
