// TODO Set accordingly
#define SENSOR_R 2 // with the sensors at 12 o'clock it is the RIGHT one
#define SENSOR_L 3 // with the sensors at 12 o'clock it is the LEFT one

#define FIRST_SECTOR 0
#define LAST_SECTOR 5

// values for moveDirection
#define MOVING_RIGHT 1
#define MOVING_LEFT -1
#define MOVING_IN_SECTOR 0

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
    else if (exitSensorR && !atSensorL) {
        isCalibrated = true; // starts game
        curSector = FIRST_SECTOR;
    }
    // initialize when exiting right
    else if (exitSensorL && !atSensorR) {
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
            case MOVING_IN_SENSOR:
                break;
        }
    }
}
