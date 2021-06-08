// *****************************************
// Palettes
// *****************************************

uint8_t gColorPalettePaletteIndex = 0; 

extern const TProgmemPalette16 RedWhiteBluePalette_p PROGMEM;
extern const TProgmemPalette16 BlackAndWhiteStripedPalette_p PROGMEM;
extern CRGBPalette16 PurpleAndGreenPalette_p;

typedef struct {
  CRGBPalette16 palette;
  String name;
} PaletteAndName;
typedef PaletteAndName PaletteAndNameList[];

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
    BlackAndWhiteStripedPalette_p
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
    "Black and White"
};

CRGB gpp_purple = CHSV( HUE_PURPLE, 255, 255);
CRGB gpp_green  = CHSV( HUE_GREEN, 255, 255);
CRGB gpp_black  = CRGB::Black;

CRGBPalette16 PurpleAndGreenPalette_p = CRGBPalette16(
    gpp_green,  gpp_green,  gpp_black,  gpp_black,
    gpp_purple, gpp_purple, gpp_black,  gpp_black,
    gpp_green,  gpp_green,  gpp_black,  gpp_black,
    gpp_purple, gpp_purple, gpp_black,  gpp_black );

const TProgmemPalette16 RedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
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

String setPalette(String value)
{
  gColorPalettePaletteIndex = value.toInt();;

  if (gColorPalettePaletteIndex >= PALETTE_COUNT)
    gColorPalettePaletteIndex = PALETTE_COUNT - 1;

  return String(gColorPalettePaletteIndex);

}
String getPalette() {
  return String(gColorPalettePaletteIndex);
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


