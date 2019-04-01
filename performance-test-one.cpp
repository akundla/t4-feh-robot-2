/*
 * performance-test-one.cpp
 * PERFORMANCE TEST ONE: Navigates the robot up the steep ramp and flips the lever, relying entirely on timing.
 * */

#include "common.h"
#include "cds-cell-functions.h"
#include "time-drive-functions.h"

/**
 * @brief performanceTestOne: Navigates through the course to complete the first performance test.
 * This function first moves the servo motor into the 120 degree position,
 * at which point it prompts the user to rotate the arm to the correct
 * position so that when the servo is set to 0 degrees, the arm will hit
 * the lever. Once the user presses the screen, it calls a function to
 * wait for the red light to come on. When it detects the red light,
 * the function calls a turn function to rotate
 * the robot 45 degrees counerclockwise to align it with the ramp. It
 * then calls the driveForSeconds function to
 * drive the robot from the starting box up the ramp. At the top of the
 * ramp, it calls the turn function to turn the robot around nearly 180
 * degrees, then calls the drive function again to drive the robot forward
 * the last few inches. The function then sets the servo arm's position
 * back to 0 to flip the lever and calls the drive function to drive back
 * down the ramp to the starting box.
 */
void performanceTestOne () {

    // Calibration values:
    const double secondsToDriveUpRamp = 5.6;
    const double secondsToDriveToLever = 1.34;
    const double secondsToDriveBack = 6.0;

    // Set the servo arm in starting position
    lower_servo.SetDegree(120);

    float x, y;
    LCD.WriteLine("Performance test 1");
    LCD.WriteLine("Set the arm to the correct position you robot champion!");
    LCD.WriteLine("Then touch the screen to continue");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    // Waits for the light to turn red
    waitForStartLight();

    // Turns the robot roughly 45 degrees on start to align with the ramp
    turnOnStart();

    // Drives from starting box up the short ramp
    driveForSeconds(SKID_FIRST, secondsToDriveUpRamp, DRIVE_POWER);

    // Turns the robot roughly 180 degrees at the top of the ramp
    turnAround();

    // Drives the robot to the lever
    driveForSeconds(WHEELS_FIRST, secondsToDriveToLever, DRIVE_POWER);

    // Pauses to allow the robot to settle into place
    Sleep(0.5);
    // Turns the servo arm to hit the lever
    lower_servo.SetDegree(0);
    // Pauses to allow the arm to actually hit said lever
    Sleep(0.5);

    // Drives back down the ramp
    driveForSeconds(SKID_FIRST, secondsToDriveBack, DRIVE_POWER);

    rightMotor.Stop();
    leftMotor.Stop();
}
