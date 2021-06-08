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

// uint8_t power = 1;
// uint8_t brightness = brightnessMap[brightnessIndex];

// *****************************************
// Power
// *****************************************

String setPower(String value) {
 gPowerLed = value.toInt();
//  if(gPowerLed < 0) gPowerLed = 0;
//  else if (power > 1) power = 1;
  storageWrite(STORAGE_POWER, gPowerLed);
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
  if(gBrightness < 0) gBrightness = 0;
  else if (gBrightness > 255) gBrightness = 255;
  FastLED.setBrightness( gBrightness );

  storageWrite(STORAGE_BRIGHTNESS, gBrightness);

  return String(gBrightness);
}

String getBrightness() {
  return String(gBrightness);
}

// *****************************************
// Pattern
// *****************************************

String setPattern(String value)
{
  gPatternIndex = value.toInt();;

  if (gPatternIndex >= PATTERNS_TOTAL)
    gPatternIndex = PATTERNS_TOTAL - 1;

  // if (autoplay == 0) {
  //   EEPROM.write(1, currentPatternIndex);
  //   EEPROM.commit();
  // }
  storageWrite(STORAGE_PATTERN, gPatternIndex);

  // broadcastInt("pattern", currentPatternIndex);
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
  return gHostNameString;
}

// *****************************************
// Fields
// *****************************************

FieldList fields = {
    {"name", "Name", LabelFieldType, 0, 0, getName, NULL, NULL, "all"},

    {"power", "Power", BooleanFieldType, 0, 1, getPower, NULL, setPower, "all"},
    {"pattern", "Pattern", SelectFieldType, 0, PATTERNS_TOTAL, getPattern, getPatterns, setPattern, "all"},
    {"brightness", "Brightness", NumberFieldType, 1, 255, getBrightness, NULL, setBrightness, "all"},

    {"customSection", "Pattern Specific Parameters", SectionFieldType},

    {"rainbowDelay", "Delay", NumberFieldType, 1, 255, getRainbowDelay, NULL, setRainbowDelay, "rainbow"},
    {"rainbowCycle", "Cycle Colors", BooleanFieldType, 0, 1, getRainbowCycle, NULL, setRainbowCycle, "rainbow"},
    {"rainbowIdenticalTiles", "Identical Tiles", BooleanFieldType, 0, 1, getRainbowIdenticalTiles, NULL, setRainbowIdenticalTiles, "rainbow"},
    
    {"colorPaletteDelay", "Delay", NumberFieldType, 1, 255, getColorPaletteDelay, NULL, setColorPaletteDelay, "colorPalette"},
    {"colorPaletteCycle", "Cycle Colors", BooleanFieldType, 0, 1, getColorPaletteCycle, NULL, setColorPaletteCycle, "colorPalette"},
    {"colorPaletteIdenticalTiles", "Identical Tiles", BooleanFieldType, 0, 1, getColorPaletteIdenticalTiles, NULL, setColorPaletteIdenticalTiles, "colorPalette"},
    {"colorPalettePalette", "Palette", SelectFieldType, 0, PALETTE_COUNT, getPalette, getPalettes, setPalette, "colorPalette"},

    {"demoReelDelay", "Delay", NumberFieldType, 1, 255, getDemoReelDelay, NULL, setDemoReelDelay, "demoReel"},
    {"demoReelCycle", "Cycle Colors", BooleanFieldType, 0, 1, getDemoReelCycle, NULL, setDemoReelCycle, "demoReel"},
    {"demoReelGlitter", "Glitter", BooleanFieldType, 0, 1, getDemoReelGlitter, NULL, setDemoReelGlitter, "demoReel"},
    {"demoReelGlitterChance", "Glitter Chance", NumberFieldType, 1, 255, getDemoReelGlitterChance, NULL, setDemoReelGlitterChance, "demoReel"},
    {"demoReelHue", "Hue", NumberFieldType, 0, 255, getDemoReelHue, NULL, setDemoReelHue, "demoReel"},

    {"confettiDelay", "Delay", NumberFieldType, 1, 255, getConfettiDelay, NULL, setConfettiDelay, "confetti"},
    {"confettiCycle", "Cycle Colors", BooleanFieldType, 0, 1, getConfettiCycle, NULL, setConfettiCycle, "confetti"},
    {"confettiDots", "Dots", NumberFieldType, 1, 20, getConfettiNumberDots, NULL, setConfettiNumberDots, "confetti"},
    {"confettiHue", "Hue", NumberFieldType, 0, 255, getConfettiHue, NULL, setConfettiHue, "confetti"},

    {"sinelonDelay", "Delay", NumberFieldType, 1, 255, getSinelonDelay, NULL, setSinelonDelay, "sinelon"},
    {"sinelonCycle", "Cycle Colors", BooleanFieldType, 0, 1, getSinelonCycle, NULL, setSinelonCycle, "sinelon"},

    {"juggleDelay", "Delay", NumberFieldType, 1, 255, getJuggleDelay, NULL, setJuggleDelay, "juggle"},
    {"juggleCycle", "Cycle Colors", BooleanFieldType, 0, 1, getJuggleCycle, NULL, setJuggleCycle, "juggle"},
    // {"juggleStep", "Beats Per Minute", NumberFieldType, 1, 255, getJuggleBeatsPerMinute, NULL, setJuggleBeatsPerMinute, "juggle"},
    // {"juggleHue", "Hue", NumberFieldType, 0, 255, getJuggleHue, NULL, setJuggleHue, "juggle"},

    {"bpmDelay", "Delay", NumberFieldType, 1, 255, getBpmDelay, NULL, setBpmDelay, "bpm"},
    {"bpmCycle", "Cycle Colors", BooleanFieldType, 0, 1, getBpmCycle, NULL, setBpmCycle, "bpm"},
    {"bpmStep", "Beats Per Minute", NumberFieldType, 1, 255, getBpmBeatsPerMinute, NULL, setBpmBeatsPerMinute, "bpm"},
    {"bpmHue", "Hue", NumberFieldType, 0, 255, getBpmHue, NULL, setBpmHue, "bpm"},

    {"cylonDelay", "Delay", NumberFieldType, 1, 255, getCylonDelay, NULL, setCylonDelay, "cylon"},
    {"cylonCycle", "Cycle Color", BooleanFieldType, 0, 1, getCylonCycleColor, NULL, setCylonCycleColor, "cylon"},
    {"cylonTrail", "Trail Lenght", NumberFieldType, 0, 255, getCylonTrail, NULL, setCylonTrail, "cylon"},

    {"SolidRed", "Red", NumberFieldType, 0, 255, getSolidRed, NULL, setSolidRed, "solid"},
    {"SolidGreen", "Green", NumberFieldType, 0, 255, getSolidGreen, NULL, setSolidGreen, "solid"},
    {"SolidBlue", "Blue", NumberFieldType, 0, 255, getSolidBlue, NULL, setSolidBlue, "solid"},

    {"solidEffectsDelay", "Delay", NumberFieldType, 1, 255, getSolidEffectsDelay, NULL, setSolidEffectsDelay, "solidEffects"},
    {"solidEffectsCycle", "Cycle Background", BooleanFieldType, 0, 1, getSolidEffectsCycle, NULL, setSolidEffectsCycle, "solidEffects"},
    {"solidEffectsDot", "Dot", BooleanFieldType, 0, 1, getSolidEffectsDot, NULL, setSolidEffectsDot, "solidEffects"},
    {"solidEffectsDotRandom", "Random Direction", BooleanFieldType, 0, 1, getSolidEffectsDotRandom, NULL, setSolidEffectsDotRandom, "solidEffects"},

    {"wholeTileDelay", "Delay", NumberFieldType, 1, 255, getWholeTileDelay, NULL, setWholeTileDelay, "wholeTile"},
    {"wholeTileCycle", "Cycle Colors", BooleanFieldType, 0, 1, getWholeTileCycle, NULL, setWholeTileCycle, "wholeTile"},
    {"wholeTileStep", "Step", NumberFieldType, 5, 40, getWholeTileStep, NULL, setWholeTileStep, "wholeTile"},
    {"wholeTileHue", "Hue", NumberFieldType, 0, 255, getWholeTileHue, NULL, setWholeTileHue, "wholeTile"},

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
