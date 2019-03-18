#include "common.h"
#include "shaft-encoders-drive-functions.h"

// Prints the RPS coordinates
void printRPSLocation() {
    float touch_x, touch_y;

    //Call this function to initialize the RPS to a course
    RPS.InitializeTouchMenu();

    //Wait for touchscreen to be pressed
    LCD.WriteLine("Press Screen to Start");
    while(!LCD.Touch(&touch_x, &touch_y));

    LCD.Clear();

   //Write initial screen info
   LCD.WriteRC("RPS Test Program",0,0);
   LCD.WriteRC("X Position:",2,0);
   LCD.WriteRC("Y Position:",3,0);
   LCD.WriteRC("   Heading:",4,0);

   while (true) {
       LCD.WriteRC(RPS.X(),2,12); //update the x coordinate
       LCD.WriteRC(RPS.Y(),3,12); //update the y coordinate
       LCD.WriteRC(RPS.Heading(),4,12); //update the heading

       Sleep(10); //wait for a 10ms to avoid updating the screen too quickly
   }
    //we will never get here because of the infinite while loop
}

void check_x_plus(float x_coordinate) //using RPS while robot is in the +x direction
{
    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1 && RPS.Heading() != -1 && RPS.Heading() != -2)
    {
        // Skids are pointing Left
        if (RPS.Heading() > 90 && RPS.Heading() < 270) {
            if(RPS.X() > x_coordinate)
            {
                //pulse the motors for one tick in the correct direction
                driveForInches(SKID_FIRST, INCHES_PER_TICK, RPS_POWER);
            }
            else if(RPS.X() < x_coordinate)
            {
                //pulse the motors for a short duration in the correct direction
                driveForInches(WHEELS_FIRST, INCHES_PER_TICK, RPS_POWER);
            }
        }
        // else the skids are pointing right
        else {
            if(RPS.X() > x_coordinate)
            {
                //pulse the motors for one tick in the correct direction
                driveForInches(WHEELS_FIRST, INCHES_PER_TICK, RPS_POWER);
            }
            else if(RPS.X() < x_coordinate)
            {
                //pulse the motors for a short duration in the correct direction
                driveForInches(SKID_FIRST, INCHES_PER_TICK, RPS_POWER);
            }
        }
    }
}


void check_y_plus(float y_coordinate) //using RPS while robot is in the +y direction
{
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1 && RPS.Heading() != -1 && RPS.Heading() != -2)
    {
        // Skids are pointing up
        if (RPS.Heading() > 0 && RPS.Heading() < 180) {
            if(RPS.Y() > y_coordinate)
            {
                //pulse the motors for one tick in the correct direction
                driveForInches(WHEELS_FIRST, INCHES_PER_TICK, RPS_POWER);
            }
            else if(RPS.Y() < y_coordinate)
            {
                //pulse the motors for a short duration in the correct direction
                driveForInches(SKID_FIRST, INCHES_PER_TICK, RPS_POWER);
            }
        }
        // else the skids are pointing down
        else {
            if(RPS.Y() > y_coordinate)
            {
                //pulse the motors for one tick in the correct direction
                driveForInches(SKID_FIRST, INCHES_PER_TICK, RPS_POWER);
            }
            else if(RPS.Y() < y_coordinate)
            {
                //pulse the motors for a short duration in the correct direction
                driveForInches(WHEELS_FIRST, INCHES_PER_TICK, RPS_POWER);
            }
        }
    }
}


bool RPSIsWorking () {

    if (RPS.Heading() != -1 && RPS.Heading() != -2) {
        return true;
    } else if (RPS.Heading() == -1) {
        LCD.WriteLine("RPS RETURNED -1");
        return false;
    } else {
        LCD.WriteLine("RPS RETURNED -2");
        return false;
    }

}

void check_heading(float heading) //using RPS
{
    // The current timeout
    float timeout = TimeNow();

    // While the robot is reading a heading on the course and that heading is not the correct heading
    while (RPS.Heading() > (heading + DEGREE_TOLERANCE) || RPS.Heading() < (heading - DEGREE_TOLERANCE) && RPSIsWorking() && (TimeNow() - timeout) < SECONDS_TIMEOUT)
    {
        if(RPS.Heading() > heading) {
            // If we should turn clockwise, do that
           if ((RPS.Heading() - heading) <= 180) {
               turnCountsInPlace(CLOCKWISE, 1, RPS_POWER);
           }
           // Else we should go counterclockwise
           else {
               turnCountsInPlace(COUNTER_CLOCKWISE, 1, RPS_POWER);
           }
        } else if(RPS.Heading() < heading) {
            // If we should turn counterclockwise, do that
           if ((heading - RPS.Heading()) <= 180) {
               turnCountsInPlace(COUNTER_CLOCKWISE, 1, RPS_POWER);
           }
           // Else we should go counterclockwise
           else {
               turnCountsInPlace(CLOCKWISE, 1, RPS_POWER);
           }
        }
        Sleep(0.7);
    }
}
