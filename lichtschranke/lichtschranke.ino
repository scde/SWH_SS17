// TODO Set accordingly
#define SENSOR_R 5 // with the sensors at 12 o'clock it is the RIGHT one
#define SENSOR_L 9 // with the sensors at 12 o'clock it is the LEFT one

#define FIRST_SECTOR 0
#define LAST_SECTOR 5

/*enum Sector {TOP_RIGHT, RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT, LEFT, TOP_LEFT};*/
// TODO use enmus (NOT_CALIBRATED [maybe do not do this to enable cycling thru values],
// TOP_RIGHT, RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT, LEFT, TOP_LEFT)
// 6 pointer values (-1 [not calibrated], 0, 60, 120, 180, 240, 300) [corresponds to sectors; left of sectors]
/*int curPointerPos;*/
// 6 sector values (-1 [not calibrated], 0, 1, 2, 3, 4, 5) [corresponds to pointer position; right of pointer position]
int curSector;
/*// 2 sensor values (0 [sensor1], 1 [sensor2])*/
/*int lastSensor;*/

int calibrationPointer;
bool isCalibrated;

bool enterSensorR;
bool wasAtSensorR;
bool exitSensorR;
bool enterSensorL;
bool wasAtSensorL;
bool exitSensorL;

/*bool trigger = false;*/
/*bool triggered = false;*/

void setup() {
    Serial.begin(9600);
    pinMode(SENSOR_R, INPUT);
    pinMode(SENSOR_L, INPUT);

    isCalibrated = false;
    calibrationPointer = null;

    curSector = -1;
    enterSensorR = false;
    wasAtSensorR = false;
    exitSensorR = false;
    enterSensorL = false;
    wasAtSensorL = false;
    exitSensorL = false;
}

void loop() {
    checkCollisions();
    calculateSector();
    if (!isCalibrated) {
        if (atZeroPointer) {
            // color only sector 0 and 5
        }
    }
    else {
        startGame();
    }
}

void checkCollisions() {
    // read new sensor data
    bool atSensorR = digitalRead(SENSOR_R) == LOW;
    bool atSensorL = digitalRead(SENSOR_L) == LOW;

    // check for new/different collision reading at right sensor
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
    // reset enter and exit values
    else {
        enterSensorR = false;
        exitSensorR = false;
    }

    // check for new/different collision reading at left sensor
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
    // reset enter and exit values
    else {
        enterSensorL = false;
        exitSensorL = false;
    }

    // set values for next loop
    wasAtSensorR = atSensorR;
    wasAtSensorL = atSensorL;
}

void calibrateSectors() {
    // initial calibration
    if (atSensorR && atSensorL) {
        atZeroPointer = true;
        curSector = 0;
    }
    // exit left
    else if (exitSensorR && !atSensorR && !atSensorL) {
        isCalibrated = true; // starts game
        curSector = 0;
    }
    // exit right
    else if (exitSensorL && !atSensorR && !atSensorL) {
        isCalibrated = true; // starts game
        curSector = 5;
    }
}

void calculateSector() {
    // 4. set pointer position (all pointers are able to trigger both sensors)
    else {
        // trigger both sensors
        if (atSensorR && atSensorL) {
            // moving pointer to the right over sensors barrier
            if (enterSensorR) {
                if (curSector == LAST_SECTOR) {
                    curSector = FIRST_SECTOR;
                }
                else {
                    curSector++;
                }
            }
            // moving pointer to the left over sensors barrier
            else if (enterSensorL) {
                if (curSector == FIRST_SECTOR) {
                    curSector = LAST_SECTOR;
                }
                else {
                    curSector--;
                }
            }
        }
        /*// trigger right sensor but not the left one*/
        /*else if (atSensorR) {*/
            /*// moving pointer to the left into the right sensor*/
            /*if (enterSensorR) {*/
            /*}*/
            /*// moving pointer to the right while triggering the right sensor*/
            /*else if (exitSensorL) {*/
            /*}*/
        /*}*/
        /*// trigger left sensor but not the right one*/
        /*else if (atSensorL) {*/
            /*// moving pointer to the right into the left sensor*/
            /*if (enterSensorL) {*/
            /*}*/
            /*// moving pointer to the left while triggering the left sensor*/
            /*else if (exitSensorR) {*/
            /*}*/
        /*}*/
        /*// trigger no sensor*/
        /*else {*/
        /*}*/
    }

    Serial.println(curSector);
}
