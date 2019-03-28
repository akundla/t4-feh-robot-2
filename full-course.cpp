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
    const float rampPower = DRIVE_POWER * 1.55;
    const float yTopOfShortRamp = 46.0;
    const int ticksIn90DegreeTurn = 22;
    const float xCoinSlot = 18.75;
    const int ticksToLineUpWithCoinSlot = 22;
    const double inchesToLever = 9.5;
    const int ticksFinalTurnForLever = 3;
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
    driveForInches(SKID_FIRST, InchesUpRamp, rampPower, 0);

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
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksToLineUpWithCoinSlot + 1, TURN_POWER);

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
    driveForInches(WHEELS_FIRST, InchesToCoin + 0.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Turn to face the lever
    turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    // Check that the wheels face the lever
    check_heading(WHEELS_COURSE_TOP);

    // Prepare arm to hit lever
    lower_servo.SetDegree(LOWER_DEGREE_STRAIGHT_OUT);
    upper_servo.SetDegree(15);
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
    Sleep(0.4);

    upper_servo.SetDegree(15);

    // UNTESTED CODE BEGINS HERE

    //Drive back from lever
    driveForInches(SKID_FIRST, 1.0, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    upper_servo.SetDegree(140);
    Sleep(0.4);

    // Turn so skids face right wall
    turnCountsInPlace(CLOCKWISE, ticksFinalTurnForLever, TURN_POWER);

    driveForInches(SKID_FIRST, 9.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    check_heading(SKIDS_COURSE_RIGHT);

    driveForInches(SKID_FIRST, 6.0, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    turnCountsInPlace(COUNTER_CLOCKWISE, 9, TURN_POWER);

    driveForInches(SKID_FIRST, 8.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    turnCountsInPlace(CLOCKWISE, 9, TURN_POWER);

    // Drive until you hit the wall
    DriveSkidFirstUntilHitWall(DRIVE_POWER);

    driveForInches(WHEELS_FIRST, 1.5, DRIVE_POWER/2, LEFT_MOTOR_OFFSET);

    turnCountsInPlace(CLOCKWISE, 22, TURN_POWER);

    check_heading(WHEELS_COURSE_TOP);

    Sleep(RPS_SLEEP_SECONDS);

    // Drive to counters
    driveForInches(WHEELS_FIRST, 11, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Rotate to be parallel to counters
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn-1, TURN_POWER);

    //Drive in +x direction to line up with wall
    DriveSkidFirstUntilHitWall(DRIVE_POWER * 2.0);

    // TESTED (and maybe slightly inconsistent) code begins again here

    // Rotate arm to hit counters back wall
    lower_servo.SetDegree(degreeToHitCountersWall);

    // Wait after turning
    Sleep(0.5);

    // Rotate arm on its axis to hit counters
    upper_servo.SetDegree(UPPER_DEGREE_VERTICAL_DOWN);

    // Wait after turning
    Sleep(0.5);

    bool shouldYEET = true;

    if (!shouldYEET) {
        // Drive forward dragging the counters along
        driveForInches(WHEELS_FIRST, countersDistance - 1.0, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

        // Wait after turning
        Sleep(0.5);

        // TODO: Make constant
        lower_servo.SetDegree(60);

        // Wait after turning
        Sleep(0.5);

        // Rotate arm on its axis to shove counters into wall
        upper_servo.SetDegree(170);

        // Wait after turning
        Sleep(0.5);
    } else {
        // Drive forward dragging the counters along
        driveForInches(WHEELS_FIRST, (countersDistance * 2.0/3.0) + 2.0, DRIVE_POWER / 2.0, 0.05);

        // Wait after turning
        Sleep(0.5);

        upper_servo.SetDegree(170);

        Sleep(0.5);

        // Drive forward dragging the counters along
        driveForInches(SKID_FIRST, 3.0, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

        Sleep(0.5);

        upper_servo.SetDegree(UPPER_DEGREE_VERTICAL_DOWN);

        Sleep(0.5);

        // Drive forward dragging the counters along
        driveForInches(WHEELS_FIRST, 3.5, DRIVE_POWER / 2.0, 0.05);

        Sleep(0.5);

        upper_servo.SetDegree(170);

        Sleep(0.5);
    }

    // TODO: Make constant
    lower_servo.SetDegree(15);

    //Turn to angle away from foosball counters
    turnCountsInPlace(CLOCKWISE, 1, TURN_POWER);

    driveForInches(SKID_FIRST, 3.0, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    //Turn to angle away from foosball counters
    turnCountsInPlace(CLOCKWISE, 3, TURN_POWER);

    //drive to right side wall of course
    DriveSkidFirstUntilHitWall(DRIVE_POWER);

    //drive away from wall
    driveForInches(WHEELS_FIRST, 1.0, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    //turn to align with acrylic ramp
    turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    //drive up to acrylic ramp
    driveForInches(SKID_FIRST, 8, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // UNTESTED CODE RESUMES HERE
    bool skidFirstDownRamp = true;

    const float yTopOfLongRamp = 53.5;
    const double inchesUpAcrylicRamp = 34;
    // TODO: Completely uncalibrated
    const double xRightLight = 29.12;
    const double xLeftButton = 24.3;
    const double yDDRLight = 13.5;

     check_y_plus(yTopOfLongRamp);


    if (skidFirstDownRamp) {
        // Rotate robot to be ready for skids first down the ramp
        check_heading(SKIDS_COURSE_BOTTOM);
        // Check for being vertically down
        Sleep(RPS_SLEEP_SECONDS);

        // Drive down ramp
        driveForInches(SKID_FIRST, inchesUpAcrylicRamp - 4, DRIVE_POWER, LEFT_MOTOR_OFFSET);

        if (RPS.X() > xRightLight) {
            turnCountsInPlace(CLOCKWISE, 5, TURN_POWER);

            check_x_plus(xRightLight);

            turnCountsInPlace(COUNTER_CLOCKWISE, 5, TURN_POWER);
        } else {
            turnCountsInPlace(COUNTER_CLOCKWISE, 5, TURN_POWER);

            check_x_plus(xRightLight);

            turnCountsInPlace(CLOCKWISE, 5, TURN_POWER);
        }
        check_heading(SKIDS_COURSE_BOTTOM);

        Sleep(RPS_SLEEP_SECONDS);

        check_y_plus(yDDRLight);

    } else {
        check_heading(WHEELS_COURSE_BOTTOM);
        // Check for being vertically down
        Sleep(RPS_SLEEP_SECONDS);

        // Drive down ramp
        driveForInches(WHEELS_FIRST, inchesUpAcrylicRamp, DRIVE_POWER, LEFT_MOTOR_OFFSET);
    }

    // Detects chosen light color
    check_y_plus(yDDRLight);
    bool IS_RED = false;
    if (detectLight(RED_LIGHT_RED_F_V_AVG))
    {
        IS_RED = true;
        LCD.WriteLine("Detected Red Light");
    } else {
        LCD.WriteLine("Detected Blue Light");
    }

    if (IS_RED) {
        const double inchesToRedButton = 5.0;

        // Turn 90 degrees so the wheel-side of the robot faces the left wall
        turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

        // Allows the robot to settle then checks the heading
        check_heading(WHEELS_COURSE_LEFT);
        Sleep(RPS_SLEEP_SECONDS);

        // Drives the robot up to be level with the red button
        driveForInches(WHEELS_FIRST, inchesToRedButton, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

        check_x_plus(xLeftButton);

        // Turn 90 degrees so the wheel-side of the robot faces the buttons
        turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

        // Allows the robot to settle then checks the heading
        check_heading(WHEELS_COURSE_BOTTOM);
        Sleep(RPS_SLEEP_SECONDS);

        // Drives the robot into the button
        driveForSeconds(WHEELS_FIRST, 3.0, (DRIVE_POWER / 3) + 10);

        // Holds the button down
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);
    }
    else {
        // turn around to face button
        turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn * 2, TURN_POWER);

        check_heading(WHEELS_COURSE_BOTTOM);
        Sleep(RPS_SLEEP_SECONDS);

        // Drives the robot into the blue button
        driveForSeconds(WHEELS_FIRST, 3.0, (DRIVE_POWER / 3) + 10);

        // Holds the button down
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);
    }
    // 11 counts makes a 45 degree turn. Don't mess with it.
    const int countsToTurn = 11;
    // 8 Inches is virtually perfect
    const double inchesToDriveToButton = 7.5;

    // Turn to face ending button
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn, DRIVE_POWER);

    //Drive back to start
    driveForInches(SKID_FIRST, inchesToDriveToButton, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    //Turn Clockwise about 45 degrees to face end button
    turnCountsInPlace(COUNTER_CLOCKWISE, countsToTurn, TURN_POWER);

    // hit final button
    DriveSkidFirstUntilHitWall(DRIVE_POWER);
}
