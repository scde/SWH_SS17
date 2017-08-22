// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include "FastLED.h"
 
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
#define   PIN_6                6
#define   PIN_7                7
#define   PIN_8                8
#define   PIN_9                9
#define   PIN_10               10
#define   PIN_11               11
#define   PIN_0                0 


// How many NeoPixels are attached to the Arduino?
#define   NUM_LEDS_PER_STRIP   7
#define   NUM_SECTORS          6

#define   FIRST_FIELD          0

#define   PATH_PATTERN         0
#define   WALL_PATTERN_1       1
#define   WALL_PATTERN_2       2
#define   WALL_PATTERN_3       3
#define   RANDOM_LOWER_LIMIT   0
#define   RANDOM_UPPER_LIMIT   6

CRGB strips[NUM_SECTORS][NUM_LEDS_PER_STRIP];

bool wallSpawn;

// game update speed (wall spawn/move time)
int gameSpeed;
long iteration = 0;
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
    randomSeed(analogRead(0));
    
    wallSpawn = false;

    gameSpeed = 800;
    lastGameUpdate = 0;


    rWall = 150;
    gWall = 0;
    bWall = 0;

    rPath = 0;
    gPath = 150;
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
   
    if (millis() - lastGameUpdate >= gameSpeed) {
        lastGameUpdate = millis();

        // moves colors out one field and overwrites the last one
        for (byte sector = 0; sector < NUM_SECTORS; sector++) {
            memmove(&strips[sector][FIRST_FIELD + 1],
                    &strips[sector][FIRST_FIELD],
                    (NUM_LEDS_PER_STRIP - 1) * sizeof( CRGB));       
        }

        /*for(byte sectors =0; sectors <NUM_SECTORS;sectors++){
          for (byte fields = 0; fields<7; fields++){
            strips[sectors][fields].red += fields *5;
          }
        }*/


        if (wallSpawn) {
          if(millis() <= 10000){
            spawnPattern(WALL_PATTERN_1);
        }else if(millis() <= 20000){
            gameSpeed = 800;
            spawnPattern(WALL_PATTERN_2);
          }else if(millis() > 20000){
            spawnPattern(WALL_PATTERN_3);
            gameSpeed = 750;
          }
        }
        else {
            spawnPattern(PATH_PATTERN);
        }
        wallSpawn = !wallSpawn;

        FastLED.show();

    }

    delay(10);
}

void getRandomArray(byte *randomValues, int length ){

  int openFields=3;
  long lastNumber = 0;
  for (byte i =0;i<openFields;i++){
    long minNumber = lastNumber + iteration; 
    long maxNumber = 5 - (openFields-i-1) + iteration;
    long randomNumber = random( minNumber,  maxNumber + 1);
    randomNumber = (randomNumber + iteration) % 6;
    randomValues[i] = randomNumber;
    lastNumber = randomNumber+1;
  }
  iteration++;
}



void spawnPattern(byte patternType) {
  int randomDoor = -1;
  byte randomValues[3];
  getRandomArray(randomValues, (sizeof(randomValues)/sizeof(byte)));
  
    switch (patternType) {
        case PATH_PATTERN:
            for (byte sector = 0; sector < NUM_SECTORS; sector++) {
                if(sector == randomValues[0]){
                  strips[sector][FIRST_FIELD].setRGB(rWall, gWall, bWall);
                }else{
                strips[sector][FIRST_FIELD].setRGB(rPath, gPath, bPath);
                }
            }
           
            break;
        // TODO better more random patterns, e.g. 1 gap, 2 gaps, 3 gaps, 4 gaps
        case WALL_PATTERN_1: 
       
            for (byte sector = 0; sector < NUM_SECTORS; sector++) 
            {
                for (byte i = 0; i < sizeof(randomValues)/sizeof(byte); i++)
                {
                    if(sector == randomValues[i])
                    {
                        strips[sector][FIRST_FIELD].setRGB(rPath, gPath, bPath);
                        break;
                    }
                    else
                    {
                        strips[sector][FIRST_FIELD].setRGB(rWall, gWall, bWall);
                    }
                }
            }
            break;
        case WALL_PATTERN_2:
             for (byte sector = 0; sector < NUM_SECTORS; sector++) 
            {
                for (byte i = 0; i < sizeof(randomValues)/sizeof(byte)-1; i++)
                {
                    if(sector == randomValues[i])
                    {
                      strips[sector][FIRST_FIELD+1].red == rWall;
                       strips[sector][FIRST_FIELD].setRGB(rPath, gPath, bPath);
                       break;
                    }
                    else
                    {
                        strips[sector][FIRST_FIELD].setRGB(rWall, gWall, bWall);
                    }
                }
            }
            break;
        case WALL_PATTERN_3:
            for (byte sector = 0; sector < NUM_SECTORS; sector++) 
            {
                for (byte i = 0; i < sizeof(randomValues)/sizeof(byte)-2; i++)
                {
                    if(sector == randomValues[i])
                    {
                      if(strips[sector][FIRST_FIELD+1].red == rWall){
                          strips[sector+1][FIRST_FIELD].setRGB(rPath, gPath, bPath);
                          break;
                      }else{
                        strips[sector][FIRST_FIELD].setRGB(rPath, gPath, bPath);
                      }
                    }
                    else
                    {
                        strips[sector][FIRST_FIELD].setRGB(rWall, gWall, bWall);
                    }
                }
            }
            break;
    }
    
   byte counter = 0;
   for (byte sector = 0; sector < NUM_SECTORS; sector++) {
     if(strips[sector][FIRST_FIELD].red ==rWall && strips[sector][FIRST_FIELD].green == gWall && strips[sector][FIRST_FIELD].blue == bWall){
       counter ++;
     }
     if(counter == 6){
      spawnPattern(patternType);
     }
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


