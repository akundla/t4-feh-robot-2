/*
 * time-drive-functions.cpp
 * TIME DRIVE FUNCTIONS: Drives the robot forward or turns it in place for a given number of seconds.
 * */

#ifndef TIMEDRIVEFUNCTIONS_H
#define TIMEDRIVEFUNCTIONS_H

/**
 *  @brief driveForSeconds: Drives the robot in a straight line. Can drive the
 *  robot either skids first or wheels first, for any duration of time
 *  and motor power percentage.
 *
 * @param bool skidFirst: Controls whether the robot drives skid first or wheels-first.
 * @param double seconds: The number of seconds for which the robot will drive.
 * @param int motorPowerPercent: The percentage at which the robot will drive.
 * */
void driveForSeconds(bool skidFirst, double seconds, int motorPowerPercent);

/**
 * @brief turnInPlace: Rotates the robot a certain number of degrees in place
 * @param turnClockwise: Boolean whether the robot should turn clockwise or counterclockwise
 * @param seconds: The number of seconds for which the robot should turn
 * @param motorPowerPercent: The percentage at which the motors should be driven
 */
void turnInPlace(bool turnClockwise, double seconds, int motorPowerPercent);

// Turns the robot about 180 degrees clockwise at the top of the ramp
void turnAround();

// Turns the robot about 45 degrees counterclockwise in the starting box
void turnOnStart();

#endif // TIMEDRIVEFUNCTIONS_H
