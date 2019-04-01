/*
 * pid-drive-functions.cpp
 * PID DRIVE FUNCTIONS: Uses Proportional, Integral, Derivative to try to make the robot drive straight using Calculus on
 * shaft encoder values.
 * NOTE: This implementation of PID does not make the team's robot drive straight.
 * It makes the robot drive to the right when driven skid-first. Changing the P I and D constants
 * doesn't seem to make the robot go straight.
 * */

#ifndef PIDDRIVEFUNCTIONS_H
#define PIDDRIVEFUNCTIONS_H

// Attempts to drive the robot straight using the PID function.
void driveForInchesPID(bool skidFirst, double inches, float inchesPerSecond);

#endif // PIDDRIVEFUNCTIONS_H
