// LED library
#include <FastLED.h>

#define DEBUG true

#ifdef __AVR__
#include <avr/interrupt.h>
#endif

// Digital Pins
#define BODY_LED_IN 3
#define BASKET_LED_IN 5

// LED init settings
#define LED_TYPE WS2812
#define LED_PROFILE GRB

// Variables for the LED strand
const uint8_t gBrightness = 200;
const uint8_t numBodyLED = 128; // Body
const uint8_t numBasketLED = 80; // Basket
CRGBArray<numBodyLED> bodyLEDs;
CRGBArray<numBasketLED> basketLEDs;

// Setup and global variable delcaration for palettes
CRGBPalette16 gPalette;
TBlendType gBlending = LINEARBLEND;
volatile uint8_t gPaletteCounter = 0;
extern const uint8_t numPalettes;

// To control hue globally through a potentiometer input
uint8_t gHue = 0;
uint8_t gSpeed = 0;
uint8_t gIndex = 0;
uint8_t gGlitchIndex = 0;
uint8_t gGlitchSwitch = false;

// For animation switching, this number needs to be hard coded unforunately
const uint8_t numAnimation = 14;
volatile uint8_t gAnimCounter = 0;

// Static animation settings
// TODO make const ??
uint8_t gWidth = 2;


void setup() {
  delay(3000); // Safely power up
  // Testing
  // Serial.begin(57600);
  // Serial.println("Booting up boyoo");

  // Setup strands of LED
  FastLED.addLeds<LED_TYPE, BODY_LED_IN, LED_PROFILE>(bodyLEDs, numBodyLED).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, BASKET_LED_IN, LED_PROFILE>(basketLEDs, numBasketLED).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(gBrightness);
  reset_all();
  FastLED.show();
}

void loop() {
  // Read color from potentiometer input
  EVERY_N_MILLISECONDS(20) {
    gHue++;
  }

  // Stop switching palettes if we are stopped on the animation,
  // but you can still play with the hue on the currently selected
//  if (gAnimCounter > 2) {
  // EVERY_N_SECONDS(5) {
  //   gPaletteCounter = (gPaletteCounter + 1) % numPalettes;
  // }
//  }
  gPaletteCounter = 7;  // Solid color and black
  // gPaletteCounter = 8; // Red & Blue & black smooth
  updateGPalette();


  // EVERY_N_MINUTES(5) {
  //   gAnimCounter = (gAnimCounter + 1) % numAnimation;
  // }


  EVERY_N_MILLISECONDS(200) {
    // gGlitchIndex++;
    if (gGlitchIndex > 16 || gGlitchIndex < 0) {
      gGlitchSwitch = !gGlitchSwitch;
    }

    if (gGlitchSwitch) {
      gGlitchIndex++;
    }
    else {
      gGlitchIndex--;
    }
    // gGlitchIndex = (gGlitchIndex + 1) % 32;
  }

  EVERY_N_MILLISECONDS(30) {
    gIndex++;
  }

  // Select animation to run based on global counter
  gAnimCounter = 6;
  gWidth = 2;
  static uint8_t chase_width = 1;

  switch ( gAnimCounter ) {
    case 0:
      fill_all();
      break;
    case 1:
      fill_all_grad();
      break;
    case 2:
      fill_all_grad_2();
      break;
    case 3:
      theater_chase(gWidth);
      break;
    case 4:
      theater_chase_mir(chase_width, gWidth, 0, 0);
      break;
    case 5:
      flash(48);
      break;
    case 6:
      theater_chase_glitch();
      break;
    case 7:
      theater_chase_glitch_move();
      break;
  }

}
