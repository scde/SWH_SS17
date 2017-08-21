// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include "FastLED.h"

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define   PIN_6                6
#define   PIN_7                7
#define   PIN_8                8
#define   PIN_9                9
#define   PIN_10               10
#define   PIN_11               11

#define   GREENVALUE           150
#define   REDVALUE             255
#define   NOCOLORVALUE         0

#define   RANDOMLOWERLIMIT     0
#define   RANDOMUPPERLIMIT     5

// How many NeoPixels are attached to the Arduino?
#define   NUM_LEDS_PER_STRIP   7
#define   NUM_SECTORS          6

#define   FIRST_FIELD          0

#define   PATH_PATTERN         0
#define   WALL_PATTERN_1       1
#define   WALL_PATTERN_2       2
#define   WALL_PATTERN_3       3

CRGB strips[NUM_SECTORS][NUM_LEDS_PER_STRIP];

bool wallSpawn;

// game update speed (wall spawn/move time)
int gameSpeed;
unsigned long lastGameUpdate;
/*int colorUpdateTime;*/
/*unsigned long lastColorUpdate;*/

/*unsigned long curMillis;*/
/*unsigned long prevMillis;*/
/*int deltaTime;*/

byte rWall;
byte gWall;
byte bWall;

byte rPath;
byte gPath;
byte bPath;

void setup() {
    Serial.begin(9600);

    /*curMillis = millis();*/
    /*prevMillis = millis();*/
    /*deltaTime = 0;*/

    wallSpawn = false;

    gameSpeed = 2000;
    lastGameUpdate = 0;

    /*colorUpdateTime = 20000;*/
    /*lastColorUpdate = 0;*/

    rWall = 255;
    gWall = 0;
    bWall = 0;

    rPath = 0;
    gPath = 255;
    bPath = 0;

    // initialize strips to off
    FastLED.addLeds<NEOPIXEL, PIN_6>(strips[0], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, PIN_7>(strips[1], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, PIN_8>(strips[2], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, PIN_9>(strips[3], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, PIN_10>(strips[4], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, PIN_11>(strips[5], NUM_LEDS_PER_STRIP);
}

void loop() {
    /*for (byte sector = 0; sector < NUM_SECTORS; sector++) {*/
        /*for (byte field = 0; field < NUM_LEDS_PER_STRIP; field++) {*/
            /*strips[sector][field].setRGB(random(0,255), random(0,255), random(0,255));*/
        /*}*/
    /*}*/
    /*FastLED.show();*/
    /*delay (2000);*/

    /*curMillis = millis();*/
    /*deltaTime = prevMillis - curMillis;*/
    /*prevMillis = curMillis;*/

    // FIXME use FastLED color sets that fit together
    /*if (millis() - lastColorUpdate >= colorUpdateTime) {*/
        /*lastColorUpdate = millis();*/
        /*randomizeColors();*/
    /*}*/

    if (millis() - lastGameUpdate >= gameSpeed) {
        lastGameUpdate = millis();

        // moves colors out one field and overwrites the last one
        for (byte sector = 0; sector < NUM_SECTORS; sector++) {
            memmove(&strips[sector][FIRST_FIELD + 1],
                    &strips[sector][FIRST_FIELD],
                    (NUM_LEDS_PER_STRIP - 1) * sizeof( CRGB));
        }

        if (wallSpawn) {
            spawnPattern(random(WALL_PATTERN_1, WALL_PATTERN_3));
        }
        else {
            spawnPattern(PATH_PATTERN);
        }
        wallSpawn = !wallSpawn;

        FastLED.show();

        /*for (byte field = 0; field < NUM_LEDS_PER_STRIP; field++) {*/
            /*for (byte sector = 0; sector < STRIPS_LEN; sector++) {*/
                /*uint32_t prevColor = strips[sector].getPixelColor(field);*/
                /*ledColorize(field, &strips[sector], rWall, gWall, bWall);*/
            /*}*/
        /*}*/

        /*// move colors one row ahead*/
        /*for (byte sector = 0; sector < STRIPS_LEN; sector++) {*/
            /*for (byte field = NUM_LEDS_PER_STRIP - 1; field > 0; field--) {*/
                /*colors[sector][field] = colors[sector][field - 1];*/
            /*}*/
        /*}*/

        /*// fill inner circle*/
        /*for (byte sector = 0; sector < STRIPS_LEN; sector++) {*/
            /*colors[sector][0] = ;*/
        /*}*/

        /*for (byte i = 0; i < STRIPS_LEN; i++) {*/
            /*strips[i].show();*/
        /*}*/
    }

    delay(10);
}

void spawnPattern(byte patternType) {
    switch (patternType) {
        case PATH_PATTERN:
            for (byte sector = 0; sector < NUM_SECTORS; sector++) {
                strips[sector][FIRST_FIELD].setRGB(rPath, gPath, bPath);
            }
            break;
        // TODO better more random patterns, e.g. 1 gap, 2 gaps, 3 gaps, 4 gaps
        case WALL_PATTERN_1:
            for (byte sector = 0; sector < NUM_SECTORS; sector++) {
                if (sector % 2 == 0) {
                    strips[sector][FIRST_FIELD].setRGB(rPath, gPath, bPath);
                }
                else {
                    strips[sector][FIRST_FIELD].setRGB(rWall, gWall, bWall);
                }
            }
            break;
        case WALL_PATTERN_2:
            for (byte sector = 0; sector < NUM_SECTORS; sector++) {
                if (sector % 3 == 0) {
                    strips[sector][FIRST_FIELD].setRGB(rPath, gPath, bPath);
                }
                else {
                    strips[sector][FIRST_FIELD].setRGB(rWall, gWall, bWall);
                }
            }
            break;
        case WALL_PATTERN_3:
            for (byte sector = 0; sector < NUM_SECTORS; sector++) {
                if (sector % 4 == 0) {
                    strips[sector][FIRST_FIELD].setRGB(rPath, gPath, bPath);
                }
                else {
                    strips[sector][FIRST_FIELD].setRGB(rWall, gWall, bWall);
                }
            }
            break;
    }
}

void randomizeColors() {
    byte wallColor = (byte) random(0, 255);
    byte pathColor = (byte) random(0, 255);

    rWall = wallColor;
    gWall = wallColor;
    bWall = wallColor;

    rPath = pathColor;
    gPath = pathColor;
    bPath = pathColor;
}

//void ledColorize(int field, Adafruit_NeoPixel *strip, byte r, byte g, byte b){
//    strip->setPixelColor(field, strip->Color(r,g,b));
//}
//
//void ledOff(int field, Adafruit_NeoPixel *strip){
//    ledColorize(field, strip, 0, 0, 0);
//}
//
//void allLedOff(){
//    for (byte sector = 0; sector < STRIPS_LEN; sector++){
//        for (byte field = 0; field < NUM_LEDS_PER_STRIP; field++){
//            ledOff(field, &strips[sector]);
//        }
//        strips[sector].show();
//    }
//}
//
//void ledsShininginDifferentColors(int i, int chosenLedSector){
//    for (int y=0; y < STRIPS_LEN; y++){
//        if(y == chosenLedSector){
//            ledColorize( i, &strips[y], NOCOLORVALUE,GREENVALUE,NOCOLORVALUE);
//            continue;
//        }
//        ledColorize( i,  &strips[y], REDVALUE, NOCOLORVALUE,NOCOLORVALUE);
//
//    }
//}
