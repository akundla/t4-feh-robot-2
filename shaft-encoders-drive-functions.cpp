#include "common.h"

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

            int leftEncoderLast = left_encoder.Counts();
            int rightEncoderLast = right_encoder.Counts();

            LCD.Clear();

            LCD.Write("--> Left Encoder: ");
            LCD.WriteLine(left_encoder.Counts());
            LCD.Write("<-- Right Encoder: ");
            LCD.WriteLine(right_encoder.Counts());
        }
    }
}

void driveForInches(bool skidFirst, double inches, int motorPowerPercent) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();



    LCD.WriteLine("Driving for ");
    LCD.Write(inches);
    LCD.Write(" inches");

    // Left and Right motor must be driven at different percentages
    double leftMotorPowerPercent = 0.0;
    double rightMotorPowerPercent = 0.0;

    // Drives skids first
    if (skidFirst) {
        leftMotorPowerPercent = motorPowerPercent + LEFT_MOTOR_OFFSET;
        rightMotorPowerPercent = -motorPowerPercent;
    }
    // Drives wheels first
    else {
        leftMotorPowerPercent = -(motorPowerPercent + LEFT_MOTOR_OFFSET);
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

        // The numebr of ticks that the left wheel has traveled more than the right wheel
        int leftTicksDiff = left_encoder.Counts() - right_encoder.Counts();

        double rightMotorPowerAdjustment = (leftTicksDiff / TICKS_PER_REV) * rightMotorPowerPercent;

        rightMotor.SetPercent(rightMotorPowerPercent + rightMotorPowerAdjustment);
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
        leftMotorPowerPercent = -(motorPowerPercent + LEFT_MOTOR_OFFSET);
        rightMotorPowerPercent = -motorPowerPercent;
    }
    // Turns counterclockwise
    else {
        leftMotorPowerPercent = motorPowerPercent + LEFT_MOTOR_OFFSET;
        rightMotorPowerPercent = motorPowerPercent;
    }

    // Sets the motors in motion
    leftMotor.SetPercent(leftMotorPowerPercent);
    rightMotor.SetPercent(rightMotorPowerPercent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    // Stops both motors
    leftMotor.Stop();
    rightMotor.Stop();
}
