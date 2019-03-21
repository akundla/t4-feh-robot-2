#include "common.h"

//Create a function to drive motors forward until microswitch is pressed.
void DriveSkidFirstUntilHitWall(int motorPowerPercent)
{
    //Turn both motors on at given percent motor power.
    // Left and Right motor must be driven at different percentages
    // Robot will drive skids first
    leftMotor.SetPercent(motorPowerPercent + LEFT_MOTOR_OFFSET);
    rightMotor.SetPercent(-motorPowerPercent);

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
