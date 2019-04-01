/*
 * other-functions.h
 * OTHER FUNCTIONS HEADRER: Catch-all for functions that don't fit anywhere else.
 * */

#ifndef OTHERFUNCTIONS_H
#define OTHERFUNCTIONS_H

// Calibrates the servos
void calibrateServos();

// Prompts and waits for the user to touch the screen
void waitForTouch();

// Sets the servos to the correct initial position
void setupServos();

// Prepares the robot to run on the course
void prepareToLaunch();

// Moves the arm to drop the coin into the slot
void dropCoin();

// Moves the arm back and forth to ensure it functions correctly
void testArm();

#endif // OTHERFUNCTIONS_H
