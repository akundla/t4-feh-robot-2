#include "common.h"
#include "FEHSD.h"

// In inches per second
#define STANDARD_DRIVE_SPEED 10.0

// About 0.75
#define P 0.90
// Less than 0.1
#define I 0.09
// About 0.25
#define D 0.40
// Between 0.1 and 0.2, prefer lower (probably)
#define TIME_TO_SLEEP 0.2

float leftMotorPower = 30;
float leftPastError = 0.0;
float leftErrorSum = 0.0;
float leftPastCounts = 0.0;
float leftPastTime = 0;

float rightMotorPower = 30;
float rightPastError = 0.0;
float rightErrorSum = 0.0;
float rightPastCounts = 0.0;
float rightPastTime = 0;

// The function used to Reset PID each time.
void resetPIDVariables() {
    leftMotorPower = 0;
    leftPastError = 0.0;
    leftErrorSum = 0.0;
    leftPastCounts = 0.0;
    leftPastTime = 0;

    rightMotorPower = 0;
    rightPastError = 0.0;
    rightErrorSum = 0.0;
    rightPastCounts = 0.0;
    rightPastTime = 0;

    left_encoder.ResetCounts();
    right_encoder.ResetCounts();

    // Wait for Small Time to Get Data (To avoid time difference of 0)
    Sleep(TIME_TO_SLEEP);
}

//The function used to do PID control for the left motor
float leftMotorPIDAdjustment(float expectedSpeed) {
    int changeInCounts = left_encoder.Counts() - leftPastCounts;
    float changeInTime = TimeNow() - leftPastTime;
    float actualSpeed = (changeInCounts * INCHES_PER_TICK) / changeInTime;
    float error = expectedSpeed - actualSpeed;
    leftErrorSum += error;

    SD.Printf("\n\nleft actual speed: ");
    SD.Printf("%f", actualSpeed);
    SD.Printf("\nleft error: ");
    SD.Printf("%f", error);
    SD.Printf("\nLeft change in Counts: ");
    SD.Printf("%d", changeInCounts);
    SD.Printf("\nLeft change in time: ");
    SD.Printf("%f", changeInTime);

    // Speeds the motor up or slows it down a little bit each time
    float pTerm = error * P; // Calculate PTerm
    /* Sets the motor power to deal with accumulated error: such as
     * the motor being stalled or one motor going off-course
     * (This make the robot drive STRAIGHT!!) */
    float iTerm = leftErrorSum * I; // Calculate ITerm
    // Makes bigger changes if the error is getting worse and smaller changes if the error is getting better.
    float dTerm = (error - leftPastError) * D; // Calculate DTerm

    // Update tracked values
    leftPastError = error;
    leftPastCounts = left_encoder.Counts();
    leftPastTime = TimeNow();

    SD.Printf("\nOld left motor power: ");
    SD.Printf("%f", leftMotorPower);
    SD.Printf("\nleft pTerm: ");
    SD.Printf("%f", pTerm);
    SD.Printf("\nleft iTerm: ");
    SD.Printf("%f", iTerm);
    SD.Printf("\nleft dTerm: ");
    SD.Printf("%f", dTerm);

    // Finally set the motorPower
    leftMotorPower = leftMotorPower + pTerm + iTerm + dTerm;

    SD.Printf("\nNew left motor power: ");
    SD.Printf("%f", leftMotorPower);

    return leftMotorPower;
}

//The function used to do PID control for the right motor
float rightMotorPIDAdjustment(float expectedSpeed) {
    int changeInCounts = right_encoder.Counts() - rightPastCounts;
    float changeInTime = TimeNow() - rightPastTime;
    float actualSpeed = (changeInCounts * INCHES_PER_TICK) / changeInTime;
    float error = expectedSpeed - actualSpeed;
    rightErrorSum += error;

    SD.Printf("\n\nright actual speed: ");
    SD.Printf("%f", actualSpeed);
    SD.Printf("\nright error: ");
    SD.Printf("%f", error);
    SD.Printf("\nLeft change in Counts: ");
    SD.Printf("%d", changeInCounts);
    SD.Printf("\nLeft change in time: ");
    SD.Printf("%f", changeInTime);

    // Speeds the motor up or slows it down a little bit each time
    float pTerm = error * P; // Calculate PTerm
    /* Sets the motor power to deal with accumulated error: such as
     * the motor being stalled or one motor going off-course
     * (This make the robot drive STRAIGHT!!) */
    float iTerm = rightErrorSum * I; // Calculate ITerm
    // Makes bigger changes if the error is getting worse and smaller changes if the error is getting better.
    float dTerm = (error - rightPastError) * D; // Calculate DTerm

    SD.Printf("\nOld right motor power: ");
    SD.Printf("%f", rightMotorPower);
    SD.Printf("\nright pTerm: ");
    SD.Printf("%f", pTerm);
    SD.Printf("\nright iTerm: ");
    SD.Printf("%f", iTerm);
    SD.Printf("\nright dTerm: ");
    SD.Printf("%f", dTerm);

    // Update tracked values
    rightPastError = error;
    rightPastCounts = right_encoder.Counts();
    rightPastTime = TimeNow();

    // Finally set the motorPower
    rightMotorPower = rightMotorPower + pTerm + iTerm + dTerm;

    SD.Printf("\nNew right motor power: ");
    SD.Printf("%f", rightMotorPower);

    return rightMotorPower;
}

// PID function for driving. Repeatedly calls
void driveForInchesPID(bool skidFirst, double inches, float inchesPerSecond) {

    resetPIDVariables();

    // Open file to log constants
    SD.OpenLog();

    //Be careful of potentially making your first time difference of 0, this can cause big errors!
    while ( (((left_encoder.Counts() + right_encoder.Counts()) / 2.0) * INCHES_PER_TICK) < inches) {

        // Drives skids first
        if (skidFirst) {
            leftMotor.SetPercent(leftMotorPIDAdjustment(inchesPerSecond));
            rightMotor.SetPercent(-rightMotorPIDAdjustment(inchesPerSecond));
        }
        // Drives wheels first
        else {
            leftMotor.SetPercent(-leftMotorPIDAdjustment(inchesPerSecond));
            rightMotor.SetPercent(rightMotorPIDAdjustment(inchesPerSecond));
        }
        LCD.WriteLine(leftMotorPower);
        LCD.WriteLine(rightMotorPower);


        // Wait for Small Time to Get Data (To avoid time difference of 0)
        Sleep(TIME_TO_SLEEP);
    }

    rightMotor.Stop();
    leftMotor.Stop();

    SD.CloseLog();
}
