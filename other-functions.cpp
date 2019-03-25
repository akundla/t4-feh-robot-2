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
    lower_servo.SetDegree(15);
    upper_servo.SetDegree(140);
}

void prepareToLaunch() {
    calibrateServos();
    setupServos();

    LCD.WriteLine("Set the arm to the correct position.");
    LCD.WriteLine("Then touch the screen to continue");

    waitForTouch();

    //Choose which course for RPS
    RPS.InitializeTouchMenu();

    //Wait for light
    waitForStartLight();
}
