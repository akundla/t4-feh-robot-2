/*
 * cds-cell-functions.cpp
 * CDS CELL FUNCTIONS
 * All functionality that has to do with the CdS cell and its light detection
 *
 * */
#include "common.h"

// Determines whether the CdS cell sees a certain light color
bool detectLight(double lightVoltage) {
    return (cdsCell.Value() < lightVoltage + MoE && cdsCell.Value() > lightVoltage - MoE);
}

// Runs (burns time, makes robot wait) while the cdsCell detects light that is not in the voltage range of Red
void waitForStartLight() {
    float startTime = TimeNow();
    while ((cdsCell.Value() > RED_LIGHT_RED_F_V_AVG + MoE || cdsCell.Value() < RED_LIGHT_RED_F_V_AVG - MoE) && (TimeNow() - startTime) < 20.0) {
        LCD.WriteLine(cdsCell.Value());
    }
}

// Self-explanatory function for data gathering
void printCDsCellValues() {
    // Runs continuously
    while(true) {
        //Print the value of the CdS cell to the screen.
        LCD.WriteLine("CDs Cell: ");
        LCD.Write(cdsCell.Value());
        LCD.Write(" V");

        Sleep(1.0);
    }
}

// Runs continuously and prints to the screen if the CDS cell detects blue light: A requirement for Performance test 2
void detectBlueLight() {

    // Acceptable margin of error (+/-) in voltage value (determines window in which light can be detected)
    double LightV_MoE = 0.01;

    // If the voltage is within the MoE of blue light, prints to the screen that the light is blue
    if (cdsCell.Value() < BLUE_LIGHT_NO_FILTER_V_AVG + LightV_MoE && cdsCell.Value() > BLUE_LIGHT_NO_FILTER_V_AVG - LightV_MoE) {
        LCD.WriteLine("Blue Light detected");
    } else {
        LCD.WriteLine("Blue Light not detected");
    }
}

// NOTE: This is only used in the exploration
// Turns a servo motor in response to the reading from a CDS light cell
void moveServoToLight() {
    // Runs continuously
    while(true) {
        //Print the value of the CdS cell to the screen.
        LCD.Write("Voltage from CDS cell: ");
        LCD.Write(cdsCell.Value());
        LCD.Write("Volts");

        // Set the servo arm depending on the amount of light from the CDS cell
        // 0 light corresponds to 0 degress and full light to 180 degrees
        lower_servo.SetDegree( (NO_LIGHT_V - cdsCell.Value() ) * RATIO_SERVO_DEGREE_TO_CDS_CELL);
    }
}
