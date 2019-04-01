/*
 * line-following-functions.h
 * LINE FOLLOWING FUNCTIONS HEADER: Makes the CrayolaBot in Exploration 2 follow lines.
 * NOTE: This is ONLY for CrayolaBots in Exploration 2. The Optosensors declared are never even initialized
 * */

#ifndef LINEFOLLOWERFUNCTIONS_H
#define LINEFOLLOWERFUNCTIONS_H

// Prints the values of the optosensors
void lineFollowerPrintValues();

// Navigates the robot along the black line
void FollowBlackLine();

// Makes the robot drive along the red line in Exploration 2
void FollowRedLine();

#endif // LINEFOLLOWERFUNCTIONS_H
