/*
   ESP8266 + FastLED + IR Remote: https://github.com/jasoncoon/esp8266-fastled-webserver
   Copyright (C) 2016 Jason Coon

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// *****************************************
// Power
// *****************************************

String setPower(String value) {
  gPowerLed = value.toInt();
  if (gPowerLed < 0) gPowerLed = 0;
  if (gPowerLed > 1) gPowerLed = 1;
  storageWrite("power");
  broadcastInt("power", gPowerLed);
  return String(gPowerLed);
}

String getPower() {
  return String(gPowerLed);
}

// *****************************************
// Brightness
// *****************************************

String setBrightness(String value) {
  gBrightness = value.toInt();
  if (gBrightness < 0) gBrightness = 0;
  if (gBrightness > 255) gBrightness = 255;
  FastLED.setBrightness( gBrightness );
  storageWrite("brightness");
  broadcastInt("brightness", gBrightness);
  return String(gBrightness);
}

String getBrightness() {
  return String(gBrightness);
}
// *****************************************
// Autoplay
// *****************************************

String setAutoplay(String value) {
  gAutoplay = value.toInt();
  if (gAutoplay < 0) gAutoplay = 0;
  if (gAutoplay > 1) gAutoplay = 1;
  storageWrite("autoplay");
  broadcastInt("autoplay", gAutoplay);
  return String(gAutoplay);
}

String getAutoplay() {
  return String(gAutoplay);
}

String setAutoplaySeconds(String value) {
  gAutoplaySeconds = value.toInt();
  if (gAutoplaySeconds < 0) gAutoplaySeconds = 0;
  if (gAutoplaySeconds > 255) gAutoplaySeconds = 255;
  gAutoplayTimeout = millis() + (gAutoplaySeconds * 1000);
  storageWrite("autoplaySeconds");
  broadcastInt("autoplaySeconds", gAutoplaySeconds);
  return String(gAutoplaySeconds);
}

String getAutoplaySeconds() {
  return String(gAutoplaySeconds);
}

// *****************************************
// Pattern
// *****************************************

String setPattern(String value)
{
  gPatternIndex = value.toInt();;
  if (gPatternIndex >= PATTERNS_TOTAL) {
    gPatternIndex = 0;
  }
  if (gAutoplay == false) {
    storageWrite("pattern");
  }
  broadcastInt("pattern", gPatternIndex);
  return String(gPatternIndex);
}

String getPattern() {
  return String(gPatternIndex);
}

String getPatterns() {
  String json = "";
  for (uint8_t i = 0; i < PATTERNS_TOTAL; i++) {

    json += "{\"label\":\"" + g_patterns[i].label + "\",\"name\":\"" + g_patterns[i].name + "\"}";
    if (i < PATTERNS_TOTAL - 1)
      json += ",";
  }
  
  return json;
}

// *****************************************
// PatternByName
// *****************************************

void setPatternByName(String label)
{
  for (uint8_t i = 0; i < PATTERNS_TOTAL; i++) {
    if (g_patterns[i].label == label) {
      setPattern(String(i));
      break;
    }
  }
}

// *****************************************
// HexaLight Name
// *****************************************

String getName() {
  return String(gHostName);
}

// *****************************************
// Fields
// *****************************************

FieldList fields = {
    {"name", "Name", LabelFieldType, 0, 0, getName, NULL, NULL, "all", 255},

    {"power", "Power", BooleanFieldType, 0, 1, getPower, NULL, setPower, "all", 1},
    {"pattern", "Pattern", SelectFieldType, 0, PATTERNS_TOTAL, getPattern, getPatterns, setPattern, "all", 2},
    {"brightness", "Brightness", NumberFieldType, 1, 255, getBrightness, NULL, setBrightness, "all", 3},

    {"autoplaySection", "Autoplay", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "all", 255},
    {"autoplay", "Autoplay Enabled", BooleanFieldType, 0, 1, getAutoplay, NULL, setAutoplay, "all", 4},
    {"autoplaySeconds", "Duration (Seconds)", NumberFieldType, 1, 255, getAutoplaySeconds, NULL, setAutoplaySeconds, "all", 5},

    {"colorPaletteSection", "Color Palette Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "colorPalette", 255},
    {"colorPaletteDelay", "Delay", NumberFieldType, 1, 255, getColorPaletteDelay, NULL, setColorPaletteDelay, "colorPalette", 11},
    {"colorPaletteCycle", "Cycle Colors", BooleanFieldType, 0, 1, getColorPaletteCycle, NULL, setColorPaletteCycle, "colorPalette", 12},
    {"colorPaletteIdenticalTiles", "Identical Tiles", BooleanFieldType, 0, 1, getColorPaletteIdenticalTiles, NULL, setColorPaletteIdenticalTiles, "colorPalette", 13},
    {"colorPalettePalette", "Palette", SelectFieldType, 0, PALETTE_COUNT, getColorPalettePalette, getPalettes, setColorPalettePalette, "colorPalette", 14},

    {"demoReelSection", "Demo Reel Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "demoReel", 255},
    {"demoReelDelay", "Delay", NumberFieldType, 1, 255, getDemoReelDelay, NULL, setDemoReelDelay, "demoReel", 21},
    {"demoReelCycle", "Cycle Colors", BooleanFieldType, 0, 1, getDemoReelCycle, NULL, setDemoReelCycle, "demoReel", 22},
    {"demoReelGlitter", "Glitter", BooleanFieldType, 0, 1, getDemoReelGlitter, NULL, setDemoReelGlitter, "demoReel", 23},
    {"demoReelGlitterChance", "Glitter Chance", NumberFieldType, 1, 255, getDemoReelGlitterChance, NULL, setDemoReelGlitterChance, "demoReel", 24},
    // {"demoReelHue", "Hue", NumberFieldType, 0, 255, getDemoReelHue, NULL, setDemoReelHue, "demoReel", 25},

    {"confettiSection", "Confetti Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "confetti", 255},
    {"confettiDelay", "Delay", NumberFieldType, 1, 255, getConfettiDelay, NULL, setConfettiDelay, "confetti", 31},
    {"confettiCycle", "Cycle Colors", BooleanFieldType, 0, 1, getConfettiCycle, NULL, setConfettiCycle, "confetti", 32},
    {"confettiDots", "Dots", NumberFieldType, 1, 20, getConfettiNumberDots, NULL, setConfettiNumberDots, "confetti", 33},
    {"confettiHue", "Hue", NumberFieldType, 0, 255, getConfettiHue, NULL, setConfettiHue, "confetti", 34},

    {"sinelonSection", "Sinelon Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "sinelon", 255},
    {"sinelonDelay", "Delay", NumberFieldType, 1, 255, getSinelonDelay, NULL, setSinelonDelay, "sinelon", 41},
    {"sinelonCycle", "Cycle Colors", BooleanFieldType, 0, 1, getSinelonCycle, NULL, setSinelonCycle, "sinelon", 42},

    {"juggleSection", "Juggle Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "juggle", 255},
    {"juggleDelay", "Delay", NumberFieldType, 1, 255, getJuggleDelay, NULL, setJuggleDelay, "juggle", 51},
    {"juggleCycle", "Cycle Colors", BooleanFieldType, 0, 1, getJuggleCycle, NULL, setJuggleCycle, "juggle", 52},
    // {"juggleHue", "Hue", NumberFieldType, 0, 255, getJuggleHue, NULL, setJuggleHue, "juggle", 0},

    {"bpmSection", "BPM Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "bpm", 255},
    {"bpmDelay", "Delay", NumberFieldType, 1, 255, getBpmDelay, NULL, setBpmDelay, "bpm", 61},
    {"bpmCycle", "Cycle Colors", BooleanFieldType, 0, 1, getBpmCycle, NULL, setBpmCycle, "bpm", 62},
    {"bpmStep", "Beats Per Minute", NumberFieldType, 1, 255, getBpmBeatsPerMinute, NULL, setBpmBeatsPerMinute, "bpm", 63},
    {"bpmHue", "Hue", NumberFieldType, 0, 255, getBpmHue, NULL, setBpmHue, "bpm", 64},

    {"cylonSection", "Cylon Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "cylon", 255},
    {"cylonDelay", "Delay", NumberFieldType, 1, 255, getCylonDelay, NULL, setCylonDelay, "cylon", 71},
    {"cylonCycle", "Cycle Color", BooleanFieldType, 0, 1, getCylonCycleColor, NULL, setCylonCycleColor, "cylon", 72},
    {"cylonTrail", "Trail Lenght", NumberFieldType, 0, 255, getCylonTrail, NULL, setCylonTrail, "cylon", 73},

    {"solidSection", "Solid Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "solid", 255},
    {"solidRed", "Red", NumberFieldType, 0, 255, getSolidRed, NULL, setSolidRed, "solid", 81},
    {"solidGreen", "Green", NumberFieldType, 0, 255, getSolidGreen, NULL, setSolidGreen, "solid", 82},
    {"solidBlue", "Blue", NumberFieldType, 0, 255, getSolidBlue, NULL, setSolidBlue, "solid", 83},

    {"solidEffectsSection", "Solid Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "solidEffects", 255},
    {"solidEffectsDelay", "Delay", NumberFieldType, 1, 255, getSolidEffectsDelay, NULL, setSolidEffectsDelay, "solidEffects", 91},
    {"solidEffectsCycle", "Cycle Background", BooleanFieldType, 0, 1, getSolidEffectsCycle, NULL, setSolidEffectsCycle, "solidEffects", 92},
    {"solidEffectsDot", "Dot", BooleanFieldType, 0, 1, getSolidEffectsDot, NULL, setSolidEffectsDot, "solidEffects", 93},
    {"solidEffectsDotRandom", "Random Direction", BooleanFieldType, 0, 1, getSolidEffectsDotRandom, NULL, setSolidEffectsDotRandom, "solidEffects", 94},

    {"wholeTileSection", "Whole Tile Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "wholeTile", 255},
    {"wholeTileDelay", "Delay", NumberFieldType, 1, 255, getWholeTileDelay, NULL, setWholeTileDelay, "wholeTile", 101},
    {"wholeTileCycle", "Cycle Colors", BooleanFieldType, 0, 1, getWholeTileCycle, NULL, setWholeTileCycle, "wholeTile", 102},
    {"wholeTileStep", "Step", NumberFieldType, 5, 40, getWholeTileStep, NULL, setWholeTileStep, "wholeTile", 103},
    {"wholeTileHue", "Hue", NumberFieldType, 0, 255, getWholeTileHue, NULL, setWholeTileHue, "wholeTile", 104},

    {"twinklesSection", "Twinkle Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "twinkles", 255},
    {"twinklesDelay", "Delay", NumberFieldType, 1, 255, getTwinklesDelay, NULL, setTwinklesDelay, "twinkles", 111},
    {"twinklesPalette", "Palette", SelectFieldType, 0, PALETTE_COUNT, getTwinklesPalette, getPalettes, setTwinklesPalette, "twinkles", 112},
    {"twinklesFadeIn", "Fade In Speed", NumberFieldType, 0, 255, getTwinklesFadeIn, NULL, setTwinklesFadeIn, "twinkles", 113},
    {"twinklesFadeOut", "Fade Out Speed", NumberFieldType, 0, 255, getTwinklesFadeOut, NULL, setTwinklesFadeOut, "twinkles", 114},
    {"twinklesDensity", "Density", NumberFieldType, 1, 255, getTwinklesDensity, NULL, setTwinklesDensity, "twinkles", 115},

    {"twinkleFoxSection", "Twinkle Fox Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "twinkleFox", 255},
    {"twinkleFoxSpeed", "Speed", NumberFieldType, 0, 8, getTwinkleFoxSpeed, NULL, setTwinkleFoxSpeed, "twinkleFox", 121},
    {"twinkleFoxDensity", "Density", NumberFieldType, 0, 8, getTwinkleFoxDensity, NULL, setTwinkleFoxDensity, "twinkleFox", 122},
    {"twinkleFoxPalette", "Palette", SelectFieldType, 0, PALETTE_COUNT, getTwinkleFoxPalette, getPalettes, setTwinkleFoxPalette, "twinkleFox", 123},
    {"twinkleFoxAutoBack", "Autoselect background", BooleanFieldType, 0, 1, getTwinkleFoxAutoBackground, NULL, setTwinkleFoxAutoBackground, "twinkleFox", 124},
};

const uint8_t FIELD_COUNT = (sizeof(fields) / sizeof((fields)[0]));
