#ifndef TIMEDRIVEFUNCTIONS_H
#define TIMEDRIVEFUNCTIONS_H

void driveForSeconds(bool skidFirst, double seconds, int motorPowerPercent);

void turnInPlace(bool turnClockwise, double seconds, int motorPowerPercent);

void turnAround();

void turnOnStart();

#endif // TIMEDRIVEFUNCTIONS_H
