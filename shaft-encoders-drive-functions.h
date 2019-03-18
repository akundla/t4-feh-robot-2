#ifndef SHAFTENCODERSDRIVEFUNCTIONS_H
#define SHAFTENCODERSDRIVEFUNCTIONS_H

void printShaftEncoderValues ();

void driveForInches(bool skidFirst, double inches, int motorPowerPercent);

void turnCountsInPlace(bool turnClockwise, int counts, int motorPowerPercent);

#endif // SHAFTENCODERSDRIVEFUNCTIONS_H
