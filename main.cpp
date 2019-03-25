// INCLUDE CUSTOM LIBRARIES
#include "common.h"
#include "time-drive-functions.h"
#include "bump-switch-functions.h"
#include "line-follower-functions.h"
#include "shaft-encoders-drive-functions.h"
#include "rps-functions.h"
#include "cds-cell-functions.h"
#include "pid-drive-functions.h"

// INCLUDE EXPLORATION CODE
#include "exploration-one.h"
#include "exploration-two.h"

// INCLUDE PERFORMANCE TEST CODE
#include "performance-test-one.h"
#include "performance-test-two.h"
#include "performance-test-three.h"
#include "performance-test-four.h"
#include "full-course.h"

// SENSORS
//Declare a CdS Cell sensor as an analog input and assign it to an IO port
AnalogInputPin cdsCell (FEHIO::P0_0);

// ENCODERS
DigitalEncoder left_encoder(FEHIO::P1_7);
DigitalEncoder right_encoder(FEHIO::P1_0);

// DRIVE MOTORS
//Assign the right and left motors to motor ports with a max voltage of 9.0V
FEHMotor leftMotor (FEHMotor :: Motor0, 9.0);
FEHMotor rightMotor (FEHMotor :: Motor1, 9.0);

// SERVOS
//Declare a servo motor and assign it to a servo port
FEHServo lower_servo (FEHServo::Servo7);
FEHServo upper_servo (FEHServo::Servo0);

// BUMP SWITCHES
DigitalInputPin skidServoCornerBump (FEHIO::P3_7);
DigitalInputPin skidNonServoCornerBump (FEHIO::P3_0);

/*
 * Main function: Calls whatever other function the robot is to run.
 */
int main(void) {

//    double inchesToDrive = 60.0;
//    float inchesPerSecond = 12.0;
//    driveForInchesPID(SKID_FIRST, inchesToDrive, inchesPerSecond);

    // Call desired function
    navigateFullCourse();

    // Just a conventional best practice
    return 0;
}
