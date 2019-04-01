/*
 * time-drive-functions.cpp
 * TIME DRIVE FUNCTIONS: Drives the robot forward or turns it in place for a given number of seconds.
 * */

#include "common.h"

/**
 *  @brief driveForSeconds: Drives the robot in a straight line. Can drive the
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
        leftMotorPowerPercent = motorPowerPercent + (motorPowerPercent * LEFT_MOTOR_OFFSET);
        rightMotorPowerPercent = -motorPowerPercent;
    }
    // Drives wheels first
    else {
        leftMotorPowerPercent = -(motorPowerPercent + (motorPowerPercent * LEFT_MOTOR_OFFSET));
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

/**
 * @brief turnInPlace: Rotates the robot a certain number of degrees in place
 * @param turnClockwise: Boolean whether the robot should turn clockwise or counterclockwise
 * @param seconds: The number of seconds for which the robot should turn
 * @param motorPowerPercent: The percentage at which the motors should be driven
 */
void turnInPlace(bool turnClockwise, double seconds, int motorPowerPercent) {
    LCD.WriteLine("Turning around");

    // Left and Right motor must be driven at different percentages
    double leftMotorPowerPercent = 0.0;
    double rightMotorPowerPercent = 0.0;

    // Turns clockwise
    if (turnClockwise) {
        leftMotorPowerPercent = -(motorPowerPercent + (motorPowerPercent * LEFT_MOTOR_OFFSET));
        rightMotorPowerPercent = -motorPowerPercent;
    }
    // Turns counterclockwise
    else {
        leftMotorPowerPercent = motorPowerPercent + (motorPowerPercent * LEFT_MOTOR_OFFSET);
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

// PREDEFINED COMBINATION FUNCTIONS for Performance test 1
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
