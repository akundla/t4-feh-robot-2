/*
 * bump-switch-functions.cpp
 * BUMP SWITCH DRIVE FUNCTIONS
 * Drive the robot into walls usubg bump switches: Used for CrayolaBots and for the team's robot
 * */

#include "common.h"

// Drives the robot skid first into a wall, then squares the robot with that wall
void DriveSkidFirstUntilHitWall(int motorPowerPercent)
{
    //Turn both motors on at given percent motor power.
    // Left and Right motor must be driven at different percentages
    // Robot will drive skids first
    leftMotor.SetPercent(motorPowerPercent + (motorPowerPercent * LEFT_MOTOR_OFFSET));
    rightMotor.SetPercent(-motorPowerPercent);

    // Psuedo-infinite loop to burn time while both switches are not pressed.
    // Note that bump switches are "true" when not pressed and "false" when pressed
    float timeout = TimeNow();
    while (skidServoCornerBump.Value() == 1 || skidNonServoCornerBump.Value() == 1  && ((TimeNow() - timeout) < SECONDS_TIMEOUT)) {
        LCD.WriteLine("Driving with Bump Switches");

        if (skidServoCornerBump.Value() == 0)  {
            rightMotor.Stop();
        }

        if (skidNonServoCornerBump.Value() == 0)  {
            leftMotor.Stop();
        }
    }

    // Stops motors
    leftMotor.Stop();
    rightMotor.Stop();

}

// NOTE: This was only useful on the CrayolaBots, not for the team's robot.
// Function to reverse the robot to the left until a wall is hit
void BackLeftTurnUntilHitWall()
{
    // Set both motors to a low power level
    leftMotor.SetPercent(-10);
    rightMotor.SetPercent(-25);

    // Psuedo-infinite loop to burn time while both switches are not pressed.
    // Note that bump switches are "true" when not pressed and "false" when pressed
    while (skidServoCornerBump.Value() == 1 || skidNonServoCornerBump.Value() == 1) {}

    // Stops motors
    leftMotor.Stop();
    rightMotor.Stop();
}

// NOTE: This was only useful on the CrayolaBots, not for the team's robot.
// Function to reverse the robot back to the right until one switch hits a wall
void BackRightTurnUntilCornerHitWall()
{
    // Set both motors to a low power level
    leftMotor.SetPercent(-50);
    rightMotor.SetPercent(-10);

    // Psuedo-infinite loop to burn time while one switch is not pressed.
    // Note that bump switches are "true" when not pressed and "false" when pressed
    while (skidServoCornerBump.Value() == 1 && skidNonServoCornerBump.Value() == 1) { }

    // Stops left motor
    leftMotor.Stop();

    // Drives the right motor forward to make the robot square in the course
    rightMotor.SetPercent(25);

    // Allows the right motor to run for 2 tenths of a second
    Sleep(0.2);

    // Stops right motor
    rightMotor.Stop();
}
