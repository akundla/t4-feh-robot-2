/*
 * shaft-encoders-drive-functions.cpp
 * SHAFT ENCODING DRIVING FUNCTIONS: Drives the robot forward or turns it in place by distance.
 * */

#include "common.h"

/**
 * @brief printShaftEncoderValues: Prints out the encoder counts for testing purposes
 */
void printShaftEncoderValues () {
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    LCD.WriteLine("Encoder Counts: ");

    int leftEncoderLast = left_encoder.Counts();
    int rightEncoderLast = right_encoder.Counts();

    LCD.Write("--> Left Encoder: ");
    LCD.WriteLine(left_encoder.Counts());
    LCD.Write("<-- Right Encoder: ");
    LCD.WriteLine(right_encoder.Counts());

    while (true) {
        if ((left_encoder.Counts() != leftEncoderLast) || (right_encoder.Counts() != rightEncoderLast)) {

            LCD.Clear();

            LCD.Write("--> Left Encoder: ");
            LCD.WriteLine(left_encoder.Counts());
            LCD.Write("<-- Right Encoder: ");
            LCD.WriteLine(right_encoder.Counts());
        }
    }
}

/**
 * @brief driveForInches: Drives the robot for a specific distance using shaft encoders
 * @param skidFirst: Whether the robot will be driven skid first or wheels first
 * @param inches: The distance to drive in inches
 * @param motorPowerPercent: The level of power at which the motors should be driven
 * @param leftMotorOffset: The percentage added or subtracted to the power from the left motor
 */
void driveForInches(bool skidFirst, double inches, int motorPowerPercent, float leftMotorOffset) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    int lastLeftCounts = 0;
    int lastRightCounts = 0;
    float timeBetweenStuckChecks = 0.25;
    float lastTime = TimeNow();


    LCD.WriteLine("Driving for ");
    LCD.Write(inches);
    LCD.Write(" inches");

    // Left and Right motor must be driven at different percentages
    double leftMotorPowerPercent = 0.0;
    double rightMotorPowerPercent = 0.0;

    // Drives skids first
    if (skidFirst) {
        leftMotorPowerPercent = motorPowerPercent + (motorPowerPercent * leftMotorOffset);
        rightMotorPowerPercent = -motorPowerPercent;
    }
    // Drives wheels first
    else {
        leftMotorPowerPercent = -(motorPowerPercent + (motorPowerPercent * leftMotorOffset));
        rightMotorPowerPercent = motorPowerPercent;
    }

    // Sets the motors in motion
    leftMotor.SetPercent(leftMotorPowerPercent);
    rightMotor.SetPercent(rightMotorPowerPercent);

    double counts = inches * TICKS_PER_INCH;

    // Runs motors while the average of the left and right encoder is less than counts,
    //keep running motors
    float timeout = TimeNow();
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts && ((TimeNow() - timeout) < SECONDS_TIMEOUT)) {
        LCD.WriteLine("L: ");
        LCD.Write(left_encoder.Counts());
        LCD.WriteLine("R: ");
        LCD.WriteLine(right_encoder.Counts());
        LCD.WriteLine("");

        // Checks if the wheels are stuck every half second and ramps up power if they are
        if ((TimeNow() - lastTime) > timeBetweenStuckChecks) {

            // If the left wheel is stuck
            if (lastLeftCounts == left_encoder.Counts()) {
                leftMotor.SetPercent(100);
            } else {
                leftMotor.SetPercent(leftMotorPowerPercent);
            }

            // If the right wheel is stuck
            if (lastLeftCounts == left_encoder.Counts()) {
                leftMotor.SetPercent(100);
            } else {
                rightMotor.SetPercent(rightMotorPowerPercent);
            }


            // Saves time, counts
            lastTime = TimeNow();
            lastLeftCounts = left_encoder.Counts();
            lastRightCounts - right_encoder.Counts();
        } else {
            // If wheels aren't stuck, performs normal power adjustment on right motor

            // The numebr of ticks that the left wheel has traveled more than the right wheel
            int leftTicksDiff = left_encoder.Counts() - right_encoder.Counts();

            double rightMotorPowerAdjustment = (leftTicksDiff / TICKS_PER_REV) * rightMotorPowerPercent;

            rightMotor.SetPercent(rightMotorPowerPercent + rightMotorPowerAdjustment);
        }
    }

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();

    LCD.Clear();

    LCD.Write("Drove for ");
    LCD.Write(inches);
    LCD.WriteLine(" inches");

    LCD.WriteLine("Final Encoder Counts: ");
    LCD.Write("<-- Left Encoder: ");
    LCD.WriteLine(left_encoder.Counts());
    LCD.Write("--> Right Encoder: ");
    LCD.WriteLine(right_encoder.Counts());
}

/**
 * @brief turnCountsInPlace: Rotates the robot a certain number of degrees in place
 * @param turnClockwise: Boolean whether the robot should turn clockwise or counterclockwise
 * @param counts: The number of encoder counts (per encoder) that the robot should turn
 * @param motorPowerPercent: The percentage at which the motors should be driven
 */
void turnCountsInPlace(bool turnClockwise, int counts, int motorPowerPercent) {

    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    LCD.WriteLine("Turning in place with counts");

    // Left and Right motor must be driven at different percentages
    double leftMotorPowerPercent = 0.0;
    double rightMotorPowerPercent = 0.0;

    // Turns clockwise
    if (turnClockwise) {
        leftMotorPowerPercent = -(motorPowerPercent + (motorPowerPercent * LEFT_MOTOR_OFFSET));
        rightMotorPowerPercent = -motorPowerPercent;
    }
    // Turns counterclockwise
    else {
        leftMotorPowerPercent = motorPowerPercent + (motorPowerPercent * LEFT_MOTOR_OFFSET);
        rightMotorPowerPercent = motorPowerPercent;
    }

    // Sets the motors in motion
    leftMotor.SetPercent(leftMotorPowerPercent);
    rightMotor.SetPercent(rightMotorPowerPercent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    float timeout = TimeNow();
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts && ((TimeNow() - timeout) < SECONDS_TIMEOUT));

    // Stops both motors
    leftMotor.Stop();
    rightMotor.Stop();
}
