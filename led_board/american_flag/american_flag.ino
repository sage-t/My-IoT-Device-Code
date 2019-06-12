#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 12

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(143, PIN, NEO_RGB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  Serial.begin(9600);
  
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  americanFlag();
  delay(100);
}

void americanFlag() {
  int row = 0;
  bool left = true;
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (i % 11 == 0) {
      row++;
    }

    Serial.println(i);
    Serial.println(row);
    
    if (i % 11 < 5 && row % 2 == 1) {
      left = true;
    } else if (i % 11 > 5 && row % 2 == 0) {
      left = true;
    } else {
      left = false;
    }

    if (row > 6 && left) {
      if (i % 2) {
        strip.setPixelColor(i, strip.Color(0, 0, 255)); // blue
      } else {
        strip.setPixelColor(i, strip.Color(60, 60, 255)); // white
      }
    } else if (row % 2) {
      strip.setPixelColor(i, strip.Color(0, 255, 0)); // red
    } else {
      strip.setPixelColor(i, strip.Color(255, 255, 255)); // white
    }

    strip.show();
    
//    strip.setPixelColor(i, c);
//    strip.show();
//    delay(wait);
  }
}


