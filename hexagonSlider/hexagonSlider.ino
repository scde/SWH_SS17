#include "FastLED.h"
//LED Constants
//Set PINS for all LEDs
#define PIN_6    6
#define PIN_7    7
#define PIN_8    8
#define PIN_9    9
#define PIN_10  10
#define PIN_11  11 
#define PIN_0    0

//Set Constants for randomization
# define RANDOMLOWERLIMIT 0
# define RANDOMUPPERLIMIT 5
# define NUMBEROFSECTORS  6

// Set Constants for different Patterns
#define   NUM_LEDS_PER_STRIP   7
#define   NUM_SECTORS          6
#define   FIRST_FIELD          0
#define   PATH_PATTERN         0
#define   WALL_PATTERN_1       1
#define   WALL_PATTERN_2       2
#define   WALL_PATTERN_3       3

//Sector Constants
// TODO Set accordingly
#define SENSOR_R 5 // with the sensors at 12 o'clock it is the RIGHT one
#define SENSOR_L 9 // with the sensors at 12 o'clock it is the LEFT one

#define FIRST_SECTOR 0
#define LAST_SECTOR 5

// values for moveDirection
#define MOVING_RIGHT 1
#define MOVING_LEFT  2
#define MOVING_IN_SECTOR 0


// LED Variables
CRGB strips[NUM_SECTORS][NUM_LEDS_PER_STRIP];

bool wallSpawn;

// game update speed (wall spawn/move time)
int gameSpeed;
long iteration = 0;
unsigned long lastGameUpdate;

byte rWall;
byte gWall;
byte bWall;

byte rPath;
byte gPath;
byte bPath;


//Sector Variables
// 7 sector values (-1 [not calibrated], 0, 1, 2, 3, 4, 5)
byte curSector;

// for tracking move direction while in sensor range
byte moveDirection;

int calibrationPointer;
bool isCalibrated;

bool atSensorR;
bool enterSensorR;
bool wasAtSensorR;
bool exitSensorR;

bool atSensorL;
bool enterSensorL;
bool wasAtSensorL;
bool exitSensorL;

void setup() {
    Serial.begin(9600);

    ledSetup();
    sensorSetup();
}

void ledSetup(){
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

void sensorSetup(){
    pinMode(SENSOR_R, INPUT);
    pinMode(SENSOR_L, INPUT);

    isCalibrated = false;

    curSector = -1;
    
    enterSensorR = false;
    wasAtSensorR = false;
    exitSensorR = false;
    enterSensorL = false;
    wasAtSensorL = false;
    exitSensorL = false;
}
void loop() {
    ledLoop();
    sensorLoop();
}

void ledLoop(){
  if (millis() - lastGameUpdate >= gameSpeed) {
        lastGameUpdate = millis();

        // moves colors out one field and overwrites the last one
        for (byte sector = 0; sector < NUM_SECTORS; sector++) {
            memmove(&strips[sector][FIRST_FIELD + 1],
                    &strips[sector][FIRST_FIELD],
                    (NUM_LEDS_PER_STRIP - 1) * sizeof( CRGB));       
        }
             
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

void sensorLoop(){
  checkSensors();
    
    if (!isCalibrated) {
        calibrateSectors();
    }
    else {
        calculateSector();
        // TODO start game
    }
}

// check light sensor readings
void checkSensors() {
    // reset enter and exit values
    enterSensorR = false;
    exitSensorR = false;

    enterSensorL = false;
    exitSensorL = false;

    // read new sensor data
    atSensorR = digitalRead(SENSOR_R) == LOW;
    atSensorL = digitalRead(SENSOR_L) == LOW;

    // check if right sensor values changed
    if (wasAtSensorR != atSensorR) {
        // check if exiting right sensor
        if (wasAtSensorR) {
            exitSensorR = true;
        }
        // check if entering right sensor
        else {
            enterSensorR = true;
        }
    }

    // check if left sensor values changed
    if (wasAtSensorL != atSensorL) {
        // check if exiting left sensor
        if (wasAtSensorL) {
            exitSensorL = true;
        }
        // check if entering left sensor
        else {
            enterSensorL = true;
        }
    }

    // set values for next loop
    wasAtSensorR = atSensorR;
    wasAtSensorL = atSensorL;
}

void calibrateSectors() {
    // initialize when exiting left
    if (exitSensorR && !atSensorL) {
        isCalibrated = true; // starts game
        curSector = FIRST_SECTOR;
    }
    // initialize when exiting right
    if (exitSensorL && !atSensorR) {
        isCalibrated = true; // starts game
        curSector = LAST_SECTOR;
    }
}

void calculateSector() {
    // 4. set pointer position (all pointers are able to trigger both sensors)
    // trigger both sensors
    if (atSensorR && atSensorL) {
        // moving pointer to the right over sensors barrier
        if (enterSensorR) {
            // handle overflow
            if (curSector == LAST_SECTOR) {
                curSector = FIRST_SECTOR;
            }
            else {
                curSector++;
            }
        }
        // moving pointer to the left over sensors barrier
        else if (enterSensorL) {
            // handle underflow
            if (curSector == FIRST_SECTOR) {
                curSector = LAST_SECTOR;
            }
            else {
                curSector--;
            }
        }
    }
    // TODO FIXME if ist nicht ausreichend weil es sein kann das sie sich reinbewegen zurück aber nicht komplett raus und wieder rein
    else {
        switch (moveDirection) {
            case MOVING_RIGHT:
                break;
            case MOVING_LEFT:
                break;
            case MOVING_IN_SECTOR:
                break;
        }
    }
}
