/*
 * other-functions.cpp
 * OTHER FUNCTIONS: Catch-all for functions that don't fit anywhere else.
 * */

#include "common.h"
#include "cds-cell-functions.h"

// Calibrates the servos
void calibrateServos() {
    // Calibrate lower Servo
    lower_servo.SetMin(LOWER_SERVO_MIN);
    lower_servo.SetMax(LOWER_SERVO_MAX);

    // Calibrate upper Servo
    upper_servo.SetMin(UPPER_SERVO_MIN);
    upper_servo.SetMax(UPPER_SERVO_MAX);
}

// Waits for the user to touch the screen
void waitForTouch() {
    float x, y;
    LCD.WriteLine("Waiting for touch: Press and release the screen to continue");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
}

// Sets the servos to the correct initial position
void setupServos() {
    // Set arm servos to initial position
    lower_servo.SetDegree(0);
    upper_servo.SetDegree(172);
}

// Waits for the bump switch to be pressed
void waitForSkidServoBumpSwitchPress() {
    while (skidServoCornerBump.Value() == 1) {
        // Burns time while the switches are not pressed
    }
    while (skidServoCornerBump.Value() == 0) {
        // Burns time while the switches ARE pressed
    }
}

// Prepares the robot to run on the course
void prepareToLaunch() {
    calibrateServos();
    setupServos();

    LCD.WriteLine("Set the arm to the correct position.");
    LCD.WriteLine("Then touch the screen to continue");

    waitForTouch();

    //Choose which course for RPS
    RPS.InitializeTouchMenu();

    LCD.WriteLine("Press and release the servo-side bump switch to start reading the light");

    // Wait for a bump switch to be pressed as the final action
    waitForSkidServoBumpSwitchPress();

    //Wait for light
    waitForStartLight();
}

// Moves the arm to drop the coin into the slot
void dropCoin() {
    const double SECONDS_TO_WAIT = 1.0;

    // Rotates lower servo to put the arm in position
    lower_servo.SetDegree(LOWER_DEGREE_STRAIGHT_OUT);

    Sleep(SECONDS_TO_WAIT);

    // Rotates upper servo to drop coin
    upper_servo.SetDegree(UPPER_DEGREE_VERTICAL_DOWN);

    Sleep(SECONDS_TO_WAIT);

    // Rotates lower servo to put the arm in position
    lower_servo.SetDegree(LOWER_DEGREE_STRAIGHT_OUT - 120);

    Sleep(SECONDS_TO_WAIT);
}

// Moves the arm back and forth to ensure it functions correctly
void testArm() {
    calibrateServos();

    setupServos();

    Sleep(1.5);

    waitForTouch();

    dropCoin();

    upper_servo.SetDegree(0);

    lower_servo.SetDegree(180);

    Sleep(1.5);

    lower_servo.SetDegree(0);

    Sleep(1.5);

    lower_servo.SetDegree(180);

    Sleep(1.5);

    lower_servo.SetDegree(0);
}
