/*
 * performance-test-one.h
 * PERFORMANCE TEST ONE HEADER: Navigates the robot up the steep ramp and flips the lever, relying entirely on timing.
 * */

#ifndef PERFORMANCETESTONE_H
#define PERFORMANCETESTONE_H

/**
 * @brief performanceTestOne: Navigates through the course to complete the first performance test.
 * This function first moves the servo motor into the 120 degree position,
 * at which point it prompts the user to rotate the arm to the correct
 * position so that when the servo is set to 0 degrees, the arm will hit
 * the lever. Once the user presses the screen, it calls a function to
 * wait for the red light to come on. When it detects the red light,
 * the function calls a turn function to rotate
 * the robot 45 degrees counerclockwise to align it with the ramp. It
 * then calls the driveForSeconds function to
 * drive the robot from the starting box up the ramp. At the top of the
 * ramp, it calls the turn function to turn the robot around nearly 180
 * degrees, then calls the drive function again to drive the robot forward
 * the last few inches. The function then sets the servo arm's position
 * back to 0 to flip the lever and calls the drive function to drive back
 * down the ramp to the starting box.
 */
void performanceTestOne ();

#endif // PERFORMANCETESTONE_H
