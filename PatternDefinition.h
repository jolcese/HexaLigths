// *****************************************
// Patterns
// *****************************************

typedef void (*PatternFunction)();
typedef struct {
  char name[48];
  PatternFunction function;
  uint8_t delayloop;
  uint8_t parameter1;
  uint8_t parameter2;
} PatternStructure;
typedef PatternStructure PatternStructureList[];

PatternStructureList g_patterns = {
  { "Rotating Rainbow",                         &rotating_rainbow,      100,      0,        0 },            
  { "Pacifica",                                 &pacifica,              20,       0,        0 },
  { "Palette - Rainbow",                        &color_palette,         10,       1,        0 },
  { "Palette - RainbowStripe NoBlend",          &color_palette,         8,        2,        0 },
  { "Palette - RainbowStripe Blend",            &color_palette,         8,        3,        0 },
  { "Palette - Purple and Green",               &color_palette,         20,       4,        0 },
  { "Palette - Random",                         &color_palette,         8,        5,        0 },
  { "Palette - BlackWhiteStripe NoBlend",       &color_palette,         8,        6,        0 },
  { "Palette - BlackWhiteStripe Blend",         &color_palette,         8,        7,        0 },
  { "Palette - Cloud",                          &color_palette,         8,        8,        0 },
  { "Palette - Party",                          &color_palette,         8,        9,        0 },
  { "Palette - RedWhiteBlue NoBlend",           &color_palette,         8,        10,       0 },
  { "Palette - RedWhiteBlue Blend",             &color_palette,         8,        11,       0 },
  { "Reel-rainbow",                             &rainbow,               8,        0,        0 },
  { "Reel-rainbowGlitter",                      &rainbowWithGlitter,    8,        0,        0 },
  { "Reel-confetti",                            &confetti,              20,       0,        3 },
  { "Reel-sinelon",                             &sinelon,               8,        0,        0 },
  { "Reel-juggle",                              &juggle,                15,        0,        0 },
  { "Reel-bpm",                                 &bpm,                   8,        0,        0 },
  { "Cylon",                                    &cylon,                 30,       0,        200 },
  { "Solid Background Loop",                    &solid_background,      20,       0,        0 },
  { "Solid Background Controlled",              &solid_background,      20,       1,        1 },
  { "Solid Dot Loop",                           &solid_background,      20,       2,        0 },
  { "Solid Dot Loop Rand",                      &solid_background,      20,       3,        10 },
  { "Whole Tile Palette",                       &whole_tile,            40,       0 ,       12 }
};

const uint8_t PATTERNS_TOTAL = (sizeof(g_patterns) / sizeof((g_patterns)[0]));
