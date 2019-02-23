// INCLUDE LIBRARIES
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>

//Include the FEHMotor library
#include <FEHMotor.h>
//Include the FEHServo library
#include <FEHServo.h>

// DECLARE GLOBAL CONSTANTS

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
    leftMotor.SetPercent(left_motor_percent);
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

// TODO: Recalibrate offset for new motor setup
// Uses functions written above to navigate the Exploration 1 course
void navigateExploration1Course() {

    double OFFSET_TO_DRIVE_STRAIGHT = 1.5;
    int QUARTER_POWER_PERCENT = 25;

    DriveForwardUntilHitWall(QUARTER_POWER_PERCENT, QUARTER_POWER_PERCENT - OFFSET_TO_DRIVE_STRAIGHT);
    BackLeftTurnUntilHitWall();
    DriveForwardUntilHitWall(QUARTER_POWER_PERCENT, QUARTER_POWER_PERCENT - OFFSET_TO_DRIVE_STRAIGHT);
    BackRightTurnUntilCornerHitWall();
    DriveForwardUntilHitWall(QUARTER_POWER_PERCENT, QUARTER_POWER_PERCENT - OFFSET_TO_DRIVE_STRAIGHT);
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

// Drives the robot back from the lever to the start
void DriveBackLeverToStart (int left_motor_percent, int right_motor_percent) {

    //Turn both motors on at given percent motor power.
    leftMotor.SetPercent(left_motor_percent);
    rightMotor.SetPercent(right_motor_percent);

    // TODO: Input shaft encoding here or use bump switches

    // Stops motors
    leftMotor.Stop();
    rightMotor.Stop();
}

void driveTime(double seconds, int power) {
    LCD.WriteLine("Driving forward");


    // 4.125
    double offset = 1.0;

    leftMotor.SetPercent(power + offset);
    rightMotor.SetPercent(-power);

    Sleep(seconds);

    leftMotor.Stop();
    rightMotor.Stop();

}

void driveBackwardsTime(double seconds, int power) {
    LCD.WriteLine("Driving forward");

    double offset = +3.75;

    leftMotor.SetPercent(power + offset);
    rightMotor.SetPercent(-power);

    Sleep(seconds);

    leftMotor.Stop();
    rightMotor.Stop();

}

void turnInCircle(double seconds, int power) {
    LCD.WriteLine("Turning around");

    double offset = -1.0;

    leftMotor.SetPercent(-power + offset);
    rightMotor.SetPercent(-power);

    Sleep(seconds);

    leftMotor.Stop();
    rightMotor.Stop();
}

void turnAround() {
    // 180 was about 3.700 seconds at 20 percent
    turnInCircle(3.300, 21);
}

void turnOnStart() {
    // 180 was about 3.700 seconds at 20 percent
    turnInCircle(2.900 / 4.0, -22);
}

// Navigates from the starting box to the lever, flips the lever, then drives back
void performanceTestOne () {

    float x, y;

    double powerPercent = 39;

    servo.SetDegree(120);

    LCD.WriteLine("Performance test 1");
    LCD.WriteLine("Set the arm to the correct position you lazy piece of shit");
    LCD.WriteLine("Then touch the screen to continue");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    waitForStartLight();

    turnOnStart();

    driveTime(5.6, powerPercent);
    turnAround();
    // TODO: Consider shortening this time
    driveTime(1.34, -powerPercent);

    Sleep(0.5);

    servo.SetDegree(0);
    Sleep(0.5);

    driveBackwardsTime(6.0, powerPercent);

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

// MAIN FUNCTION
int main(void)
{
    // When using servos: Consider calling servo.TouchCalibrate(); if this is the first run with those servos
    calibrateServo();

    // Call desired function
    performanceTestOne();


    // Just a conventional best practice
    return 0;
}

//test edit by Connor
