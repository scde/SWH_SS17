#include "FastLED.h"
//LED Constants
//Set PINS for all LEDs
#define SECTOR_5 5
#define SECTOR_4 6
#define SECTOR_3 7
#define SECTOR_2 8
#define SECTOR_1 9
#define SECTOR_0 10

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
#define SENSOR_R 2 // with the sensors at 12 o'clock it is the RIGHT one
#define SENSOR_L 3 // with the sensors at 12 o'clock it is the LEFT one

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

bool isMovingBetweenSensors;
bool initialEnterRight;


byte randomValues[3];

void setup() {
  Serial.begin(9600);
  sensorSetup();
  ledSetup();
}

void ledSetup() {
  randomSeed(analogRead(0));

  wallSpawn = false;

  gameSpeed = 1300;
  lastGameUpdate = 0;


  rWall = 150;
  gWall = 0;
  bWall = 0;

  rPath = 0;
  gPath = 150;
  bPath = 150;

  // initialize strips to off
  FastLED.addLeds<NEOPIXEL, SECTOR_0>(strips[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, SECTOR_1>(strips[1], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, SECTOR_2>(strips[2], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, SECTOR_3>(strips[3], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, SECTOR_4>(strips[4], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, SECTOR_5>(strips[5], NUM_LEDS_PER_STRIP);
}

void sensorSetup() {
  pinMode(SENSOR_R, INPUT);
  pinMode(SENSOR_L, INPUT);

  /*isCalibrated = false;*/

  isMovingBetweenSensors = false;
  initialEnterRight = false;
  curSector = 0;

  enterSensorR = false;
  wasAtSensorR = false;
  exitSensorR = false;
  enterSensorL = false;
  wasAtSensorL = false;
  exitSensorL = false;
}
void loop() {
  sensorLoop();
  ledLoop();
  checkCollision();
}

void checkCollision() {
  if (strips[curSector][NUM_LEDS_PER_STRIP - 1].r == rWall) {
    // TODO game over animation and restart
    gameOver();
  }
}

void gameOver() {
  /*for (byte sector = 0; sector < NUM_SECTORS; sector++) {
      for (byte field = 0; field < NUM_LEDS_PER_STRIP; field++) {
        strips[sector][field] = CRGB::Black;
      }
    }
    FastLED.show();*/

  for (int i = 0; i < 3; i++) {
    allLedsOff();
    allLedsRed();
    allLedsOff();
  }
}

void allLedsOff() {
  // turn all fields off
    for (byte sector = 0; sector < NUM_SECTORS; sector++) {
      for (byte field = 0; field < NUM_LEDS_PER_STRIP; field++) {
        strips[sector][field] = CRGB::Black;
      }
    }
    FastLED.show();
    delay(300);
}

void allLedsRed() {
    // turn all fields off
    for (byte sector = 0; sector < NUM_SECTORS; sector++) {
      for (byte field = 0; field < NUM_LEDS_PER_STRIP; field++) {
        strips[sector][field] = CRGB::Red;
      }
    }
    FastLED.show();
    delay(500);
}


void ledLoop() {
  // Sends several signals in a certain time interval
  if (millis() - lastGameUpdate >= gameSpeed) {
    lastGameUpdate = millis();

    /*
      // moves colors out one field and overwrites the last one
      for (byte sector = 0; sector < NUM_SECTORS; sector++) {
      memmove(&strips[sector][FIRST_FIELD + 1],
              &strips[sector][FIRST_FIELD],
              (NUM_LEDS_PER_STRIP - 1) * sizeof( CRGB));
      }
    */
    for (byte sector = 0; sector < NUM_SECTORS; sector++) {
      for (byte segment = NUM_LEDS_PER_STRIP - 1; segment > 0; segment--) {
        strips[sector][segment] = strips[sector][segment - 1];
      }
    }
    // checks what pattern is the actual pattern in playtime
    if (wallSpawn) {
      if (millis() <= 10000) {
        spawnPattern(WALL_PATTERN_1);
      } else if (millis() <= 30000) {
        gameSpeed = 1200;
        spawnPattern(WALL_PATTERN_2);
      } else if (millis() > 30000) {
        spawnPattern(WALL_PATTERN_3);
        gameSpeed = 1100;
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
  boolean recall = true;
  while (recall == true) {
    recall = false;
    // fills a byte array with randomvalues
    //These values ​​symbolize the door sectors
    randomValues[3];
    getRandomArray(randomValues);

    // switch case in which the different cases are processed
    switch (patternType) {

      // Puts a hiking trail for the player
      // trail includes on wall
      case PATH_PATTERN:
        for (byte sector = 0; sector < NUM_SECTORS; sector++) {
          if (sector == randomValues[0]) {
            if (strips[sector][FIRST_FIELD + 1].green == gPath ) {

              while ( sector == randomValues[0]) {
                randomValues[0] = random(0, 5);
              }
              strips[randomValues[0]][FIRST_FIELD].setRGB(rWall, gWall, bWall);
            } else {
              strips[sector][FIRST_FIELD].setRGB(rWall, gWall, bWall);
            }
          }
          else {
            strips[sector][FIRST_FIELD].setRGB(rPath, gPath, bPath);
          }
        }
        break;

      //  Generates walls with 3 doors
      case WALL_PATTERN_1:

        for (byte sector = 0; sector < NUM_SECTORS; sector++)
        {
          for (byte i = 0; i < sizeof(randomValues) / sizeof(byte); i++)
          {
            if (sector == randomValues[i])
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

      //  Generates walls with 2 doors
      case WALL_PATTERN_2:
        for (byte sector = 0; sector < NUM_SECTORS; sector++)
        {
          for (byte i = 0; i < sizeof(randomValues) / sizeof(byte) - 1; i++)
          {
            if (sector == randomValues[i])
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

      //  Generates walls with 1 door
      case WALL_PATTERN_3:
        for (byte sector = 0; sector < NUM_SECTORS; sector++)
        {
          for (byte i = 0; i < sizeof(randomValues) / sizeof(byte) - 2; i++)
          {
            if (sector == randomValues[i])
            {
              if (strips[sector][FIRST_FIELD + 1].red == rWall) {
                strips[sector + 1][FIRST_FIELD].setRGB(rPath, gPath, bPath);
                break;
              } else {
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

    // checks if all sectors are walls
    //then it calls spawnPattern again
    byte counter = 0;
    for (byte sector = 0; sector < NUM_SECTORS; sector++) {
      if (strips[sector][FIRST_FIELD].red == rWall && strips[sector][FIRST_FIELD].green == gWall && strips[sector][FIRST_FIELD].blue == bWall) {
        counter ++;
      }
      if (counter == 6) {
        recall = true;
      }
    }
  }
}

// fills a byte array with randomValues
// All values in the array are different

void getRandomArray(byte *randomValues) {

  int openFields = 3;
  long lastNumber = 0;
  for (byte i = 0; i < openFields; i++) {
    long minNumber = lastNumber + iteration;
    long maxNumber = 5 - (openFields - i - 1) + iteration;
    long randomNumber = random( minNumber,  maxNumber + 1);
    randomNumber = (randomNumber + iteration) % 6;
    randomValues[i] = randomNumber;
    lastNumber = randomNumber + 1;
  }
  iteration++;
}

void sensorLoop() {
  checkSensors();
  calculateSector();

  /*if (!isCalibrated) {*/
  /*calibrateSectors();*/
  /*}*/
  /*else {*/
  /*calculateSector();*/
  /*// TODO start game*/
  /*}*/
}

// check light sensor readings
void checkSensors() {
  // reset enter and exit values
  enterSensorR = false;
  exitSensorR = false;

  enterSensorL = false;
  exitSensorL = false;

  // read new sensor data
  if(digitalRead(SENSOR_R) == HIGH){
    gameOver();
  }
  atSensorR = digitalRead(SENSOR_R) == LOW;
  atSensorL = digitalRead(SENSOR_L) == LOW;
  /*Serial.print(atSensorR);
  Serial.println("SensorR");
  Serial.print(atSensorL);
  Serial.println("atSensorL");*/
  

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

/*void calibrateSectors() {*/
/*// initialize when exiting left*/
/*if (exitSensorR && !atSensorL) {*/
/*isCalibrated = true; // starts game*/
/*curSector = FIRST_SECTOR;*/
/*}*/
/*// initialize when exiting right*/
/*if (exitSensorL && !atSensorR) {*/
/*isCalibrated = true; // starts game*/
/*curSector = LAST_SECTOR;*/
/*}*/
/*}*/

void calculateSector() {
  // initialize moving between sectors if it is not already set

/*Serial.println(atSensorR);
Serial.println("---R---");
Serial.println(atSensorL);
Serial.println("---L---");
Serial.println(isMovingBetweenSensors);*/
  
  if ((atSensorR || atSensorL) && !isMovingBetweenSensors) {
    // check if pointer is moving between sensors
    isMovingBetweenSensors = true;
    // save enter direction for exit evaluation
    if (enterSensorR) {
      initialEnterRight = true;
    }
    else {
      initialEnterRight = false;
    }
  }

  // check while moving between sectors
  if (isMovingBetweenSensors) {
    // evaluate exit condition
    if (!atSensorR && !atSensorL) {
      // exiting to the right while coming from left
      if (exitSensorR && !initialEnterRight) {
        // handle overflow
        if (curSector == LAST_SECTOR) {
          curSector = FIRST_SECTOR;
        }
        else {
          curSector++;
        }
      }
      // exiting to the left while coming from right
      else if (exitSensorL && initialEnterRight) {
        // handle underflow
        if (curSector == FIRST_SECTOR) {
          curSector = LAST_SECTOR;
        }
        else {
          curSector--;
        }
      }

      // reset sensor evaluation
      isMovingBetweenSensors = false;
    }
  }
  Serial.println(curSector);
}

