/*
 * cds-cell-functions.h
 * CDS CELL FUNCTIONS HEADER
 * All functionality that has to do with the CdS cell and its light detection
 *
 * */

#ifndef CDSCELLFUNCTIONS_H
#define CDSCELLFUNCTIONS_H

// Self-explanatory function for data gathering
void printCDsCellValues();

// Runs continuously and prints to the screen if the CDS cell detects blue light: A requirement for Performance test 2
void detectBlueLight();

// Determines whether the CdS cell sees a certain light color
bool detectLight(double lightVoltage);

// Runs (burns time, makes robot wait) while the cdsCell detects light that is not in the voltage range of Red
void waitForStartLight();

// NOTE: This is only used in the exploration
// Turns a servo motor in response to the reading from a CDS light cell
void moveServoToLight();

#endif // CDSCELLFUNCTIONS_H
