#include "common.h"
#include "cds-cell-functions.h"
#include "time-drive-functions.h"
#include "shaft-encoders-drive-functions.h"

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
    driveForInches(SKID_FIRST, inchesToDriveOnStart, DRIVE_POWER);

    //Turn Clockwise about 45 degrees to face DDR buttons
    turnCountsInPlace(CLOCKWISE, countsToTurn, TURN_POWER);

    //Drive to the first DDR button
    driveForInches(SKID_FIRST, inchesToDriveToButton, DRIVE_POWER);

    // Pauses to allow the robot to settle
    Sleep(1.0);

    // TODO: Get values from the sensor for blue and red lights through the filter.
    bool IS_RED = false;
    if (cdsCell.Value() < RED_LIGHT_RED_F_V_AVG + MoE && cdsCell.Value() > RED_LIGHT_RED_F_V_AVG - MoE)
    {
        IS_RED = true;
        LCD.WriteLine("Detected Red Light");
        Sleep(5.0);
    }
    else
    {
        LCD.WriteLine("Detected Blue Light");
        Sleep(5.0);
    }

    // Hits the red button. This is DIALED IN, don't recalibrate unless it stops working - AK 8:55PM Wednesday 27th February 2019
    if (IS_RED) {
        // Drives the robot up to be level with the red button
        driveForInches(SKID_FIRST, 4.0, DRIVE_POWER / 2.0);

        // Turn 90 degrees so the wheel-side of the robot faces the buttons
        turnCountsInPlace(COUNTER_CLOCKWISE, 22, TURN_POWER);

        // Drives the robot into the button
        driveForSeconds(WHEELS_FIRST, 3.0, (DRIVE_POWER / 3) + 10);

        // Holds the button down
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER);

        //Turn 90 Degrees to face the second light
        turnCountsInPlace(CLOCKWISE, 22, TURN_POWER);

        //Drive to second light
        driveForInches(SKID_FIRST, 3.75, DRIVE_POWER / 2.0);

        //Turn to face acrylic ramp
        turnCountsInPlace(COUNTER_CLOCKWISE, 22, TURN_POWER);
    }
    // Otherwise the light is blue
    else {
        // TODO: Calibrate, it's definitely more than 5
        const double inchesToBlueButton = 9.0;

        // Drives the robot up to be level with the second button
        driveForInches(SKID_FIRST, 9.0, DRIVE_POWER / 2.0);

        // Turn 90 degrees so the wheel-side of the robot faces the buttons
        turnCountsInPlace(COUNTER_CLOCKWISE, 24, TURN_POWER);

        // Drives the robot into the button
        driveForSeconds(WHEELS_FIRST, 3.0, (DRIVE_POWER / 3) + 10);

        // Hold down the button
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER);
    }

    //Drive to the foosball counter
    driveForInches(SKID_FIRST, 45, DRIVE_POWER);

    //Move robot left by turning and driving straight
    turnCountsInPlace(COUNTER_CLOCKWISE, 11, TURN_POWER);
    driveForInches(SKID_FIRST, 2, DRIVE_POWER / 2.0);

    //turn toward foosball and drive to hit it
    turnCountsInPlace(CLOCKWISE, 11, TURN_POWER);
    driveForInches(SKID_FIRST, 15, DRIVE_POWER / 2.0);
}
