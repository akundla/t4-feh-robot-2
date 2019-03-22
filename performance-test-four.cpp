#include "common.h"
#include "cds-cell-functions.h"
#include "shaft-encoders-drive-functions.h"
#include "time-drive-functions.h"
#include "rps-functions.h"
#include "bump-switch-functions.h"

void performanceTestFour() {

    // Flag to make it go to blue
    bool forceBlue = false;

    // Calibration Values
    const double inchesToDriveOnStart = 3.60;
    // 11 counts makes a 45 degree turn.
    const int countsToTurn = 11;
    // Distance to drive to red button
    const double inchesToDriveToButton = 7.5;

    //Clear the screen
    LCD.Clear();

    // Set arm servos to initial position
    lower_servo.SetDegree(0);
    upper_servo.SetDegree(140);

    float x, y;
    LCD.WriteLine("Performance test 3 servo testing");
    LCD.WriteLine("Set the arm to the correct position you robot champion!");
    LCD.WriteLine("Then touch the screen to continue");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    //Print the RPS course selection menu
    RPS.InitializeTouchMenu();

    //Clear the screen
    LCD.Clear();

    //Print testing information
    LCD.WriteLine("Performance test 4");
    LCD.WriteLine("Waiting for start light...");

    // Robot will begin on start light
    waitForStartLight();

    LCD.Clear();
    LCD.WriteLine("Driving...");

    // Drives out from starting box
    driveForInches(SKID_FIRST, inchesToDriveOnStart, DRIVE_POWER);

    LCD.Clear();
    LCD.WriteLine("Turning 45 degrees clockwise...");

    // Turn Clockwise about 45 degrees to face DDR buttons
    turnCountsInPlace(CLOCKWISE, countsToTurn, TURN_POWER);

    //check robot heading   ALYZA ADDED
    check_heading(356);

    LCD.Clear();
    LCD.WriteLine("Driving to red button...");

    // Drive to the first DDR button
    driveForInches(SKID_FIRST, inchesToDriveToButton, DRIVE_POWER);

    // Pauses to allow the robot to settle
    LCD.Clear();
    Sleep(1.0);

    // Detects red light
    bool IS_RED = false;
    if (!forceBlue && cdsCell.Value() < RED_LIGHT_RED_F_V_AVG + MoE && cdsCell.Value() > RED_LIGHT_RED_F_V_AVG - MoE)
    {
        IS_RED = true;
        LCD.Clear();
        LCD.WriteLine("Detected Red Light");
        Sleep(1.0);
    } else {
        LCD.Clear();
        LCD.WriteLine("Detected Blue Light");
        Sleep(1.0);
    }

    // Hits the red button. This is DIALED IN, don't recalibrate unless it stops working - AK 8:55PM Wednesday 27th February 2019
    if (IS_RED) {
        // Drives the robot up to be level with the red button

        //check heading before driving to align with red button
        check_heading(0);

        driveForInches(SKID_FIRST, 3.5, DRIVE_POWER / 2.0);

        // Turn 90 degrees so the wheel-side of the robot faces the buttons
        turnCountsInPlace(COUNTER_CLOCKWISE, 22, TURN_POWER);

        //check robot angle
        check_heading(90);

        // Drives the robot into the button
        driveForSeconds(WHEELS_FIRST, 3.0, (DRIVE_POWER / 3) + 10);

        // Holds the button down
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER);

        //Turn 90 Degrees to face the second light
        turnCountsInPlace(CLOCKWISE, 22, TURN_POWER);;

        //Drive to second light
        DriveSkidFirstUntilHitWall(DRIVE_POWER);

        //drive slightly away from wall
        driveForInches(WHEELS_FIRST, 1.25, DRIVE_POWER);

        //Turn to face acrylic ramp
        turnCountsInPlace(COUNTER_CLOCKWISE, 22, TURN_POWER);
    }
    // Otherwise the light is blue
    else {
        // TODO: Calibrate, it's definitely more than 5
        const double inchesToBlueButton = 8.0;

        //check robot heading
        check_heading(0);

        // Drives the robot up to be level with the second button
        driveForInches(SKID_FIRST, inchesToBlueButton, DRIVE_POWER / 2.0);

        // Turn 90 degrees so the wheel-side of the robot faces the buttons
        turnCountsInPlace(COUNTER_CLOCKWISE, 24, TURN_POWER);

        //check robot angle
        check_heading(90);

        // Drives the robot into the button
        driveForSeconds(WHEELS_FIRST, 2.0, (DRIVE_POWER / 3) + 10);

        // Hold down the button
        Sleep(5.0);

        // Releases the button
        driveForInches(SKID_FIRST, 0.5, DRIVE_POWER);
    }

    const double inchesUpAcrylicRamp = 34;
    const double ticksIn90DegreeTurn = 22;
    const double inchesToLineUpWithCounters = 1.5;
    const double inchesToCounters = 9.5;
    const float degreeToHitCountersWall = 20;
    const double countersDistance = 9.5;
    const float degreePastVertical = 0;
    const float degreeBack = 25;
    const double inchesToLever = 10;
    const double inchesPastLever = 5;
    const double inchesToSteepRamp = 10;
    const double inchesDownRamp = 20;
    const double secondsToPressButton = 5.0;

    const double xTopOfSteepRamp = 10;
    const double yTopOfSteepRamp = 45;

    // TODO: Find point at top of acrylic ramp for adjustment
    const double xPosition = 30;
    const double yPosition = 53.5;

    //Mitchie needs to rest
    Sleep(1.0);

    //Check heading before going up ramp, account for left-drifting tendency
    check_heading(72);

    // Drive up the acrylic ramp
    driveForInches(SKID_FIRST, inchesUpAcrylicRamp, DRIVE_POWER+15);

    //Check angle
    check_heading(85);
    check_y_plus(yPosition);

    // Drive forward enough to turn
    driveForInches(SKID_FIRST, 3.0, DRIVE_POWER);

    // Turn 90 degrees so the skids point left
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    // Check and correct positioning and drive to line up with counters
    driveForInches(SKID_FIRST, inchesToLineUpWithCounters, DRIVE_POWER);

    // Turn 90 degrees so the wheels and arm face the counters
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn+1, TURN_POWER);

    // Stick the arm straight out so it doesn't hit the frame of the foosball counters
    lower_servo.SetDegree(180);

    // Drive to counters
    driveForInches(WHEELS_FIRST, inchesToCounters, DRIVE_POWER);

    //fix mitchie geting stuck when she sees the red light
    if(IS_RED)
    {
        driveForInches(SKID_FIRST, 0.1, DRIVE_POWER);
    }

    // Rotate to be parallel to counters
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn-1, TURN_POWER);

    //Drive in +x direction to line up with wall
    DriveSkidFirstUntilHitWall(DRIVE_POWER);

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

    // Drive forward dragging the counters along
    driveForInches(WHEELS_FIRST, countersDistance, DRIVE_POWER / 2.0);

    // Rotate arm on its axis to no longer touch counters
    upper_servo.SetDegree(140);

    // TODO: Make constant
    lower_servo.SetDegree(0);

    //Turn to angle away from foosball counters
    turnCountsInPlace(CLOCKWISE, 1, TURN_POWER);

    //drive to right side wall of course
    DriveSkidFirstUntilHitWall(DRIVE_POWER);

    //drive away from wall
    driveForInches(WHEELS_FIRST, 0.5, DRIVE_POWER);

    //turn to align with acrylic ramp
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn, TURN_POWER);

    //drive up to acrylic ramp
    driveForInches(WHEELS_FIRST, 8, DRIVE_POWER);

    //turn to face left wall
    turnCountsInPlace(COUNTER_CLOCKWISE, ticksIn90DegreeTurn+1, TURN_POWER);

    //drive to left wall
    DriveSkidFirstUntilHitWall(DRIVE_POWER);

    //turn to face the steep ramp
    turnCountsInPlace(CLOCKWISE, ticksIn90DegreeTurn+1, TURN_POWER);

    //hit final button
    driveForInches(WHEELS_FIRST, 40, DRIVE_POWER+15);
}
