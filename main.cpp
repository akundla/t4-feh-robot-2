// INCLUDE LIBRARIES
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHRPS.h>

//Include the FEHMotor library
#include <FEHMotor.h>
//Include the FEHServo library
#include <FEHServo.h>

// DECLARE GLOBAL CONSTANTS

// Power offset added to the left motor to make the robot drive straight
#define LEFT_MOTOR_OFFSET 1.0

#define SKID_FIRST true
#define WHEELS_FIRST false

#define CLOCKWISE true
#define COUNTER_CLOCKWISE false

// Standard percentage of motor power for driving forwards or backwards
#define DRIVE_POWER 40.0

// Standard percentage of motor power for turning the robot in place
#define TURN_POWER 20.0

// SENSORS
//Declare a CdS Cell sensor as an analog input and assign it to an IO port
AnalogInputPin cdsCell (FEHIO::P0_0);


//Declare a microswitch as a digital input and assign it to an IO port
DigitalInputPin frontLeftBump (FEHIO::P3_6);
DigitalInputPin frontRightBump (FEHIO::P3_1);
DigitalInputPin backLeftBump (FEHIO::P3_7);
DigitalInputPin backRightBump (FEHIO::P3_0);

AnalogInputPin leftOptosensor (FEHIO::P2_2);
AnalogInputPin middleOptosensor (FEHIO::P2_1);
AnalogInputPin rightOptosensor (FEHIO::P2_0);

// ENCODERS
DigitalEncoder left_encoder(FEHIO::P1_7);
DigitalEncoder right_encoder(FEHIO::P1_0);

#define LEFT_TICKS_PER_REV 260
#define RIGHT_TICKS_PER_REV 350
// It goes about 277 - 289

//MOTORS
//Assign the right and left motors to motor ports with a max voltage of 9.0V
FEHMotor leftMotor (FEHMotor :: Motor0, 9.0);
FEHMotor rightMotor (FEHMotor :: Motor1, 9.0);

//Declare a servo motor and assign it to a servo port
FEHServo servo (FEHServo::Servo0);

// LIGHT CONSTANTS
// Voltage reading from CDS cell when cell is covered (no light, pitch black)
#define NO_LIGHT_V 3.400

// CDS Cell average Voltage readings
#define BLUE_LIGHT_NO_FILTER_V_AVG 0.524
#define RED_LIGHT_NO_FILTER_V_AVG 0.188
#define BLUE_BACKLGROUND_V_AVG 3.200

// Ratio that a CDS cell reading is multiplied by to create a degree (0 - 180) to which to move the servo
// CDS cell has an approximate voltage range of 0 - 3.400 volts. Servo has range of 0 - 180 degrees. 180 / 3.400 = 52.94
#define RATIO_SERVO_DEGREE_TO_CDS_CELL 52.94

// CALIBRATION VALUES
// Calibration values for Exploration 1 servo
#define SERVO_MIN 769
#define SERVO_MAX 2490

// Constants from exploration 1
// Left optosensor
#define LEFT_RED 1.250
#define LEFT_LIGHT_BACKGROUND 1.635
#define LEFT_DARK_BACKGROUND 1.691
#define LEFT_BLACK 2.293

// Middle optosensor
#define MID_RED 1.440
#define MID_LIGHT_BACKGROUND 1.626
#define MID_DARK_BACKGROUND 1.685
#define MID_BLACK 2.358

// Right optosensor
#define RIGHT_RED 1.650
#define RIGHT_LIGHT_BACKGROUND 1.912
#define RIGHT_DARK_BACKGROUND 2.028
#define RIGHT_BLACK 2.434

// This margin is wider than necessary for easy detection on clear-contrast backgrounds
#define MoE 0.500


// CALIBRATION FUNCTIONS

// Calibrates the servo tested in Exploration 1
void calibrateServo() {
    // Calibrate Servo
    servo.SetMin(SERVO_MIN);
    servo.SetMax(SERVO_MAX);
}


// DRIVE FUNCTIONS

// TODO: Fine-tune for each motor setup
//Create a function to drive motors forward until microswitch is pressed.
void DriveForwardUntilHitWall(int left_motor_percent, int right_motor_percent)
{
    //Turn both motors on at given percent motor power.
    leftMotor.SetPercent(left_motor_percent + LEFT_MOTOR_OFFSET);
    rightMotor.SetPercent(right_motor_percent);

    // Psuedo-infinite loop to burn time while both switches are not pressed.
    // Note that bump switches are "true" when not pressed and "false" when pressed
    while (frontLeftBump.Value() == 1 || frontRightBump.Value() == 1) { }

    // Stops motors
    leftMotor.Stop();
    rightMotor.Stop();
}

// TODO: Fine-tune for each motor setup
// Function to reverse the robot to the left until a wall is hit
void BackLeftTurnUntilHitWall()
{
    // Set both motors to a low power level
    leftMotor.SetPercent(-10);
    rightMotor.SetPercent(-25);

    // Psuedo-infinite loop to burn time while both switches are not pressed.
    // Note that bump switches are "true" when not pressed and "false" when pressed
    while (backLeftBump.Value() == 1 || backRightBump.Value() == 1) { }

    // Stops motors
    leftMotor.Stop();
    rightMotor.Stop();
}

// TODO: Fine-tune for each motor setup
// Function to reverse the robot back to the right until one switch hits a wall
void BackRightTurnUntilCornerHitWall()
{
    // Set both motors to a low power level
    leftMotor.SetPercent(-50);
    rightMotor.SetPercent(-10);

    // Psuedo-infinite loop to burn time while one switch is not pressed.
    // Note that bump switches are "true" when not pressed and "false" when pressed
    while (backLeftBump.Value() == 1 && backRightBump.Value() == 1) { }

    // Stops left motor
    leftMotor.Stop();

    // Drives the right motor forward to make the robot square in the course
    rightMotor.SetPercent(25);

    // Allows the right motor to run for 2 tenths of a second
    Sleep(0.2);

    // Stops right motor
    rightMotor.Stop();
}


// LIGHT FUNCTIONS

// Turns a servo motor in response to the reading from a CDS light cell
void printCDsCellValues() {
    // Runs continuously
    while(true) {
        //Print the value of the CdS cell to the screen.
        LCD.Write("CDs Cell: ");
        LCD.Write(cdsCell.Value());
        LCD.Write(" V");

        LCD.Clear(BLACK);
    }
}

// Turns a servo motor in response to the reading from a CDS light cell
void moveServoToLight() {
    // Runs continuously
    while(true) {
        //Print the value of the CdS cell to the screen.
        LCD.Write("Voltage from CDS cell: ");
        LCD.Write(cdsCell.Value());
        LCD.Write("Volts");

        // Set the servo arm depending on the amount of light from the CDS cell
        // 0 light corresponds to 0 degress and full light to 180 degrees
        servo.SetDegree( (NO_LIGHT_V - cdsCell.Value() ) * RATIO_SERVO_DEGREE_TO_CDS_CELL);
    }
}

// Runs continuously and prints to the screen if the CDS cell detects blue light
void detectBlueLight() {

    // Acceptable margin of error (+/-) in voltage value (determines window in which light can be detected)
    double LightV_MoE = 0.01;

    // If the voltage is within the MoE of blue light, prints to the screen that the light is blue
    if (cdsCell.Value() < BLUE_LIGHT_NO_FILTER_V_AVG + LightV_MoE && cdsCell.Value() > BLUE_LIGHT_NO_FILTER_V_AVG - LightV_MoE) {
        LCD.WriteLine("Blue Light detected");
    } else {
        LCD.WriteLine("Blue Light not detected");
    }
}

// EXPLORATION 2 CODE
void driveDistanceForward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    rightMotor.SetPercent(percent);
    leftMotor.SetPercent(-percent);

    // Runs motors while the average of the left and right encoder is less than counts,
    //keep running motors
    while(right_encoder.Counts() < counts) {
        LCD.WriteLine("L: ");
        LCD.Write(left_encoder.Counts());
        LCD.WriteLine("R: ");
        LCD.WriteLine(right_encoder.Counts());
        LCD.WriteLine("");
    }

    LCD.Clear();

    LCD.WriteLine("L: ");
    LCD.Write(left_encoder.Counts());
    LCD.WriteLine("R: ");
    LCD.WriteLine(right_encoder.Counts());
    LCD.WriteLine("");

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();
}


// TODO: Calibrate this to turn the correct direction. In the exploration this actually turned left.
// TODO: Use the percent parameter to specify how fast to turn
void turn_right(int percent, int counts) {
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    rightMotor.SetPercent(-15);
    leftMotor.SetPercent(30);

    // Runs motors while the average of the left and right encoder is less than counts
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts);

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();
 }


// TODO: Calibrate this to turn the correct direction. In the exploration this actually turned right.
// TODO: Use the percent parameter to specify how fast to turn
void turn_left(int percent, int counts) {
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    rightMotor.SetPercent(30);
    leftMotor.SetPercent(-15);

    // Runs motors while the average of the left and right encoder is less than counts
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts);

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();
 }

void explorationTwoShaftEncoders () {
    //Input power level here
    int motor_percent = 25;
    // How many ticks the encoder reports per revolution
    int ticksPerRevolution = 48;
    // Number of ticks to turn should be similar one revolution
    // TODO: Test this value
    int ticksPerTurn = 48;

    // The number of ticks to move
    int ticksToMove = ticksPerRevolution;

    // Garbage variables for touch screen
    float x, y;

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Shaft Encoder Exploration Test");
    LCD.WriteLine("Touch the screen to begin");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    // Move forward one revolution
    driveDistanceForward(motor_percent, ticksToMove);

    // Call turn right function
    turn_right(motor_percent, ticksPerTurn);

    // Move forward one revolution
    driveDistanceForward(motor_percent, ticksToMove);

    // call turn left function
    turn_left(motor_percent, ticksPerTurn);

    //reset expected counts to move 4 inches
    ticksToMove = 162;

    // Drive forward one revolution
    driveDistanceForward(motor_percent, ticksToMove);
}

// COMBINATION FUNCTIONS

// Uses functions written above to navigate the Exploration 1 course
void navigateExploration1Course() {

    int QUARTER_POWER_PERCENT = 25;

    DriveForwardUntilHitWall(QUARTER_POWER_PERCENT, QUARTER_POWER_PERCENT);
    BackLeftTurnUntilHitWall();
    DriveForwardUntilHitWall(QUARTER_POWER_PERCENT, QUARTER_POWER_PERCENT);
    BackRightTurnUntilCornerHitWall();
    DriveForwardUntilHitWall(QUARTER_POWER_PERCENT, QUARTER_POWER_PERCENT);
}

void lineFollowerPrintValues() {

    float trashX, trashY;

    while (true) {

        LCD.WriteLine("Left optosensor voltages: ");
        for (int i = 0; i < 5; i++) {
            LCD.WriteLine(leftOptosensor.Value());
        }

        while (!LCD.Touch(&trashX, &trashY)) {}
        while (LCD.Touch(&trashX, &trashY)) {}

        LCD.WriteLine("Middle optosensor voltages: ");
        for (int i = 0; i < 5; i++) {
            LCD.WriteLine(middleOptosensor.Value());
        }

        while (!LCD.Touch(&trashX, &trashY)) {}
        while (LCD.Touch(&trashX, &trashY)) {}

        LCD.WriteLine("Right optosensor voltages: ");
        for (int i = 0; i < 5; i++) {
            LCD.WriteLine(rightOptosensor.Value());
        }

        while (!LCD.Touch(&trashX, &trashY)) {}
        while (LCD.Touch(&trashX, &trashY)) {}
    }
}

// Navigates the robot along the black line
void FollowBlackLine(){

    // Robot navigation state
    bool leftOfLine = false;
    bool rightOfLine = false;
    bool onLine = false;

    float leftVal, midVal, rightVal;

    while(true) {
        // Take values
        leftVal = leftOptosensor.Value();
        midVal = middleOptosensor.Value();
        rightVal = rightOptosensor.Value();

        // if left sees line, position = right of line
        if (leftVal > LEFT_BLACK - MoE && leftVal < LEFT_BLACK + MoE) {
            leftOfLine = false;
            rightOfLine = true;
            onLine = false;
        }

        // if right sees line, position = left of line
        else if (rightVal > RIGHT_BLACK - MoE && rightVal < RIGHT_BLACK + MoE) {
            leftOfLine = true;
            rightOfLine = false;
            onLine = false;
        }

        // if middle sees line, position = on line
        else if (midVal > MID_BLACK - MoE && midVal < MID_BLACK + MoE) {
            leftOfLine = false;
            rightOfLine = false;
            onLine = true;
        }

        if (onLine) {

            LCD.WriteLine("On line");

            leftMotor.SetPercent(15);
            rightMotor.SetPercent(15);
        } else if (rightOfLine) {

            LCD.WriteLine("RightOfLine");

            leftMotor.SetPercent(10);
            rightMotor.SetPercent(30);
        } else if (leftOfLine) {

            LCD.WriteLine("LeftOfLine");

            leftMotor.SetPercent(30);
            rightMotor.SetPercent(10);
        }
    }
}

// Makes the robot drive along the red line in Exploration 2
void FollowRedLine(){

    bool leftOfLine = false;
    bool rightOfLine = false;
    bool onLine = false;

    float leftVal, midVal, rightVal;

    while(true) {
        // Take values
        leftVal = leftOptosensor.Value();
        midVal = middleOptosensor.Value();
        rightVal = rightOptosensor.Value();

        // if left sees line, position = right of line
        if (leftVal > LEFT_RED - MoE && leftVal < LEFT_RED + MoE) {
            leftOfLine = false;
            rightOfLine = true;
            onLine = false;
        }

        // if right sees line, position = left of line
        else if (rightVal > RIGHT_RED - MoE && rightVal < RIGHT_RED + MoE) {
            leftOfLine = true;
            rightOfLine = false;
            onLine = false;
        }

        // if middle sees line, position = on line
        else if (midVal > MID_RED - MoE && midVal < MID_RED + MoE) {
            leftOfLine = false;
            rightOfLine = false;
            onLine = true;
        }

        if (onLine) {

            LCD.WriteLine("On line");

            leftMotor.SetPercent(25);
            rightMotor.SetPercent(25);
        } else if (rightOfLine) {

            LCD.WriteLine("RightOfLine");

            leftMotor.SetPercent(15);
            rightMotor.SetPercent(25);
        } else if (leftOfLine) {

            LCD.WriteLine("LeftOfLine");

            leftMotor.SetPercent(25);
            rightMotor.SetPercent(15);
        }
    }
}

void waitForStartLight () {

    // Runs (burns time, makes robot wait) while the cdsCell detects light that is not in the voltage range of Red
    while (cdsCell.Value() > RED_LIGHT_NO_FILTER_V_AVG + MoE || cdsCell.Value() < RED_LIGHT_NO_FILTER_V_AVG - MoE) {
        LCD.WriteLine(cdsCell.Value());
    }
}

/**
 *  driveForSeconds: Drives the robot in a straight line. Can drive the
 *  robot either skids first or wheels first, for any duration of time
 *  and motor power percentage.
 *
 * @param bool skidFirst: Controls whether the robot drives skid first or wheels-first.
 * @param double seconds: The number of seconds for which the robot will drive.
 * @param int motorPowerPercent: The percentage at which the robot will drive.
 * */
void driveForSeconds(bool skidFirst, double seconds, int motorPowerPercent) {
    LCD.WriteLine("Driving for ");
    LCD.Write(seconds);
    LCD.Write(" seconds");

    // Left and Right motor must be driven at different percentages
    double leftMotorPowerPercent = 0.0;
    double rightMotorPowerPercent = 0.0;

    // Drives skids first
    if (skidFirst) {
        leftMotorPowerPercent = motorPowerPercent + LEFT_MOTOR_OFFSET;
        rightMotorPowerPercent = -motorPowerPercent;
    }
    // Drives wheels first
    else {
        leftMotorPowerPercent = -(motorPowerPercent + LEFT_MOTOR_OFFSET);
        rightMotorPowerPercent = motorPowerPercent;
    }

    // Sets the motors in motion
    leftMotor.SetPercent(leftMotorPowerPercent);
    rightMotor.SetPercent(rightMotorPowerPercent);

    // Runs the motors for the specified time
    Sleep(seconds);

    leftMotor.Stop();
    rightMotor.Stop();
}

void turnInPlace(bool turnClockwise, double seconds, int motorPowerPercent) {
    LCD.WriteLine("Turning around");

    // Left and Right motor must be driven at different percentages
    double leftMotorPowerPercent = 0.0;
    double rightMotorPowerPercent = 0.0;

    // Turns clockwise
    if (turnClockwise) {
        leftMotorPowerPercent = -(motorPowerPercent + LEFT_MOTOR_OFFSET);
        rightMotorPowerPercent = -motorPowerPercent;
    }
    // Turns counterclockwise
    else {
        leftMotorPowerPercent = motorPowerPercent + LEFT_MOTOR_OFFSET;
        rightMotorPowerPercent = motorPowerPercent;
    }

    // Sets the motors in motion
    leftMotor.SetPercent(leftMotorPowerPercent);
    rightMotor.SetPercent(rightMotorPowerPercent);

    // Turns for the specified duration
    Sleep(seconds);

    // Stops both motors
    leftMotor.Stop();
    rightMotor.Stop();
}

// Turns the robot about 180 degrees clockwise at the top of the ramp
void turnAround() {
    const double secondsToTurn = 3.30;
    turnInPlace(CLOCKWISE, secondsToTurn, TURN_POWER);
}

// Turns the robot about 45 degrees counterclockwise in the starting box
void turnOnStart() {
    const double secondsToTurn = 0.725;
    turnInPlace(CLOCKWISE, secondsToTurn, TURN_POWER);
}

/*
 * Navigates through the course to complete the first performance test.
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
    servo.SetDegree(120);

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
    servo.SetDegree(0);
    // Pauses to allow the arm to actually hit said lever
    Sleep(0.5);

    // Drives back down the ramp
    driveForSeconds(SKID_FIRST, secondsToDriveBack, DRIVE_POWER);

    rightMotor.Stop();
    leftMotor.Stop();
}

void printShaftEncoderValues () {
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Runs motors while the average of the left and right encoder is less than counts,
    //keep running motors
    while (true) {
        LCD.WriteLine("L: ");
        LCD.WriteLine(left_encoder.Counts());

        LCD.WriteLine("R: ");
        LCD.WriteLine(right_encoder.Counts());
        LCD.WriteLine("");
    }
}

void performanceTestTwo () {
    float x, y;
    //Calibration Values
    const double secondsToFirstDDRButton = 5.0;
    const double secondsAfterStart = 0.5;

    //Set initial servo degree
    servo.SetDegree(180);

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

// Exploration 3

void printRPSLocation() {
    float touch_x, touch_y;

    //Call this function to initialize the RPS to a course
    RPS.InitializeTouchMenu();

    //Wait for touchscreen to be pressed
    LCD.WriteLine("Press Screen to Start");
    while(!LCD.Touch(&touch_x, &touch_y));

    LCD.Clear();

   //Write initial screen info
   LCD.WriteRC("RPS Test Program",0,0);
   LCD.WriteRC("X Position:",2,0);
   LCD.WriteRC("Y Position:",3,0);
   LCD.WriteRC("   Heading:",4,0);

   while (true) {
       LCD.WriteRC(RPS.X(),2,12); //update the x coordinate
       LCD.WriteRC(RPS.Y(),3,12); //update the y coordinate
       LCD.WriteRC(RPS.Heading(),4,12); //update the heading

       Sleep(10); //wait for a 10ms to avoid updating the screen too quickly
   }
    //we will never get here because of the infinite while loop
}

// TODO: Update these declarations
//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P0_1);
DigitalEncoder left_encoder(FEHIO::P0_0);
FEHMotor right_motor(FEHMotor::Motor1, 9.0);
FEHMotor left_motor(FEHMotor::Motor0, 9.0);

void move_forward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_right(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_left(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void check_x_plus(float x_coordinate) //using RPS while robot is in the +x direction
{
    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)
    {
        if(RPS.X() > x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            //<ADD CODE HERE>
        }
        else if(RPS.X() < x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            //<ADD CODE HERE>
        }
    }
}

void check_y_minus(float y_coordinate) //using RPS while robot is in the -y direction
{
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        if(RPS.Y() > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            //<ADD CODE HERE>
        }
        else if(RPS.Y() < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            //<ADD CODE HERE>
        }
    }
}

void check_y_plus(float y_coordinate) //using RPS while robot is in the +y direction
{
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        if(RPS.Y() > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            //<ADD CODE HERE>
        }
        else if(RPS.Y() < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            //<ADD CODE HERE>
        }
    }
}

void check_heading(float heading) //using RPS
{
    //you will need to fill out this one yourself and take into account
    //the edge conditions (when you want the robot to go to 0 degrees
    //or close to 0 degrees)
}

// TODO: Turn into helper function
int main(void)
{
    float touch_x,touch_y;

    RPS.InitializeTouchMenu();

    LCD.WriteLine("RPS & Data Logging Test");
    LCD.WriteLine("Press Screen To Start");
    while(!LCD.Touch(&touch_x,&touch_y)); //Wait for touchscreen press

    //STUDENT CODE HERE

    return 0;
}



/*
 * Main function: Calls whatever other function the robot is to run.
 */
// MAIN FUNCTION
int main(void)
{
    // When using servos: Consider calling servo.TouchCalibrate(); if this is the first run with those servos
    //calibrateServo();

    // Call desired function
    printRPSLocation();

    // Just a conventional best practice
    return 0;
}
