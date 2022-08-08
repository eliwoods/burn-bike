// Some utility shit
uint8_t triwave(uint8_t in) {
  if ( in < 64) {
    return in;
  }
  else if (in >= 64 && in < 128) {
    return (127 - in);
  }
  else if (in >= 128 && in < 192) {
    return (in - 128);
  }
  else if (in >= 192) {
    return (255 - in);
  }
}


// Reset everything aka set everything to black
void reset_all() {
  fill_solid(bodyLEDs, numBodyLED, CRGB::Black);
  fill_solid(basketLEDs, numBasketLED, CRGB::Black);
}


// Fills all LEDs with a single color.
// Change in color is controlled by gHue
void fill_all() {
  fill_solid(bodyLEDs, numBodyLED, ColorFromPalette(gPalette, gIndex, gBrightness, gBlending));
  fill_solid(basketLEDs, numBasketLED, ColorFromPalette(gPalette, gIndex, gBrightness, gBlending));
  FastLED.show();
}


// Fills all LEDs with a gradient between two colors 180 degrees apart
// Change in color is controlled by gHue
// Ignores current palette setting
void fill_all_grad() {
  // Grab the colors
  CRGB col_start = CHSV(gHue, 255, gBrightness);
  CRGB col_end = CHSV((gHue + 128) % 255, 255, gBrightness);
  fill_gradient_RGB(bodyLEDs, numBodyLED, col_start, col_end);
  fill_gradient_RGB(basketLEDs, numBasketLED, col_start, col_end);

  FastLED.show();
}


// This function fills a mirrored gradient, even though it calculates 4 different colors
// Hue is updated by gHue
// Ignores palette setting
void fill_all_grad_2() {
  CRGB col0 = CHSV(gHue, 255, gBrightness);
  CRGB col1 = CHSV((gHue + 64) % 255, 255, gBrightness);
  CRGB col2 = CHSV((gHue + 128) % 255, 255, gBrightness);
  CRGB col3 = CHSV((gHue + 192) % 255, 255, gBrightness);

  fill_gradient_RGB(bodyLEDs, numBodyLED/2, col0, col1);
  fill_gradient_RGB(bodyLEDs + numBodyLED/2, numBodyLED/2, col1, col0);

  fill_gradient_RGB(basketLEDs, numBasketLED/2, col0, col1);
  fill_gradient_RGB(basketLEDs + numBasketLED/2, numBasketLED/2, col1, col0);

  FastLED.show();
}

///////////////////////////////////////////////////////////////////////////////////////////
// All of the following animations are some kind of theater marquee style. This means    //
// at the core, its  packets of light chasing eachother across the strip, although we'll //
// change up how they chase in different animations.                                     //
///////////////////////////////////////////////////////////////////////////////////////////

// Simple theater chase where packets move continuously
// Obeys the current palette setting
void theater_chase(uint8_t width) {
  fill_palette(bodyLEDs, numBodyLED, gIndex, width, gPalette, gBrightness, gBlending);
  fill_palette(basketLEDs, numBasketLED, gIndex, width*3, gPalette, gBrightness, gBlending);
  FastLED.show();
}


// Obeys the current palette setting
void theater_chase_glitch() {
  fill_palette(bodyLEDs, numBodyLED, 20, gGlitchIndex, gPalette, gBrightness, gBlending);
  fill_palette(basketLEDs, numBasketLED, 0, gGlitchIndex, gPalette, gBrightness, gBlending);
  FastLED.show();
}


// Obeys the current palette setting
void theater_chase_glitch_move() {
  fill_palette(bodyLEDs, numBodyLED, gIndex, gGlitchIndex, gPalette, gBrightness, gBlending);
  fill_palette(basketLEDs, numBasketLED, gIndex, gGlitchIndex, gPalette, gBrightness, gBlending);
  FastLED.show();
}


// TODO FIX THIS SHIT, BROKE IT WHEN ADDING THE SECOND STRIP
// Theater chase mirrored about the center of LED strip (assumes even number)
// Obeys current palette setting
void theater_chase_mir(uint8_t splits, uint8_t width, uint8_t offset, uint8_t func) {
  uint8_t numSection = 2*splits;
  // Body
  uint8_t lenBodySection = numBodyLED/numSection;
  uint8_t bodyStart = (0 + offset) % numBodyLED;
  uint8_t bodyEnd = ((numBodyLED/numSection) - 1 + offset) % numBodyLED;

  // Basket
  uint8_t lenBasketSection = numBasketLED/numSection;
  uint8_t basketStart = (0 + offset) % numBasketLED;
  uint8_t basketEnd = ((numBasketLED/numSection) - 1 + offset) % numBasketLED;

  switch(func) {
    case 0:
      fill_palette(bodyLEDs(bodyStart, bodyEnd), lenBodySection, gIndex, width, gPalette, gBrightness, gBlending);
      fill_palette(basketLEDs(basketStart, basketEnd), lenBasketSection, gIndex, width*3, gPalette, gBrightness, gBlending);
      break;
    case 1:
      fill_palette(bodyLEDs(bodyStart, bodyEnd), lenBodySection, triwave8(gIndex), width, gPalette, gBrightness, gBlending);
      fill_palette(basketLEDs(basketStart, basketEnd), lenBasketSection, triwave8(gIndex), width*3, gPalette, gBrightness, gBlending);
      break;
  }

  uint8_t prevBodyStart, prevBodyEnd, prevBasketStart, prevBasketEnd;
  for (uint8_t i = 0; i < numSection; i++) {
    prevBodyStart = bodyStart;
    prevBodyEnd = bodyEnd;
    bodyStart = bodyEnd + 1;
    bodyEnd = bodyStart + lenBodySection - 1;
    bodyLEDs(bodyStart, bodyEnd) = bodyLEDs(prevBodyEnd, prevBodyStart);

    prevBasketStart = basketStart;
    prevBasketEnd = basketEnd;
    basketStart = basketEnd + 1;
    basketEnd = basketStart + lenBasketSection - 1;
    basketLEDs(basketStart, basketEnd) = basketLEDs(prevBasketEnd, prevBasketStart);
  }

  FastLED.show();
}

// Flashes like some kinda hazard mode
void flash(uint8_t inc) {
  static uint8_t brightness = 0;
  static uint8_t cIndex = 0;

  EVERY_N_MILLISECONDS(70) {
    brightness += inc;
  }

  fill_solid(bodyLEDs, numBodyLED, ColorFromPalette(gPalette, cIndex, triwave(brightness), gBlending));
  fill_solid(basketLEDs, numBasketLED, ColorFromPalette(gPalette, cIndex, triwave(brightness), gBlending));
  FastLED.show();
}