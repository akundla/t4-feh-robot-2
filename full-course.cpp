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
    const double InchesToCoin = 6.6;
    const double InchesToCoinSlot = 2;
    const float rampPower = DRIVE_POWER * 1.55;
    const float yTopOfShortRamp = 46.0;
    const int ticksIn90DegreeTurn = 22;
    const float xCoinSlot = 18.25;
    const int ticksToLineUpWithCoinSlot = 22;
    const double inchesToLever = 9.0;
    const int ticksFinalTurnForLever = 7;
    const double finalInchesToLever = 3.35;
    // TODO: Calibrate to the arm skips as few teeth as possible
    const int lowerDegreeToHitLever = 80;
     const float degreeToHitCountersWall = 25; //TODO: Old value was 10, needs to be calibrated a bit
     const double countersDistance = 9.5;

    LCD.WriteLine("Full Course Navigator");

    prepareToLaunch();

    //Turns robot counterclockwise 45 degrees to alight with steep ramp
    turnCountsInPlace(COUNTER_CLOCKWISE, InitialTurn, TURN_POWER);

    //verify robot is facing in the positive y direction
    check_heading(SKIDS_COURSE_TOP-1);

    //Drive robot to top of ramp (No offset, with + left offset it drifted into far wall)
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
    // TODO: Time optimization: Lengthen this distance to eliminate RPS check
    driveForInches(SKID_FIRST, InchesToCoin, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Check that the robot made it to the coin slot
    check_x_plus(xCoinSlot);

    //Turn to align arm
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksToLineUpWithCoinSlot + 1, TURN_POWER);

    //Sleep for RPS
    Sleep(RPS_SLEEP_SECONDS);

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
    driveForInches(WHEELS_FIRST, InchesToCoin + 0.4, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Turn to face the lever
    turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    //RPS Sleep and check
    Sleep(RPS_SLEEP_SECONDS);

    // Check that the wheels face the lever
    check_heading(WHEELS_COURSE_TOP);

    // Prepare arm to hit lever
    lower_servo.SetDegree(LOWER_DEGREE_STRAIGHT_OUT+10);
    upper_servo.SetDegree(15);
    // Wait for servos to finish moving
    Sleep(RPS_SLEEP_SECONDS);

    //check y position
    check_y_plus(44.1);

    //Drive to be be aligned with the lever
    driveForInches(WHEELS_FIRST, inchesToLever, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    //Turn counter-clockwise to angle arm better relative to lever
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksFinalTurnForLever, TURN_POWER);

    Sleep(0.2);

    //Drive toward lever
    driveForInches(WHEELS_FIRST, finalInchesToLever+0.1, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    Sleep(0.4);

    // Whack that lever
    lower_servo.SetDegree(lowerDegreeToHitLever);
    // Sleep so servo can finish moving
    Sleep(0.4);

    upper_servo.SetDegree(15);

    Sleep(0.4);

    // UNTESTED CODE BEGINS HERE

    //Drive back from lever
    driveForInches(SKID_FIRST, 1.0, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    upper_servo.SetDegree(140);
    lower_servo.SetDegree(15);
    Sleep(0.4);

    turnCountsInPlace(CLOCKWISE, ticksFinalTurnForLever, TURN_POWER);

    // Drive back to starting spot for lever
    driveForInches(SKID_FIRST, 9.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // skids face right wall
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    check_heading(SKIDS_COURSE_RIGHT);

    driveForInches(SKID_FIRST, 6.0, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    turnCountsInPlace(COUNTER_CLOCKWISE, 9, TURN_POWER);

    driveForInches(SKID_FIRST, 8.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    turnCountsInPlace(CLOCKWISE, 9, TURN_POWER);

    // Drive until you hit the wall
    DriveSkidFirstUntilHitWall(DRIVE_POWER);
    // SHOULD HAVE HIT RIGHT WALL HERR

    driveForInches(WHEELS_FIRST, 2.0, DRIVE_POWER/2, LEFT_MOTOR_OFFSET);

    // FACE COUNTERS
    turnCountsInPlace(CLOCKWISE, 22, TURN_POWER);

    check_heading(WHEELS_COURSE_TOP);

    // TODO: Maybe adjust if you have issues
    lower_servo.SetDegree(179);

    Sleep(RPS_SLEEP_SECONDS);

    // Drive to counters
    driveForInches(WHEELS_FIRST, 10.25, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // Rotate to be parallel to counters
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn-1, TURN_POWER);

    //Drive in +x direction to line up with wall
    // TODO: ADjust if we get stuck
    DriveSkidFirstUntilHitWall(DRIVE_POWER * 1.35);

    // TESTED (and maybe slightly inconsistent) code begins again here

    // Rotate arm to hit counters back wall
    turnCountsInPlace(CLOCKWISE, 1, TURN_POWER);
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
        driveForInches(WHEELS_FIRST, (countersDistance * 2.0/3.0) + 2.0, DRIVE_POWER / 2.0, 0.035);

        // Wait after turning
        Sleep(0.5);

        upper_servo.SetDegree(170);

        Sleep(0.5);

        // Drive backward dragging the counters along
        driveForInches(SKID_FIRST, 3.0, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

        Sleep(0.5);

        upper_servo.SetDegree(UPPER_DEGREE_VERTICAL_DOWN);

        Sleep(0.5);

        // Drive forward dragging the counters along
        driveForInches(WHEELS_FIRST, 3.5, DRIVE_POWER / 2.0, 0.035);

        Sleep(0.5);

        upper_servo.SetDegree(170);
        Sleep(0.2);
        lower_servo.SetDegree(15);
        Sleep(0.3);
    }

    // FULL COURSE CODE STARTS AGAIN HERE

    //Turn to angle away from foosball counters
    turnCountsInPlace(CLOCKWISE, 3, TURN_POWER);

    driveForInches(SKID_FIRST, 3.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    //Turn to angle away from foosball counters
    turnCountsInPlace(COUNTER_CLOCKWISE, 2, TURN_POWER);

    //drive to right side wall of course
    DriveSkidFirstUntilHitWall(DRIVE_POWER);

    //drive away from wall
    driveForInches(WHEELS_FIRST, 1.75, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    //turn to align with acrylic ramp
    turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    //drive up to acrylic ramp
    driveForInches(SKID_FIRST, 3, DRIVE_POWER, LEFT_MOTOR_OFFSET);

    // UNTESTED CODE RESUMES HERE
    bool skidFirstDownRamp = true;

    const float yTopOfLongRamp = 52.0;
    const double inchesUpAcrylicRamp = 34;
    // TODO: Completely uncalibrated
    const double xRightLight = 27.12;
    const double xLeftButton = 21.25;
    const double yDDRLight = 13.5;

    // TODO: Eliminate for time if we can line up with DDR button without it
    //check_y_plus(yTopOfLongRamp);


    if (skidFirstDownRamp) {
        // Rotate robot to be ready for skids first down the ramp
        Sleep(RPS_SLEEP_SECONDS);
        check_heading(SKIDS_COURSE_BOTTOM+6);
        // Check for being vertically down

        // Drive down ramp
        const double inchesDownAcrylicRamp = inchesUpAcrylicRamp + 4;
        driveForInches(SKID_FIRST, inchesDownAcrylicRamp, DRIVE_POWER + 20, -0.055);

        //check and sleep
        Sleep(RPS_SLEEP_SECONDS);
        check_y_plus(yDDRLight);
        check_heading(SKIDS_COURSE_BOTTOM);

        turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

        check_x_plus(xRightLight);

        turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

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
        Sleep(1.0);
    } else {
        LCD.WriteLine("Detected Blue Light");
        Sleep(1.0);
    }

    if (IS_RED) {
        const double inchesToRedButton = 6.25;

        driveForInches(SKID_FIRST, 1.0, DRIVE_POWER, LEFT_MOTOR_OFFSET);

        // Turn 90 degrees so the wheel-side of the robot faces the right wall
        turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

        // Allows the robot to settle then checks the heading
        check_heading(WHEELS_COURSE_RIGHT);
        Sleep(RPS_SLEEP_SECONDS);

        // Drives the robot up to be level with the red button
        driveForInches(SKID_FIRST, inchesToRedButton, DRIVE_POWER / 2.0, LEFT_MOTOR_OFFSET);

        check_x_plus(xLeftButton);

        // Turn 90 degrees so the wheel-side of the robot faces the buttons
        turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

        // Allows the robot to settle then checks the heading
        check_heading(WHEELS_COURSE_BOTTOM);
        Sleep(RPS_SLEEP_SECONDS);

        // Drives the robot into the button
        driveForSeconds(WHEELS_FIRST, 3.0, (DRIVE_POWER / 3) + 10);

        // Holds the button down
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);

        // Turn to face ending button
        turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn, DRIVE_POWER);

        // 8 Inches is virtually perfect
        const double inchesToDriveToButton = 10;

        //Drive back to start
        driveForInches(SKID_FIRST, inchesToDriveToButton, DRIVE_POWER, LEFT_MOTOR_OFFSET);
    }
    else {
        // turn around to face button
        turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn * 2, TURN_POWER);

        Sleep(RPS_SLEEP_SECONDS);
        check_heading(WHEELS_COURSE_BOTTOM);


        // Drives the robot into the blue button
        driveForSeconds(WHEELS_FIRST, 3.0, (DRIVE_POWER / 3) + 10);

        // Holds the button down
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);

        // Turn to face ending button
        turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn-3, DRIVE_POWER);

        //Drive back to start
        driveForInches(SKID_FIRST, 14.5, DRIVE_POWER, LEFT_MOTOR_OFFSET);
    }
    // 11 counts makes a 45 degree turn. Don't mess with it.
    const int countsToTurn = 11;

    //Turn Clockwise about 45 degrees to face end button
    turnCountsInPlace(COUNTER_CLOCKWISE, countsToTurn, TURN_POWER);

    // hit final button
    DriveSkidFirstUntilHitWall(DRIVE_POWER);
}
