#include "common.h"
#include "cds-cell-functions.h"
#include "shaft-encoders-drive-functions.h"
#include "rps-functions.h"

// Rotates the servo arm to drop the coin into the slot
void dropCoin() {
    const double SECONDS_TO_WAIT = 1.5;

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

void performanceTestThree() {

    //Number of counts for 45 degree turn at beginning
    const double InitialTurn = 11;
    const double InchesUpRamp = 31.5;
    const double InchesToCoin = 6.0;
    const double InchesToCoinSlot = 2;

    // Set arm servos to initial position
    lower_servo.SetDegree(15);
    upper_servo.SetDegree(140);

    float x, y;
    LCD.WriteLine("Performance Test 3");
    LCD.WriteLine("Set the arm to the correct position you robot champion!");
    LCD.WriteLine("Then touch the screen to continue");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    //Choose which course for RPS
    RPS.InitializeTouchMenu();

    //Wait for light
    waitForStartLight();

    //Turns robot counterclockwise 45 degrees to alight with steep ramp
    turnCountsInPlace(COUNTER_CLOCKWISE, InitialTurn, TURN_POWER);

    //verify robot is facing in the positive y direction
    check_heading(SKIDS_COURSE_TOP);

    //Drive robot to top of ramp
    driveForInches(SKID_FIRST, InchesUpRamp, DRIVE_POWER * 1.30);

    // Align 90 degrees
    check_heading(SKIDS_COURSE_TOP);

    //Verify Position with RPS
    check_y_plus(46.0);

    //Turn 90 degrees clockwise
    turnCountsInPlace(CLOCKWISE, InitialTurn*2.0, TURN_POWER);

    //Check turn with RPS
    check_heading(SKIDS_COURSE_RIGHT);

    //Drive to be be aligned with coin slot on the side
    driveForInches(SKID_FIRST, InchesToCoin, DRIVE_POWER);

    // Check that the robot made it to the coin slot
    check_x_plus(19.0);

    //Turn to align arm
    turnCountsInPlace(COUNTER_CLOCKWISE, InitialTurn*1.7, TURN_POWER);

    //Check turn with RPS
    check_heading(SKIDS_COURSE_TOP);

    //Drive to be be aligned with coin slot on the side
    driveForInches(WHEELS_FIRST, InchesToCoinSlot, DRIVE_POWER);

    // Turns the servos to drop the coin
    dropCoin();

    // Back away from the slot
    driveForInches(SKID_FIRST, 2.0 * InchesToCoinSlot, DRIVE_POWER);

    // Turn clockwise to drive wheels-first back to the top of the ramp
    turnCountsInPlace(CLOCKWISE, InitialTurn*2.0, TURN_POWER);

    // Check that the wheels face the left wall
    check_heading(WHEELS_COURSE_LEFT);

    //Drive to be be aligned with the lever horizontally
    driveForInches(WHEELS_FIRST, InchesToCoin, DRIVE_POWER);

    // Turn to face the lever
    turnCountsInPlace(CLOCKWISE, InitialTurn*2.0, TURN_POWER);

    // Check that the wheels face the lever
    check_heading(WHEELS_COURSE_TOP);

    // Prepare arm to hit lever
    lower_servo.SetDegree(175);
    upper_servo.SetDegree(60);

    Sleep(0.4);

    //Drive to be be aligned with the lever
    driveForInches(WHEELS_FIRST, 9.5, DRIVE_POWER);

    //Turn counter-clockwise to angle arm better relative to lever
    turnCountsInPlace(COUNTER_CLOCKWISE, 4, TURN_POWER);

    //Drive toward lever
    driveForInches(WHEELS_FIRST, 3.35, DRIVE_POWER);

    // Whack that lever
    lower_servo.SetDegree(20);
}
