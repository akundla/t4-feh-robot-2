/*
 * bump-switch-functions.h
 * BUMP SWITCH DRIVE FUNCTIONS HEADER
 * Drive the robot into walls usubg bump switches: Used for CrayolaBots and for the team's robot
 * */

#ifndef BUMPSWITCHFUNCTIONS_H
#define BUMPSWITCHFUNCTIONS_H

// Drives the robot skid first into a wall, then squares the robot with that wall
void DriveSkidFirstUntilHitWall(int motorPowerPercent);

// NOTE: These are used only by the CrayolaBot for Exploration 1

// Function to reverse the robot to the left until a wall is hit
void BackLeftTurnUntilHitWall();

// Function to reverse the robot back to the right until one switch hits a wall
void BackRightTurnUntilCornerHitWall();

#endif // BUMPSWITCHFUNCTIONS_H
