// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PINA            6
#define PINB            7
#define PINC            8
#define PIND            9
#define PINE            10
#define PINF            11
#define GREENVALUE      150
#define REDVALUE        255
#define NOCOLORVALUE     0
# define RANDOMLOWERLIMIT  0
# define RANDOMUPPERLIMIT  5
# define NUMBEROFSECTORS  6


// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      7

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixelsA = Adafruit_NeoPixel(NUMPIXELS, PINA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsB = Adafruit_NeoPixel(NUMPIXELS, PINB, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsC = Adafruit_NeoPixel(NUMPIXELS, PINC, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsD = Adafruit_NeoPixel(NUMPIXELS, PIND, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsE = Adafruit_NeoPixel(NUMPIXELS, PINE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsF = Adafruit_NeoPixel(NUMPIXELS, PINF, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
Adafruit_NeoPixel *ledStripes[] = {&pixelsA, &pixelsB, &pixelsC,&pixelsD,&pixelsE,&pixelsF};
int randomValue = -1;


void ledShineGapColor(int i, Adafruit_NeoPixel *pixels, int r, int g, int b){
  pixels->setPixelColor(i, pixels->Color(r,g,b));//0,150,0
}

void ledShineWallColor(int i, Adafruit_NeoPixel *pixels, int r, int g, int b){
  pixels->setPixelColor(i, pixels->Color(r,g,b)); // 255,0,0
}

void ledOff( int i, Adafruit_NeoPixel *pixels){
  pixels->setPixelColor(i, pixels->Color(0,0,0));
}

void allLedOff(){
  for (int y=0;y<NUMBEROFSECTORS;y++){
    for (int i =0; i<NUMPIXELS;i++){
      ledOff(i,ledStripes[y]);
    }
    ledStripes[y]->show();
  }
    
}

void ledsShininginDifferentColors(int i, int chosenLedSector){
  for (int y=0;y<NUMBEROFSECTORS;y++){
    if(y == chosenLedSector){
      ledShineGapColor( i, ledStripes[y], NOCOLORVALUE,GREENVALUE,NOCOLORVALUE);
      continue;
    }
    ledShineWallColor( i,  ledStripes[y], REDVALUE, NOCOLORVALUE,NOCOLORVALUE);
  }
}

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixelsA.begin(); // This initializes the NeoPixel library.
  pixelsB.begin();
  pixelsC.begin();
  pixelsD.begin();
  pixelsE.begin();
  pixelsF.begin();
  Serial.begin(9600);
}

void loop() {
  for(int i=0;i<NUMPIXELS;i++){
    if(i <=3){
     randomValue = (rand () % ((RANDOMUPPERLIMIT + 1) - RANDOMLOWERLIMIT)) + RANDOMLOWERLIMIT;
    }

    ledsShininginDifferentColors(i,randomValue);

    pixelsA.show(); // This sends the updated pixel color to the hardware.
    pixelsB.show();
    pixelsC.show();
    pixelsD.show();
    pixelsE.show();
    pixelsF.show(); 
  }
  delay(1000);
  
}
