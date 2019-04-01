/*
 * performance-test-two.cpp
 * PERFORMANCE TEST TWO NAVIGATOR: See @brief for performanceTestTwoCounts
 * */

#include "common.h"
#include "cds-cell-functions.h"
#include "time-drive-functions.h"
#include "shaft-encoders-drive-functions.h"

// NOTE: Incomplete drive function. Abandoned upon making the shaft encoders function.
void performanceTestTwo () {
    float x, y;
    //Calibration Values
    const double secondsToFirstDDRButton = 5.0;
    const double secondsAfterStart = 0.5;

    //Set initial servo degree
    lower_servo.SetDegree(180);

    //Print test information to screen
    LCD.WriteLine("Performance test 2");
    LCD.WriteLine("Set the arm to the correct position you robot champion!");
    LCD.WriteLine("Then touch the screen to continue");

    //Wait for final action
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    //Robot will begin on start light
    waitForStartLight();

    driveForSeconds(SKID_FIRST,secondsAfterStart, DRIVE_POWER);

    //Turn Clockwise about 45 degrees to face DDR buttons
    turnOnStart();

    //Drive to the first DDR button
    driveForSeconds(SKID_FIRST, secondsToFirstDDRButton, DRIVE_POWER);
}

/**
 * @brief performanceTestTwoCounts: Navigates the robot to the left light,
 * reads its color, drives into the button of the correct color, holds the button,
 * releases the button, and drives to touch the foosball counters.
 */
void performanceTestTwoCounts () {

    //Calibration Values
    const double inchesToDriveOnStart = 3.40;
    // 11 counts makes a 45 degree turn. Don't mess with it.
    const int countsToTurn = 11;
    // 8 Inches is virtually perfect
    const double inchesToDriveToButton = 7.5;

    //Print test information to screen
    LCD.WriteLine("Performance test 2");
    LCD.WriteLine("Waiting for start light...");

    //Robot will begin on start light
    waitForStartLight();

    LCD.WriteLine("Driving...");

    // Drives out from starting box
    driveForInches(SKID_FIRST, inchesToDriveOnStart, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    //Turn Clockwise about 45 degrees to face DDR buttons
    turnCountsInPlace(CLOCKWISE, countsToTurn, TURN_POWER);

    //Drive to the first DDR button
    driveForInches(SKID_FIRST, inchesToDriveToButton, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Pauses to allow the robot to settle
    Sleep(1.0);

    // Detects chosen light color
    bool IS_RED = false;
    if (detectLight(RED_LIGHT_RED_F_V_AVG)) {
        LCD.WriteLine("Detected Red Light");
        Sleep(5.0);
    } else {
        LCD.WriteLine("Detected Blue Light");
        Sleep(5.0);
    }

        IS_RED = true;

    // Hits the red button. This is DIALED IN, don't recalibrate unless it stops working - AK 8:55PM Wednesday 27th February 2019
    if (IS_RED) {
        // Drives the robot up to be level with the red button
        driveForInches(SKID_FIRST, 4.0, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

        // Turn 90 degrees so the wheel-side of the robot faces the buttons
        turnCountsInPlace(COUNTER_CLOCKWISE, 22, TURN_POWER);

        // Drives the robot into the button
        driveForSeconds(WHEELS_FIRST, 3.0, (DRIVE_POWER / 3) + 10);

        // Holds the button down
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);

        //Turn 90 Degrees to face the second light
        turnCountsInPlace(CLOCKWISE, 22, TURN_POWER);

        //Drive to second light
        driveForInches(SKID_FIRST, 3.75, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

        //Turn to face acrylic ramp
        turnCountsInPlace(COUNTER_CLOCKWISE, 22, TURN_POWER);
    }
    // Otherwise the light is blue
    else {
        const double inchesToBlueButton = 9.0;

        // Drives the robot up to be level with the second button
        driveForInches(SKID_FIRST, inchesToBlueButton, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

        // Turn 90 degrees so the wheel-side of the robot faces the buttons
        turnCountsInPlace(COUNTER_CLOCKWISE, 24, TURN_POWER);

        // Drives the robot into the button
        driveForSeconds(WHEELS_FIRST, 3.0, (DRIVE_POWER / 3) + 10);

        // Hold down the button
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);
    }

    //Drive to the foosball counter
    driveForInches(SKID_FIRST, 45, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    //Move robot left by turning and driving straight
    turnCountsInPlace(COUNTER_CLOCKWISE, 11, TURN_POWER);
    driveForInches(SKID_FIRST, 2, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

    //turn toward foosball and drive to hit it
    turnCountsInPlace(CLOCKWISE, 11, TURN_POWER);
    driveForInches(SKID_FIRST, 15, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);
}
