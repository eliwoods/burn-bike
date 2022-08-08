// Just to clarify comments, I'm describing the palette pattern
// in terms of the palettes index. Or it'll be copied from the
// PaletteKnife shit
//
// W == White
// B == Black
// RB == Rainbow, uses global hue position
// X == Input to the function

//////////////////////////////////////////////////////////////////////////////////
// These template need to be updated before each fill call. This is because     //
// they are either dependend on an input or they rely on some global variable   //
// that is changed in the animations.                                           //
//////////////////////////////////////////////////////////////////////////////////


// RB|B|B|B repeating
void update_RainbowBlack_p() {
  // Instead of blacking out, fill backgroud with rainbow, then
  // fill every thing with black except
  fill_palette(gPalette, 16, gHue, 16, RainbowColors_p, gBrightness, gBlending);
  for (uint8_t i = 0 ; i < 16; i++) {
    if ( i % 4 == 0) {
      continue;
    }
    gPalette[i] = CRGB::Black;
  }
}

void update_Rainbow_p() {
  fill_palette(gPalette, 16, gHue, 16, RainbowColors_p, gBrightness, gBlending);
}

// W|RB|RB|RB repeating
void update_WhiteRainbow_p() {
  fill_palette(gPalette, 16, gHue, 16, RainbowColors_p, gBrightness, gBlending);
  gPalette[0] = CHSV(255, 0, gBrightness);
  gPalette[4] = CHSV(255, 0, gBrightness);
  gPalette[8] = CHSV(255, 0, gBrightness);
  gPalette[12] = CHSV(255, 0, gBrightness);
}


//////////////////////////////////////
//    Hue adjustable palettes!!!    //
//////////////////////////////////////

// X_LEAD|X_BACK|X_BACK|X_BACK repeating
void update_WhiteCol_p() {
  fill_solid( gPalette, 16, CHSV(gHue, 255, gBrightness));
  gPalette[0] = CHSV(255, 0, gBrightness);
  gPalette[4] = CHSV(255, 0, gBrightness);
  gPalette[8] = CHSV(255, 0, gBrightness);
  gPalette[12] = CHSV(255, 0, gBrightness);
}

void update_TwoCol_p() {
  for (uint8_t i = 0; i < 16; i++) {
    if (i % 4 == 0) {
      gPalette[i] = CHSV(gHue, 255, gBrightness);
    }
    else {
      gPalette[i] = CHSV((gHue + 128) % 255, 255, gBrightness);
    }
  }
}

void update_FourCol_p() {
  for (uint8_t i = 0; i < 16; i++) {
    gPalette[i] = CHSV((gHue + 64*(i % 4)) % 255, 255, gBrightness);
  }

}

// One color at the front and nothing else
void update_ColLead_p() {
  for (uint8_t i = 0; i < 16; i++) {
    if (i % 4 == 0) {
      gPalette[i] = CHSV(gHue, 255, gBrightness);
    }
    else {
      gPalette[i] = CRGB::Black;
    }
  }
}

void update_ColBlack_p() {
  for (uint8_t i = 0; i < 16; i++) {
    if (i % 4 == 0) {
      if (DEBUG) {
        // gPalette[i] = CRGB::Green;
        gPalette[i] = CRGB::Red;
      }
      else {
        gPalette[i] = CHSV(255, 0, gBrightness);
      }
    }
    else {
      gPalette[i] = CRGB::Black;
    }
  }
}


void update_Col2Black_p() {
  for (uint8_t i = 0; i < 16; i++) {
    if (i % 4 == 0) {
      gPalette[i] = CRGB::Red;
    }
    else {
      gPalette[i] = CRGB::Black;
    }

    if (i % 8 == 0) {
      gPalette[i] = CRGB::Blue;
    }
  }
}

// This has to be hand counted and updated whenever we add more palettes. There are also some predefined palettes
// that need to get accounted for :(
const uint8_t numPalettes = 28;

// This handles the switching of palettes based on the global palette counter. Need to add palettes to this function
// whenever they are added up above. Declare it down here so we can get away with not having to use the Gradient Palette
// Pointer array.
void updateGPalette() {
  switch (gPaletteCounter) {
    case 0:
      update_RainbowBlack_p();
      break;
    case 1:
      update_WhiteRainbow_p();
      break;
    case 2:
      update_Rainbow_p();
      break;
    case 3:
      update_WhiteCol_p();
      break;
    case 4:
      update_TwoCol_p();
      break;
    case 5:
      update_FourCol_p();
      break;
    case 6:
      update_ColLead_p();
      break;
    case 7:
      update_ColBlack_p();
      break;
    case 8:
      update_Col2Black_p();
      break;
  }
}
