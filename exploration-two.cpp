#include "common.h"
#include "shaft-encoders-drive-functions.h"

void explorationTwoShaftEncoders () {

    // Garbage variables for touch screen
    float x, y;

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Shaft Encoder Exploration Test");
    LCD.WriteLine("Touch the screen to begin");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    LCD.WriteLine("1. Starting driving");

    // Move forward one revolution
    driveForInches(WHEELS_FIRST, WHEEL_CIRCUMFERENCE, DRIVE_POWER);

    LCD.WriteLine("2. Driving complete, turning");

    // Call turn right function
    turnCountsInPlace(CLOCKWISE, TICKS_PER_REV / 2.0, TURN_POWER);

    LCD.WriteLine("3. Turning complete, driving");

    // Move forward one revolution
    driveForInches(WHEELS_FIRST, WHEEL_CIRCUMFERENCE, DRIVE_POWER);

    LCD.WriteLine("4. Driving complete, turning");

    // call turn left function
    turnCountsInPlace(COUNTER_CLOCKWISE, TICKS_PER_REV / 2.0, TURN_POWER);

    LCD.WriteLine("5. Turning complete, driving");

    // Move forward one revolution
    driveForInches(WHEELS_FIRST, WHEEL_CIRCUMFERENCE, DRIVE_POWER);

    LCD.WriteLine("6. Driving complete, Done. ");
}
