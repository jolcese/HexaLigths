// *****************************************
// Palettes
// *****************************************

uint8_t gPaletteIndex = 0; 

// A pure "fairy light" palette with some brightness variations
#define HALFFAIRY ((CRGB::FairyLight & 0xFEFEFE) / 2)
#define QUARTERFAIRY ((CRGB::FairyLight & 0xFCFCFC) / 4)
const TProgmemPalette16 FairyLight_p PROGMEM =
{  CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight,
   HALFFAIRY,        HALFFAIRY,        CRGB::FairyLight, CRGB::FairyLight,
   QUARTERFAIRY,     QUARTERFAIRY,     CRGB::FairyLight, CRGB::FairyLight,
   CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight };

// A mostly red palette with green accents and white trim.
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemPalette16 RedGreenWhite_p PROGMEM =
{  CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red,
   CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red,
   CRGB::Red, CRGB::Red, CRGB::Gray, CRGB::Gray,
   CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green };

// A mostly (dark) green palette with red berries.
#define Holly_Green 0x00580c
#define Holly_Red   0xB00402
const TProgmemPalette16 Holly_p PROGMEM =
{  Holly_Green, Holly_Green, Holly_Green, Holly_Green,
   Holly_Green, Holly_Green, Holly_Green, Holly_Green,
   Holly_Green, Holly_Green, Holly_Green, Holly_Green,
   Holly_Green, Holly_Green, Holly_Green, Holly_Red };


const TProgmemPalette16 Snow_p PROGMEM = {
    CRGB::White,
    CRGB::White,
    CRGB::White,
    CRGB::White,
    0x555555,
    0x555555,
    0x555555,
    0x555555,
    0x555555,
    0x555555,
    0x555555,
    0x555555,
    0x555555,
    0x555555,
    0x555555,
    0x555555
};

const TProgmemPalette16 Incandescent_p PROGMEM = {
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024,
    0xE1A024
};

const TProgmemPalette16 PurpleAndGreenPalette_p PROGMEM = {
    CRGB::Green,
    CRGB::Green,
    CRGB::Black,
    CRGB::Black,
    CRGB::Purple,
    CRGB::Purple,
    CRGB::Black,
    CRGB::Black,
    CRGB::Green,
    CRGB::Green,
    CRGB::Black,
    CRGB::Black,
    CRGB::Purple,
    CRGB::Purple,
    CRGB::Black,
    CRGB::Black
};

const TProgmemPalette16 RedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, 
    CRGB::Blue,
    CRGB::Black,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

const TProgmemPalette16 BlackAndWhiteStripedPalette_p PROGMEM =
{
    CRGB::White,
    CRGB::Black, // 'white' is too bright compared to red and blue
    CRGB::Black,
    CRGB::Black,
    CRGB::White,
    CRGB::Black,
    CRGB::Black,
    CRGB::Black,
    CRGB::White,
    CRGB::Black,
    CRGB::Black,
    CRGB::Black,
    CRGB::White,
    CRGB::Black,
    CRGB::Black,
    CRGB::Black
};

// typedef struct {
//   CRGBPalette16 palette;
//   String name;
// } PaletteAndName;
// typedef PaletteAndName PaletteAndNameList[];

const CRGBPalette16 palettes[] = {
    RainbowColors_p,
    RainbowStripeColors_p,
    CloudColors_p,
    LavaColors_p,
    OceanColors_p,
    ForestColors_p,
    PartyColors_p,
    HeatColors_p,
    RedWhiteBluePalette_p,
    PurpleAndGreenPalette_p,
    BlackAndWhiteStripedPalette_p,
    Snow_p,
    Incandescent_p,
    RedGreenWhite_p,
    Holly_p,
    FairyLight_p
};

const uint8_t PALETTE_COUNT = (sizeof(palettes) / sizeof((palettes)[0]));

const String paletteNames[PALETTE_COUNT] = {
    "Rainbow",
    "Rainbow Stripe",
    "Cloud",
    "Lava",
    "Ocean",
    "Forest",
    "Party",
    "Heat",
    "Red White Blue",
    "Purple and Green",
    "Black and White",
    "Snow",
    "Incandescent",
    "Red Green White",
    "Holly",
    "Fairy Light"
}; 

String setPalette(String value)
{
  gPaletteIndex = value.toInt();;

  if (gPaletteIndex >= PALETTE_COUNT)
    gPaletteIndex = PALETTE_COUNT - 1;

  storageWrite(STORAGE_PALETTE, gPaletteIndex);

  return String(gPaletteIndex);

}
String getPalette() {
  return String(gPaletteIndex);
}

String getPalettes() {
  String json = "";

  for (uint8_t i = 0; i < PALETTE_COUNT; i++) {

    json += "{\"label\":\"" + paletteNames[i] + "\"}";
    if (i < PALETTE_COUNT - 1)
      json += ",";
  }

  return json;
}


