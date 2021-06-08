// *****************************************
// Patterns
// *****************************************

typedef void (*PatternFunction)();
typedef struct {
  String name;
  String label;
  PatternFunction function;

} PatternStructure;
typedef PatternStructure PatternStructureList[];

PatternStructureList g_patterns = {
  { "rainbow",        "Rotating Rainbow",        &fRainbow },            
  { "pacifica",       "Pacifica",                &fPacifica },
  { "colorPalette",   "Color Palette",           &fColorPalette },
  { "demoReel",       "Reel",                    &fDemoReel },
  { "confetti",       "Confetti",                &fConfetti },
  { "sinelon",        "Sinelon",                 &fSinelon },
  { "juggle",         "Juggle",                  &fJuggle },
  { "bpm",            "BPM",                     &fBPM },
  { "cylon",          "Cylon",                   &fCylon },
  { "solid",          "Solid",                   &fSolid },
  { "solidEffects",   "Solid Effects",           &fSolidEffects },
  { "wholeTile",      "Whole Tile Palette",      &fWholeTile }
};

const uint8_t PATTERNS_TOTAL = (sizeof(g_patterns) / sizeof((g_patterns)[0]));
