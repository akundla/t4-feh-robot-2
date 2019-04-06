/*
 * rps-functions.cpp
 * RPS FUNCTIONS: Adjusts the robot into specific RPS coordinates.
 * */

#ifndef RPSFUNCTIONS_H
#define RPSFUNCTIONS_H

// Prints the RPS coordinates of the robot
void printRPSLocation();

// Moves the robot into a specified x-coordinate
void check_x_plus(float x_coordinate);

// Moves the robot to a specific y-coordinate
void check_y_plus(float y_coordinate);

// Moves the robot to a specific y-coordinate with .25 in either direction
void check_y_plus_TIGHT(float y_coordinate);

// Helper function that determines whether the robot is in the dead zone or otherwise non-functioning
bool RPSIsWorking ();

// Moves the robot into a specific heading.
void check_heading(float heading);

#endif // RPSFUNCTIONS_H
