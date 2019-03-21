#include "common.h"

void lineFollowerPrintValues() {

//    float trashX, trashY;

//    while (true) {

//        LCD.WriteLine("Left optosensor voltages: ");
//        for (int i = 0; i < 5; i++) {
//            LCD.WriteLine(leftOptosensor.Value());
//        }

//        while (!LCD.Touch(&trashX, &trashY)) {}
//        while (LCD.Touch(&trashX, &trashY)) {}

//        LCD.WriteLine("Middle optosensor voltages: ");
//        for (int i = 0; i < 5; i++) {
//            LCD.WriteLine(middleOptosensor.Value());
//        }

//        while (!LCD.Touch(&trashX, &trashY)) {}
//        while (LCD.Touch(&trashX, &trashY)) {}

//        LCD.WriteLine("Right optosensor voltages: ");
//        for (int i = 0; i < 5; i++) {
//            LCD.WriteLine(rightOptosensor.Value());
//        }

//        while (!LCD.Touch(&trashX, &trashY)) {}
//        while (LCD.Touch(&trashX, &trashY)) {}
//    }
}

// Navigates the robot along the black line
void FollowBlackLine() {

//    // Robot navigation state
//    bool leftOfLine = false;
//    bool rightOfLine = false;
//    bool onLine = false;

//    float leftVal, midVal, rightVal;

//    while(true) {
//        // Take values
//        leftVal = leftOptosensor.Value();
//        midVal = middleOptosensor.Value();
//        rightVal = rightOptosensor.Value();

//        // if left sees line, position = right of line
//        if (leftVal > LEFT_BLACK - MoE && leftVal < LEFT_BLACK + MoE) {
//            leftOfLine = false;
//            rightOfLine = true;
//            onLine = false;
//        }

//        // if right sees line, position = left of line
//        else if (rightVal > RIGHT_BLACK - MoE && rightVal < RIGHT_BLACK + MoE) {
//            leftOfLine = true;
//            rightOfLine = false;
//            onLine = false;
//        }

//        // if middle sees line, position = on line
//        else if (midVal > MID_BLACK - MoE && midVal < MID_BLACK + MoE) {
//            leftOfLine = false;
//            rightOfLine = false;
//            onLine = true;
//        }

//        if (onLine) {

//            LCD.WriteLine("On line");

//            leftMotor.SetPercent(15);
//            rightMotor.SetPercent(15);
//        } else if (rightOfLine) {

//            LCD.WriteLine("RightOfLine");

//            leftMotor.SetPercent(10);
//            rightMotor.SetPercent(30);
//        } else if (leftOfLine) {

//            LCD.WriteLine("LeftOfLine");

//            leftMotor.SetPercent(30);
//            rightMotor.SetPercent(10);
//        }
//    }
}

// Makes the robot drive along the red line in Exploration 2
void FollowRedLine() {

//    bool leftOfLine = false;
//    bool rightOfLine = false;
//    bool onLine = false;

//    float leftVal, midVal, rightVal;

//    while(true) {
//        // Take values
//        leftVal = leftOptosensor.Value();
//        midVal = middleOptosensor.Value();
//        rightVal = rightOptosensor.Value();

//        // if left sees line, position = right of line
//        if (leftVal > LEFT_RED - MoE && leftVal < LEFT_RED + MoE) {
//            leftOfLine = false;
//            rightOfLine = true;
//            onLine = false;
//        }

//        // if right sees line, position = left of line
//        else if (rightVal > RIGHT_RED - MoE && rightVal < RIGHT_RED + MoE) {
//            leftOfLine = true;
//            rightOfLine = false;
//            onLine = false;
//        }

//        // if middle sees line, position = on line
//        else if (midVal > MID_RED - MoE && midVal < MID_RED + MoE) {
//            leftOfLine = false;
//            rightOfLine = false;
//            onLine = true;
//        }

//        if (onLine) {

//            LCD.WriteLine("On line");

//            leftMotor.SetPercent(25);
//            rightMotor.SetPercent(25);
//        } else if (rightOfLine) {

//            LCD.WriteLine("RightOfLine");

//            leftMotor.SetPercent(15);
//            rightMotor.SetPercent(25);
//        } else if (leftOfLine) {

//            LCD.WriteLine("LeftOfLine");

//            leftMotor.SetPercent(25);
//            rightMotor.SetPercent(15);
//        }
//    }
}
