
// *****************************************
// HW Definition
// *****************************************

// MH ET Mini ESP32
// https://forum.mhetlive.com/topic/8/mh-et-live-minikit-for-esp32/5
//
#define STRIP_DATA_PIN      16
#define FASTLED_ESP32_I2S

// Wemos D1 ESP8266
//
//#define STRIP_DATA_PIN      D4
//#define FASTLED_ESP8266_RAW_PIN_ORDER
//#define FASTLED_ALLOW_INTERRUPTS 0

// TODO
// rainbow all around
// eeprom
// wifi
// autoplay

#include <FastLED.h>

#define NUM_LEDS_PER_HEX    36
#define NUM_HEX             5
#define NUM_LEDS            NUM_LEDS_PER_HEX * NUM_HEX

#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB

#include <Esp32WifiManager.h>
WiFiManager wifiManager;

// *****************************************
// Around Edge
// *****************************************

//    0  -   35 Hex 1 
//   36  -   71 Hex 2
//   72  -  107 Hex 3
//  108  -  143 Hex 4
//  144  -  179 Hex 5

#define EDGE_LEDS 132

uint8_t around[EDGE_LEDS] = { 0, 1, 2,
                   3, 4, 5, 6, 7, 8, 
                   69, 70, 71,
                   36, 37, 38,
                   39, 40, 41, 42, 43, 44,
                   45, 46 ,47 ,48, 49, 50,
                   75, 76, 77, 78, 79, 80,
                   141, 142, 143,
                   108, 109, 110,
                   171, 172, 173, 174, 175, 176,
                   177, 178, 179,
                   144, 145, 146,
                   147, 148, 149, 150, 151, 152,
                   153, 154, 155, 156, 157, 158,
                   159, 160, 161, 162, 163, 164,
                   117, 118, 119, 120, 121, 122,
                   123, 124, 125, 126, 127, 128,
                   129, 130, 131, 132, 133, 134,
                   87, 88, 89, 90, 91, 92,
                   93, 94, 95, 96, 97, 98,
                   99, 100, 101, 102, 103, 104,
                   57, 58, 59, 60, 61, 62,
                   15, 16, 17, 18, 19, 20,
                   21, 22, 23, 24, 25, 26,
                   27, 28, 29, 30, 31, 32,
                   33, 34, 35 };
                   
// *****************************************
// Patterns
// *****************************************
#define PATTERNS_TOTAL 25

char pattern_str[PATTERNS_TOTAL][48] = {
  "Rotating Rainbow",
  "Pacifica",
  "Palette - Rainbow",
  "Palette - RainbowStripe NoBlend",
  "Palette - RainbowStripe Blend",
  "Palette - Purple and Green",
  "Palette - Random",
  "Palette - BlackWhiteStripe NoBlend",
  "Palette - BlackWhiteStripe Blend",
  "Palette - Cloud",
  "Palette - Party",
  "Palette - RedWhiteBlue NoBlend",
  "Palette - RedWhiteBlue Blend",
  "Reel-rainbow",
  "Reel-rainbowGlitter",
  "Reel-confetti",
  "Reel-sinelon",
  "Reel-juggle",
  "Reel-bpm",
  "Cylon",
  "Solid Backround Loop",
  "Solid Backround Controlled",
  "Solid Dot Loop",
  "Solid Dot Loop Rand",
  "Whole Tile Palette"
};

uint8_t g_pattern_delayloop_default[PATTERNS_TOTAL] = {
  100,
  20,
  10,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  20,
  8,
  8,
  8,
  30,
  20,
  20,
  20,
  20,
  40
};

uint8_t pattern_parameter_1_default[PATTERNS_TOTAL] = {
  0,
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10,
  11,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  2,
  3,
  0
};

uint8_t pattern_parameter_2_default[PATTERNS_TOTAL] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  3,
  0,
  0,
  0,
  200,
  0,
  1,
  0,
  10,
  12
};

// *****************************************
// Global variables
// *****************************************

#define BRIGTHNESS_FULL     255
#define BRIGHTNESS_STEP     0.15 //15 percent;
#define MAX_DELAYLOOP       1500 //1.5 sec;
#define DELAYLOOP_STEP      0.15 //15 percent;

CRGB g_leds[NUM_LEDS];

uint8_t g_pattern;
int16_t g_pattern_delayloop;
uint8_t g_pattern_parameter_1;
uint8_t g_pattern_parameter_2;
boolean g_repeat = true;
boolean g_cycle = true;
int16_t g_brigthness = BRIGTHNESS_FULL;

// *****************************************
// setup
// *****************************************
void setup() {
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(0xFFD0F0);
  FastLED.addLeds<LED_TYPE,STRIP_DATA_PIN,COLOR_ORDER>(g_leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);
  FastLED.setBrightness( g_brigthness );
  FastLED.setDither(DISABLE_DITHER); 

  g_pattern = 0;
  g_pattern_delayloop = g_pattern_delayloop_default[g_pattern];
  g_pattern_parameter_1 = pattern_parameter_1_default[g_pattern];
  g_pattern_parameter_2 = pattern_parameter_2_default[g_pattern];

  Serial.begin(115200);

  print_help();
  
}

//typedef void (*func)(uint8_t param_1, uint8_t param_2);
typedef void (*func)();

func pattern_function[PATTERNS_TOTAL] = {
  &rotating_rainbow,
  &pacifica,
  &color_palette,
  &color_palette,
  &color_palette,
  &color_palette,
  &color_palette,
  &color_palette,
  &color_palette,
  &color_palette,
  &color_palette,
  &color_palette,
  &color_palette,
  &rainbow,
  &rainbowWithGlitter,
  &confetti,
  &sinelon,
  &juggle,
  &bpm,
  &cylon,
  &solid_background,
  &solid_background,
  &solid_background,
  &solid_background,
  &whole_tile
}; 


// *****************************************
// Helper Functions
// *****************************************

void print_help() {
  Serial.println("");
  Serial.println("HexaCheap v1");
  Serial.println("------------");
  Serial.println("");
  Serial.println(" Z - Previous Pattern");
  Serial.println(" X - Next Pattern");
  Serial.println(" A - Decrease Brightness");
  Serial.println(" S - Increase Brightness");
  Serial.println(" Q - Decrease Speed");
  Serial.println(" W - Increase Speed");
  Serial.println(" D - Decrease Pattern Prameter 2");
  Serial.println(" F - Increase Pattern Prameter 2");
  Serial.println(" C - Current Status");
  Serial.println(" R - Repeat pattern on tiles");
  Serial.println(" T - Cycle effect");
  Serial.println(" H - Help");
}

void print_status() {
  Serial.print("Pattern = ");
  Serial.print(g_pattern);
  Serial.print(" - ");
  Serial.println(pattern_str[g_pattern]);
  Serial.print("Brightness = ");
  Serial.println(g_brigthness);
  Serial.print("Delay = ");
  Serial.println(g_pattern_delayloop);
  Serial.print("Pattern Parameter 1 = ");
  Serial.println(g_pattern_parameter_1);
  Serial.print("Pattern Parameter 2 = ");
  Serial.println(g_pattern_parameter_2);
  Serial.print("Repeat = ");
  Serial.println(g_repeat);
  Serial.print("Cycle = ");
  Serial.println(g_cycle);
  Serial.print("FPS = ");
  Serial.println(FastLED.getFPS());
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
        print_status();
        break;

      // Repeat pattern on every Tile
      case 'R':
      case 'r':
        if (g_repeat == true) {
          g_repeat = false;
          Serial.println("Pattern will be done across all LEDS");
        } else {
          g_repeat = true;
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
        if (g_pattern == 0) {
          g_pattern = PATTERNS_TOTAL - 1;
        } else {
          g_pattern--;        
        }
        Serial.print(g_pattern);
        Serial.print(" - ");
        Serial.println( pattern_str[g_pattern] );
        g_pattern_delayloop = g_pattern_delayloop_default[g_pattern];
        g_pattern_parameter_1 = pattern_parameter_1_default[g_pattern];
        g_pattern_parameter_2 = pattern_parameter_2_default[g_pattern];
        break;

      // Next pattern
      case 'X':
      case 'x':
        if (g_pattern == PATTERNS_TOTAL - 1) {
          g_pattern = 0;
        } else {
          g_pattern++;  
        }
        Serial.print(g_pattern);
        Serial.print(" - ");
        Serial.println(pattern_str[g_pattern]);
        g_pattern_delayloop = g_pattern_delayloop_default[g_pattern];
        g_pattern_parameter_1 = pattern_parameter_1_default[g_pattern];
        g_pattern_parameter_2 = pattern_parameter_2_default[g_pattern];
        break;

      // Decrease Brigthness
      case 'A':
      case 'a':
        g_brigthness = FastLED.getBrightness() * (1 - BRIGHTNESS_STEP);
        if (g_brigthness < 0) g_brigthness = 0;
        Serial.print("Brightness = ");
        Serial.println(g_brigthness);
        FastLED.setBrightness( (uint8_t) g_brigthness );
        break;

      // Increase Brightness
      case 'S':
      case 's':
        if ((uint8_t)g_brigthness == (uint8_t)(FastLED.getBrightness() * (1 + BRIGHTNESS_STEP))) {
          g_brigthness++;
        } else {
          g_brigthness = FastLED.getBrightness() * (1 + BRIGHTNESS_STEP);          
        }
        if (g_brigthness > BRIGTHNESS_FULL) g_brigthness = BRIGTHNESS_FULL;
        Serial.print("Brightness = ");
        Serial.println(g_brigthness);
        FastLED.setBrightness( (uint8_t) g_brigthness );
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
  
  pattern_function[g_pattern]();
}
