#include "common.h"
#include "other-functions.h"
#include "shaft-encoders-drive-functions.h"
#include "bump-switch-functions.h"
#include "rps-functions.h"
#include "time-drive-functions.h"
#include "performance-test-three.h"
#include "cds-cell-functions.h"

// Drives the robot throughout the entire course, completing all tasks.
void navigateFullCourse() {
    //Number of counts for 45 degree turn at beginning
    const double InitialTurn = 11;
    const double InchesUpRamp = 31.5;
    const double InchesToCoin = 5.5;
    const double InchesToCoinSlot = 2;
    const float rampPower = DRIVE_POWER * 1.45;
    const float yTopOfShortRamp = 46.0;
    const int ticksIn90DegreeTurn = 22;
    const float xCoinSlot = 19.0;
    const int ticksToLineUpWithCoinSlot = 18;
    const double inchesToLever = 9.5;
    const int ticksFinalTurnForLever = 4;
    const double finalInchesToLever = 3.35;
    // TODO: Calibrate to the arm skips as few teeth as possible
    const int lowerDegreeToHitLever = 80;
     const float degreeToHitCountersWall = 20;
     const double countersDistance = 9.5;

    LCD.WriteLine("Full Course Navigator");

    prepareToLaunch();

    //Turns robot counterclockwise 45 degrees to alight with steep ramp
    turnCountsInPlace(COUNTER_CLOCKWISE, InitialTurn, TURN_POWER);

    //verify robot is facing in the positive y direction
    check_heading(SKIDS_COURSE_TOP);

    //Drive robot to top of ramp
    driveForInches(SKID_FIRST, InchesUpRamp, rampPower, LEFT_MOTOR_OFFSET);

    // Align 90 degrees
    check_heading(SKIDS_COURSE_TOP);

    //Verify Position with RPS
    check_y_plus(yTopOfShortRamp);

    //Turn 90 degrees clockwise
    turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    //Check turn with RPS
    check_heading(SKIDS_COURSE_RIGHT);

    //Drive to be be aligned with coin slot on the side
    driveForInches(SKID_FIRST, InchesToCoin, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Check that the robot made it to the coin slot
    check_x_plus(xCoinSlot);

    //Turn to align arm
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksToLineUpWithCoinSlot, TURN_POWER);

    //Check turn with RPS
    check_heading(SKIDS_COURSE_TOP);

    //Drive to be be aligned with coin slot on the side
    driveForInches(WHEELS_FIRST, InchesToCoinSlot, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Turns the servos to drop the coin
    dropCoin();

    // Back away from the slot
    driveForInches(SKID_FIRST, 2.0 * InchesToCoinSlot, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Turn clockwise to drive wheels-first back to the top of the ramp
    turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    // Check that the wheels face the left wall
    check_heading(WHEELS_COURSE_LEFT);

    //Drive to be be aligned with the lever horizontally
    driveForInches(WHEELS_FIRST, InchesToCoin, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Turn to face the lever
    turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    // Check that the wheels face the lever
    check_heading(WHEELS_COURSE_TOP);

    // Prepare arm to hit lever
    lower_servo.SetDegree(LOWER_DEGREE_STRAIGHT_OUT + 10);
    upper_servo.SetDegree(UPPER_DEGREE_VERTICAL_DOWN + 10);
    // Wait for servos to finish moving
    Sleep(0.4);

    //Drive to be be aligned with the lever
    driveForInches(WHEELS_FIRST, inchesToLever, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    //Turn counter-clockwise to angle arm better relative to lever
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksFinalTurnForLever, TURN_POWER);

    //Drive toward lever
    driveForInches(WHEELS_FIRST, finalInchesToLever, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Whack that lever
    lower_servo.SetDegree(lowerDegreeToHitLever);
    // Sleep so servo can finish moving
    Sleep(0.6);

    // UNTESTED CODE BEGINS HERE

    //Drive back from lever
    driveForInches(SKID_FIRST, finalInchesToLever, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Turn so skids face right wall
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn - ticksFinalTurnForLever, TURN_POWER);

    // Drive until you hit the wall
    DriveSkidFirstUntilHitWall(DRIVE_POWER);

    // TESTED (and maybe slightly inconsistent) code begins again here

    // Wait after driving
    Sleep(1.0);

    // Rotate arm to hit counters back wall
    lower_servo.SetDegree(degreeToHitCountersWall);

    // Wait after turning
    Sleep(1.0);

    // Rotate arm on its axis to hit counters
    upper_servo.SetDegree(UPPER_DEGREE_VERTICAL_DOWN);

    // Wait after turning
    Sleep(1.0);

    bool shouldYEET = true;

    if (!shouldYEET) {
        // Drive forward dragging the counters along
        driveForInches(WHEELS_FIRST, countersDistance - 1.0, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

        // Wait after turning
        Sleep(1.0);

        // TODO: Make constant
        lower_servo.SetDegree(60);

        // Wait after turning
        Sleep(1.0);

        // Rotate arm on its axis to shove counters into wall
        upper_servo.SetDegree(170);

        // Wait after turning
        Sleep(1.0);
    } else {
        // Drive forward dragging the counters along
        driveForInches(WHEELS_FIRST, (countersDistance * 2.0/3.0) + 2.0, DRIVE_POWER / 2.0, 0.04);

        // Wait after turning
        Sleep(1.0);

        upper_servo.SetDegree(170);

        Sleep(1.0);

        // Drive forward dragging the counters along
        driveForInches(SKID_FIRST, 3.0, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

        Sleep(1.0);

        upper_servo.SetDegree(UPPER_DEGREE_VERTICAL_DOWN);

        Sleep(1.0);

        // Drive forward dragging the counters along
        driveForInches(WHEELS_FIRST, 3.5, DRIVE_POWER / 2.0, 0.04);

        Sleep(1.0);

        upper_servo.SetDegree(170);

        Sleep(1.0);
    }

    // TODO: Make constant
    lower_servo.SetDegree(15);

    //Turn to angle away from foosball counters
    turnCountsInPlace(CLOCKWISE, 1, TURN_POWER);

    //drive to right side wall of course
    DriveSkidFirstUntilHitWall(DRIVE_POWER);

    //drive away from wall
    driveForInches(WHEELS_FIRST, 0.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    //turn to align with acrylic ramp
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    //drive up to acrylic ramp
    driveForInches(WHEELS_FIRST, 8, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // UNTESTED CODE RESUMES HERE
    bool skidFirstDownRamp = true;

    const float yTopOfLongRamp = 53.5;
    const double inchesUpAcrylicRamp = 34;
    // TODO: Completely uncalibrated
    const double yDDRLight = 10;

    // Rotate robot to be ready for skids first down the ramp
    turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn*2, TURN_POWER);
    check_heading(SKIDS_COURSE_BOTTOM);

    check_y_plus(yTopOfLongRamp);

    // Check for being vertically down
    Sleep(RPS_SLEEP_SECONDS);
    check_heading(WHEELS_COURSE_BOTTOM);
    // Drive down ramp
    driveForInches(SKID_FIRST, inchesUpAcrylicRamp, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // TUrn back around and check
    turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn*2, TURN_POWER);
    check_heading(WHEELS_COURSE_BOTTOM);

    check_y_plus(yDDRLight);

    // Detects chosen light color
    bool IS_RED = false;
    if (detectLight(RED_LIGHT_RED_F_V_AVG))
    {
        IS_RED = true;
        LCD.WriteLine("Detected Red Light");
        Sleep(5.0);
    } else {
        LCD.WriteLine("Detected Blue Light");
        Sleep(5.0);
    }

    if (IS_RED) {
        const double inchesToRedButton = 9.0;
        // TODO: Calibrate
        const float xOfRedButton = 12;

        // Turn 90 degrees so the wheel-side of the robot faces the left wall
        turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

        // Allows the robot to settle then checks the heading
        Sleep(RPS_SLEEP_SECONDS);
        check_heading(WHEELS_COURSE_LEFT);

        // Drives the robot up to be level with the red button
        driveForInches(SKID_FIRST, inchesToRedButton, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

        check_x_plus(xOfRedButton);

        // Turn 90 degrees so the wheel-side of the robot faces the buttons
        turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

        // Allows the robot to settle then checks the heading
        Sleep(RPS_SLEEP_SECONDS);
        check_heading(WHEELS_COURSE_BOTTOM);

        // Drives the robot into the button
        driveForSeconds(WHEELS_FIRST, 3.0, (DRIVE_POWER / 3) + 10);

        // Holds the button down
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);
    }
    else {
        // Drives the robot into the blue button
        driveForSeconds(WHEELS_FIRST, 3.0, (DRIVE_POWER / 3) + 10);

        // Holds the button down
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);


    }
    const double inchesToDriveOnStart = 3.40;
    // 11 counts makes a 45 degree turn. Don't mess with it.
    const int countsToTurn = 11;
    // 8 Inches is virtually perfect
    const double inchesToDriveToButton = 7.5;

    // Turn to face ending button
    turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, DRIVE_POWER);

    //Drive back to start
    driveForInches(SKID_FIRST, inchesToDriveToButton, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    //Turn Clockwise about 45 degrees to face end button
    turnCountsInPlace(COUNTER_CLOCKWISE, countsToTurn, TURN_POWER);

    // hit final button
    DriveSkidFirstUntilHitWall(DRIVE_POWER);

    // hit final button
    DriveSkidFirstUntilHitWall(DRIVE_POWER);
}
