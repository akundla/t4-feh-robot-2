/*
 * shaft-encoders-drive-functions.h
 * SHAFT ENCODING DRIVING FUNCTIONS HEADER: Drives the robot forward or turns it in place by distance.
 * */

#ifndef SHAFTENCODERSDRIVEFUNCTIONS_H
#define SHAFTENCODERSDRIVEFUNCTIONS_H

/**
 * @brief printShaftEncoderValues: Prints out the encoder counts for testing purposes
 */
void printShaftEncoderValues ();

/**
 * @brief driveForInches: Drives the robot for a specific distance using shaft encoders
 * @param skidFirst: Whether the robot will be driven skid first or wheels first
 * @param inches: The distance to drive in inches
 * @param motorPowerPercent: The level of power at which the motors should be driven
 * @param leftMotorOffset: The percentage added or subtracted to the power from the left motor
 */
void driveForInches(bool skidFirst, double inches, int motorPowerPercent, float leftMotorOffset);

/**
 * @brief turnCountsInPlace: Rotates the robot a certain number of degrees in place
 * @param turnClockwise: Boolean whether the robot should turn clockwise or counterclockwise
 * @param counts: The number of encoder counts (per encoder) that the robot should turn
 * @param motorPowerPercent: The percentage at which the motors should be driven
 */
void turnCountsInPlace(bool turnClockwise, int counts, int motorPowerPercent);

#endif // SHAFTENCODERSDRIVEFUNCTIONS_H
