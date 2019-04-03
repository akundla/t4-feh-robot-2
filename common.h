/*
 * common.h
 * FULL PROGRAM HEADER FILE: Defines all constants and declares global variables for hardware
 * */

#ifndef COMMON_H
#define COMMON_H

// INCLUDE FEH LIBRARIES
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHRPS.h>
#include <FEHMotor.h>
#include <FEHServo.h>


// DECLARE GLOBAL CONSTANTS

// MOTOR CONSTANTS
// Power offset multiplied by the motor power and added to the left motor power to make the robot drive straight
#define LEFT_MOTOR_OFFSET 0.025
// Standard percentage of motor power for driving forwards or backwards
#define DRIVE_POWER 45.0//original is 42
// Standard percentage of motor power for turning the robot in place
#define TURN_POWER 23.0 //original 20.0
// Standard percentage for motor checking
#define RPS_POWER 15.0
#define RPS_SLEEP_SECONDS 0.5

// SERVOS
// Calibration values for lower servo
#define LOWER_SERVO_MIN 769  //original value was 769
#define LOWER_SERVO_MAX 2480  //orignal value was 2480

// Calibration values for upper servo
#define UPPER_SERVO_MIN 670
#define UPPER_SERVO_MAX 2370

// Directions for dropping coin
#define LOWER_DEGREE_STRAIGHT_OUT 170
#define UPPER_DEGREE_VERTICAL_DOWN 60


// DRIVE DIRECTION CONSTANTS
#define SKID_FIRST true
#define WHEELS_FIRST false

#define CLOCKWISE true
#define COUNTER_CLOCKWISE false


// SHAFT ENCODING CONSTANTS
#define TICKS_PER_REV 48
#define WHEEL_RADIUS 1.75
#define WHEEL_CIRCUMFERENCE 9.62112750162
#define INCHES_PER_TICK 0.20044
#define TICKS_PER_INCH 4.989025692


// RPS HEADING-CHECKING CONSTANTS
// The number of seconds the robot will attempt to calibrate for using RPS before giving up
#define SECONDS_TIMEOUT 8.0
// The number of degrees the robot can be off by on either side and still proceed forward
#define DEGREE_TOLERANCE 4

// Used for checking heading
#define SKIDS_COURSE_RIGHT 0
#define SKIDS_COURSE_TOP 90
#define SKIDS_COURSE_LEFT 180
#define SKIDS_COURSE_BOTTOM 270

#define WHEELS_COURSE_RIGHT 180
#define WHEELS_COURSE_TOP 270
#define WHEELS_COURSE_LEFT 0
#define WHEELS_COURSE_BOTTOM 90


// LIGHT CONSTANTS
// This margin is wider than necessary for easy detection on clear-contrast backgrounds
#define MoE 0.500

// Voltage reading from CDS cell when cell is covered (no light, pitch black)
#define NO_LIGHT_V 3.400

// CDS Cell average Voltage readings
#define BLUE_LIGHT_NO_FILTER_V_AVG 0.524
#define RED_LIGHT_NO_FILTER_V_AVG 0.188
#define BLUE_BACKLGROUND_V_AVG 3.200

// CDS Cell average Voltage with red filters
#define BLANK_STARTER_RED_F_V_AVG 1.745
#define RED_LIGHT_RED_F_V_AVG 0.313
#define BLUE_LIGHT_RED_F_V_AVG 0.895

// Constants from exploration 1
// Left optosensor
#define LEFT_RED 1.250
#define LEFT_LIGHT_BACKGROUND 1.635
#define LEFT_DARK_BACKGROUND 1.691
#define LEFT_BLACK 2.293

// Middle optosensor
#define MID_RED 1.440
#define MID_LIGHT_BACKGROUND 1.626
#define MID_DARK_BACKGROUND 1.685
#define MID_BLACK 2.358

// Right optosensor
#define RIGHT_RED 1.650
#define RIGHT_LIGHT_BACKGROUND 1.912
#define RIGHT_DARK_BACKGROUND 2.028
#define RIGHT_BLACK 2.434

// Average starting RPS X and Y coordinates from 208
#define AVG_START_X 8.925
#define AVG_START_Y 11.575

#define Y_TOP_OF_SHORT_RAMP 46.0
#define X_COIN_SLOT 18.15

#define Y_BEFORE_LEVER 44.1

#define Y_TOP_ACRYLIC_RAMP 52.0

#define X_LEFT_LIGHT 20.75
#define X_RIGHT_LIGHT 26.0
#define Y_DDR_LIGHTS 13.5

// OTHER
// Ratio that a CDS cell reading is multiplied by to create a degree (0 - 180) to which to move the servo
// CDS cell has an approximate voltage range of 0 - 3.400 volts. Servo has range of 0 - 180 degrees. 180 / 3.400 = 52.94
#define RATIO_SERVO_DEGREE_TO_CDS_CELL 52.94


// Name external global variables so they can be recognized
extern AnalogInputPin cdsCell;

extern DigitalEncoder left_encoder;
extern DigitalEncoder right_encoder;

extern FEHMotor leftMotor;
extern FEHMotor rightMotor;

extern FEHServo lower_servo;
extern FEHServo upper_servo;

extern DigitalInputPin skidServoCornerBump;
extern DigitalInputPin skidNonServoCornerBump;

// RPS Coordinates of the starting position
extern float startX;
extern float startY;

extern float yTopOfShortRamp;
extern float xCoinSlot;
extern float yBeforeLever;
extern float yTopAcrylicRamp;
extern float xLeftLight;
extern float xRightLight;
extern float yDDRLights;

#endif // COMMON_H
