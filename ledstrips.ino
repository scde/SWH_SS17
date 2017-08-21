#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define   STRIP_LENGTH      7

#define   PIN_STRIP_1_2     11
#define   PIN_STRIP_3_4     12
#define   PIN_STRIP_5_6     8
#define   PIN_STRIP_7_8     7
#define   PIN_STRIP_9_10    6
#define   PIN_STRIP_11_12   10

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip_1_2 = Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP_1_2, NEO_GRB + NEO_KHZ800); // Sector 4
Adafruit_NeoPixel strip_3_4 = Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP_3_4, NEO_GRB + NEO_KHZ800); // Sector 5
Adafruit_NeoPixel strip_5_6 = Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP_5_6, NEO_GRB + NEO_KHZ800); // Sector 2
Adafruit_NeoPixel strip_7_8 = Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP_7_8, NEO_GRB + NEO_KHZ800); // Sector 1
Adafruit_NeoPixel strip_9_10 = Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP_9_10, NEO_GRB + NEO_KHZ800); // Sector 0
Adafruit_NeoPixel strip_11_12 = Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP_11_12, NEO_GRB + NEO_KHZ800); // Sector 3

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
    Serial.begin(9600);

    strip_1_2.begin();
    strip_3_4.begin();
    strip_5_6.begin();
    strip_7_8.begin();
    strip_9_10.begin();
    strip_11_12.begin();

    strip_1_2.show();
    strip_3_4.show();
    strip_5_6.show();
    strip_7_8.show();
    strip_9_10.show();
    strip_11_12.show();
}

void loop() {
    Serial.println("test");
    //  // Some example procedures showing how to display to the pixels:
    //  colorWipe(strip.Color(255, 0, 0), 50); // Red
    //  colorWipe(strip.Color(0, 255, 0), 50); // Green
    //  colorWipe(strip.Color(0, 0, 255), 50); // Blue
    ////colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
    //  // Send a theater pixel chase in...
    //  theaterChase(strip.Color(127, 127, 127), 50); // White
    //  theaterChase(strip.Color(127, 0, 0), 50); // Red
    //  theaterChase(strip.Color(0, 0, 127), 50); // Blue

    rainbowCollective(20, &strip_1_2);
    rainbowCollective(20, &strip_3_4);
    rainbowCollective(20, &strip_5_6);
    rainbowCollective(20, &strip_7_8);
    rainbowCollective(20, &strip_9_10);
    rainbowCollective(20, &strip_11_12);
    //  rainbowCycle(20);
    //  theaterChaseRainbow(50);
}

void rainbowCollective(uint8_t wait, Adafruit_NeoPixel *strip) {
    uint16_t i, j;

    for(j=0; j<256; j++) {
        for(i=0; i<strip->numPixels(); i++) {
            strip->setPixelColor(i, Wheel((i+j) & 255, strip));
        }
        strip->show();
        delay(wait);
    }
}

//// Fill the dots one after the other with a color
//void colorWipe(uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
//    strip.setPixelColor(i, c);
//    strip.show();
//    delay(wait);
//  }
//}

//void rainbow(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) {
//    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel((i+j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
//// Slightly different, this makes the rainbow equally distributed throughout
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
//    for(i=0; i< strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
////Theatre-style crawling lights.
//void theaterChase(uint32_t c, uint8_t wait) {
//  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, c);    //turn every third pixel on
//      }
//      strip.show();
//
//      delay(wait);
//
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
//
////Theatre-style crawling lights with rainbow effect
//void theaterChaseRainbow(uint8_t wait) {
//  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
//      }
//      strip.show();
//
//      delay(wait);
//
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, Adafruit_NeoPixel *strip) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}}}}}}}}}}}}}}}
