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
  storageWrite(STORAGE_POWER, gPowerLed);
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
  storageWrite(STORAGE_BRIGHTNESS, gBrightness);
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
  storageWrite(STORAGE_AUTOPLAY, gAutoplay);
  
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
  storageWrite(STORAGE_AUTOPLAY_SECONDS, gAutoplaySeconds);
  gAutoplayTimeout = millis() + (gAutoplaySeconds * 1000);

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
    gPatternIndex = PATTERNS_TOTAL - 1;
  }
  if (gAutoplay == false) {
    storageWrite(STORAGE_PATTERN, gPatternIndex);
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
    {"name", "Name", LabelFieldType, 0, 0, getName, NULL, NULL, "all"},

    {"power", "Power", BooleanFieldType, 0, 1, getPower, NULL, setPower, "all"},
    {"pattern", "Pattern", SelectFieldType, 0, PATTERNS_TOTAL, getPattern, getPatterns, setPattern, "all"},
    {"brightness", "Brightness", NumberFieldType, 1, 255, getBrightness, NULL, setBrightness, "all"},

    {"autoplaySection", "Autoplay", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "all"},
    {"autoplay", "Autoplay Enabled", BooleanFieldType, 0, 1, getAutoplay, NULL, setAutoplay, "all"},
    {"autoplaySeconds", "Duration (Seconds)", NumberFieldType, 1, 255, getAutoplaySeconds, NULL, setAutoplaySeconds, "all"},

    // {"rainbowDelay", "Delay", NumberFieldType, 1, 255, getRainbowDelay, NULL, setRainbowDelay, "rainbow"},
    // {"rainbowCycle", "Cycle Colors", BooleanFieldType, 0, 1, getRainbowCycle, NULL, setRainbowCycle, "rainbow"},
    // {"rainbowIdenticalTiles", "Identical Tiles", BooleanFieldType, 0, 1, getRainbowIdenticalTiles, NULL, setRainbowIdenticalTiles, "rainbow"},
    
    {"colorPaletteSection", "Color Palette Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "colorPalette"},
    {"colorPaletteDelay", "Delay", NumberFieldType, 1, 255, getColorPaletteDelay, NULL, setColorPaletteDelay, "colorPalette"},
    {"colorPaletteCycle", "Cycle Colors", BooleanFieldType, 0, 1, getColorPaletteCycle, NULL, setColorPaletteCycle, "colorPalette"},
    {"colorPaletteIdenticalTiles", "Identical Tiles", BooleanFieldType, 0, 1, getColorPaletteIdenticalTiles, NULL, setColorPaletteIdenticalTiles, "colorPalette"},
    {"colorPalettePalette", "Palette", SelectFieldType, 0, PALETTE_COUNT, getPalette, getPalettes, setPalette, "colorPalette"},

    {"demoReelSection", "Demo Reel Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "demoReel"},
    {"demoReelDelay", "Delay", NumberFieldType, 1, 255, getDemoReelDelay, NULL, setDemoReelDelay, "demoReel"},
    {"demoReelCycle", "Cycle Colors", BooleanFieldType, 0, 1, getDemoReelCycle, NULL, setDemoReelCycle, "demoReel"},
    {"demoReelGlitter", "Glitter", BooleanFieldType, 0, 1, getDemoReelGlitter, NULL, setDemoReelGlitter, "demoReel"},
    {"demoReelGlitterChance", "Glitter Chance", NumberFieldType, 1, 255, getDemoReelGlitterChance, NULL, setDemoReelGlitterChance, "demoReel"},
    {"demoReelHue", "Hue", NumberFieldType, 0, 255, getDemoReelHue, NULL, setDemoReelHue, "demoReel"},

    {"confettiSection", "Confetti Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "confetti"},
    {"confettiDelay", "Delay", NumberFieldType, 1, 255, getConfettiDelay, NULL, setConfettiDelay, "confetti"},
    {"confettiCycle", "Cycle Colors", BooleanFieldType, 0, 1, getConfettiCycle, NULL, setConfettiCycle, "confetti"},
    {"confettiDots", "Dots", NumberFieldType, 1, 20, getConfettiNumberDots, NULL, setConfettiNumberDots, "confetti"},
    {"confettiHue", "Hue", NumberFieldType, 0, 255, getConfettiHue, NULL, setConfettiHue, "confetti"},

    {"sinelonSection", "Sinelon Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "sinelon"},
    {"sinelonDelay", "Delay", NumberFieldType, 1, 255, getSinelonDelay, NULL, setSinelonDelay, "sinelon"},
    {"sinelonCycle", "Cycle Colors", BooleanFieldType, 0, 1, getSinelonCycle, NULL, setSinelonCycle, "sinelon"},

    {"juggleSection", "Juggle Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "juggle"},
    {"juggleDelay", "Delay", NumberFieldType, 1, 255, getJuggleDelay, NULL, setJuggleDelay, "juggle"},
    {"juggleCycle", "Cycle Colors", BooleanFieldType, 0, 1, getJuggleCycle, NULL, setJuggleCycle, "juggle"},
    // {"juggleStep", "Beats Per Minute", NumberFieldType, 1, 255, getJuggleBeatsPerMinute, NULL, setJuggleBeatsPerMinute, "juggle"},
    // {"juggleHue", "Hue", NumberFieldType, 0, 255, getJuggleHue, NULL, setJuggleHue, "juggle"},

    {"bpmSection", "BPM Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "bpm"},
    {"bpmDelay", "Delay", NumberFieldType, 1, 255, getBpmDelay, NULL, setBpmDelay, "bpm"},
    {"bpmCycle", "Cycle Colors", BooleanFieldType, 0, 1, getBpmCycle, NULL, setBpmCycle, "bpm"},
    {"bpmStep", "Beats Per Minute", NumberFieldType, 1, 255, getBpmBeatsPerMinute, NULL, setBpmBeatsPerMinute, "bpm"},
    {"bpmHue", "Hue", NumberFieldType, 0, 255, getBpmHue, NULL, setBpmHue, "bpm"},

    {"cylonSection", "Cylon Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "cylon"},
    {"cylonDelay", "Delay", NumberFieldType, 1, 255, getCylonDelay, NULL, setCylonDelay, "cylon"},
    {"cylonCycle", "Cycle Color", BooleanFieldType, 0, 1, getCylonCycleColor, NULL, setCylonCycleColor, "cylon"},
    {"cylonTrail", "Trail Lenght", NumberFieldType, 0, 255, getCylonTrail, NULL, setCylonTrail, "cylon"},

    {"solidSection", "Solid Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "solid"},
    {"solidRed", "Red", NumberFieldType, 0, 255, getSolidRed, NULL, setSolidRed, "solid"},
    {"solidGreen", "Green", NumberFieldType, 0, 255, getSolidGreen, NULL, setSolidGreen, "solid"},
    {"solidBlue", "Blue", NumberFieldType, 0, 255, getSolidBlue, NULL, setSolidBlue, "solid"},

    {"solidEffectsSection", "Solid Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "solidEffects"},
    {"solidEffectsDelay", "Delay", NumberFieldType, 1, 255, getSolidEffectsDelay, NULL, setSolidEffectsDelay, "solidEffects"},
    {"solidEffectsCycle", "Cycle Background", BooleanFieldType, 0, 1, getSolidEffectsCycle, NULL, setSolidEffectsCycle, "solidEffects"},
    {"solidEffectsDot", "Dot", BooleanFieldType, 0, 1, getSolidEffectsDot, NULL, setSolidEffectsDot, "solidEffects"},
    {"solidEffectsDotRandom", "Random Direction", BooleanFieldType, 0, 1, getSolidEffectsDotRandom, NULL, setSolidEffectsDotRandom, "solidEffects"},

    {"wholeTileSection", "Whole Tile Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "wholeTile"},
    {"wholeTileDelay", "Delay", NumberFieldType, 1, 255, getWholeTileDelay, NULL, setWholeTileDelay, "wholeTile"},
    {"wholeTileCycle", "Cycle Colors", BooleanFieldType, 0, 1, getWholeTileCycle, NULL, setWholeTileCycle, "wholeTile"},
    {"wholeTileStep", "Step", NumberFieldType, 5, 40, getWholeTileStep, NULL, setWholeTileStep, "wholeTile"},
    {"wholeTileHue", "Hue", NumberFieldType, 0, 255, getWholeTileHue, NULL, setWholeTileHue, "wholeTile"},

    {"twinklesSection", "Twinkle Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "twinkles"},
    {"twinklesDelay", "Delay", NumberFieldType, 1, 255, getTwinklesDelay, NULL, setTwinklesDelay, "twinkles"},
    {"twinklesPalette", "Palette", SelectFieldType, 0, PALETTE_COUNT, getPalette, getPalettes, setPalette, "twinkles"},
    {"twinklesFadeIn", "Fade In Speed", NumberFieldType, 0, 255, getTwinklesFadeIn, NULL, setTwinklesFadeIn, "twinkles"},
    {"twinklesFadeOut", "Fade Out Speed", NumberFieldType, 0, 255, getTwinklesFadeOut, NULL, setTwinklesFadeOut, "twinkles"},
    {"twinklesDensity", "Density", NumberFieldType, 1, 255, getTwinklesDensity, NULL, setTwinklesDensity, "twinkles"},

    {"twinkleFoxSection", "Twinkle Fox Parameters", SectionFieldType, NULL, NULL, NULL, NULL, NULL, "twinkleFox"},
    {"twinkleFoxSpeed", "Speed", NumberFieldType, 0, 8, getTwinkleFoxSpeed, NULL, setTwinkleFoxSpeed, "twinkleFox"},
    {"twinkleFoxDensity", "Density", NumberFieldType, 0, 8, getTwinkleFoxDensity, NULL, setTwinkleFoxDensity, "twinkleFox"},
    {"twinkleFoxPalette", "Palette", SelectFieldType, 0, PALETTE_COUNT, getPalette, getPalettes, setPalette, "twinkleFox"},
    {"twinkleFoxAutoBack", "Autoselect background", BooleanFieldType, 0, 1, getTwinkleFoxAutoBackground, NULL, setTwinkleFoxAutoBackground, "twinkleFox"},

    // {"autoplaySection", "Autoplay", SectionFieldType},
    // {"autoplay", "Autoplay", BooleanFieldType, 0, 1, getAutoplay},
    // {"autoplayDuration", "Autoplay Duration", NumberFieldType, 0, 255, getAutoplayDuration},

    // {"solidColorSection", "Solid Color", SectionFieldType},
    // {"solidColor", "Color", ColorFieldType, 0, 255, getSolidColor},

    // {"fireSection", "Fire & Water", SectionFieldType},
    // {"cooling", "Cooling", NumberFieldType, 0, 255, getCooling},
    // {"sparking", "Sparking", NumberFieldType, 0, 255, getSparking},

    // {"twinklesSection", "Twinkles", SectionFieldType},
    // {"twinkleSpeed", "Twinkle Speed", NumberFieldType, 0, 8, getTwinkleSpeed},
    // {"twinkleDensity", "Twinkle Density", NumberFieldType, 0, 8, getTwinkleDensity},
    // {"coolLikeIncandescent", "Incandescent Cool", BooleanFieldType, 0, 1, getCoolLikeIncandescent},


    // {"prideSection", "Pride", SectionFieldType},

    // {"saturationBpm", "Saturation BPM", NumberFieldType, 0, 255, getSaturationBpm, NULL, setSaturationBpm},
    // {"saturationMin", "Saturation Min", NumberFieldType, 0, 255, getSaturationMin, NULL, setSaturationMin},
    // {"saturationMax", "Saturation Max", NumberFieldType, 0, 255, getSaturationMax, NULL, setSaturationMax},

    // {"brightDepthBpm", "Brightness Depth BPM", NumberFieldType, 0, 255, getBrightDepthBpm, NULL, setBrightDepthBpm},
    // {"brightDepthMin", "Brightness Depth Min", NumberFieldType, 0, 255, getBrightDepthMin, NULL, setBrightDepthMin},
    // {"brightDepthMax", "Brightness Depth Max", NumberFieldType, 0, 255, getBrightDepthMax, NULL, setBrightDepthMax},

    // {"brightThetaIncBpm", "Bright Theta Inc BPM", NumberFieldType, 0, 255, getBrightThetaIncBpm, NULL, setBrightThetaIncBpm},
    // {"brightThetaIncMin", "Bright Theta Inc Min", NumberFieldType, 0, 255, getBrightThetaIncMin, NULL, setBrightThetaIncMin},
    // {"brightThetaIncMax", "Bright Theta Inc Max", NumberFieldType, 0, 255, getBrightThetaIncMax, NULL, setBrightThetaIncMax},

    // {"msMultiplierBpm", "Time Multiplier BPM", NumberFieldType, 0, 255, getMsMultiplierBpm, NULL, setMsMultiplierBpm},
    // {"msMultiplierMin", "Time Multiplier Min", NumberFieldType, 0, 255, getMsMultiplierMin, NULL, setMsMultiplierMin},
    // {"msMultiplierMax", "Time Multiplier Max", NumberFieldType, 0, 255, getMsMultiplierMax, NULL, setMsMultiplierMax},

    // {"hueIncBpm", "Hue Inc BPM", NumberFieldType, 0, 255, getHueIncBpm, NULL, setHueIncBpm},
    // {"hueIncMin", "Hue Inc Min", NumberFieldType, 0, 255, getHueIncMin, NULL, setHueIncMin},
    // {"hueIncMax", "Hue Inc Max", NumberFieldType, 0, 255, getHueIncMax, NULL, setHueIncMax},

    // {"sHueBpm", "S Hue BPM", NumberFieldType, 0, 255, getSHueBpm, NULL, setSHueBpm},
    // {"sHueMin", "S Hue Min", NumberFieldType, 0, 255, getSHueMin, NULL, setSHueMin},
    // {"sHueMax", "S Hue Max", NumberFieldType, 0, 255, getSHueMax, NULL, setSHueMax},
};

const uint8_t FIELD_COUNT = (sizeof(fields) / sizeof((fields)[0]));
