/*
 BraccioV2.h - version 0.1
 Written by Lukas Severinghaus
 Based upon the Braccio library by Andrea Martino and Angelo Ferrante
 originally licensed under GNU GPL V1.2.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef BRACCIOV2_H_
#define BRACCIOV2_H_

#include <Arduino.h>
#include <Servo.h>

#define SOFT_START_VALUE 0

#define SOFT_START_PIN 12

#define BASE_ROT 0 //Base rotation joint
#define SHOULDER 1 //Shoulder joint
#define ELBOW 2 //Elbow joint
#define WRIST 3 //Wrist joint
#define WRIST_ROT 4 //Wrist rotation joint
#define GRIPPER 5 //Gripper joint

//Don't change these.
#define _BASE_ROT_PIN 11
#define _SHOULDER_PIN 10
#define _ELBOW_PIN 9
#define _WRIST_PIN 6
#define _WRIST_ROT_PIN 5
#define _GRIPPER_PIN 3

//Max/min values for all servos globally
#define GLOBAL_MIN 0
#define GLOBAL_MAX 180

class Braccio {
  public:
    Braccio();
    void begin();
    void begin(bool defaultPos);
    bool setAllAbsolute(int b, int s, int e, int w, int w_r, int g);
    bool setAllRelative(int b, int s, int e, int w, int w_r, int g);
    bool setOneAbsolute(int joint, int value);
    bool setOneRelative(int joint, int value);
    void setJointMax(int joint, int value);
    void setJointMin(int joint, int value);
    void setJointCenter(int joint, int offset);
    void setDelta(int joint, int value);
    void setAllNow(int b, int s, int e, int w, int w_r, int g);
    void update();
    void safeDelay(int ms);
    void safeDelay(int ms, int t);
    int getCenter(int joint);
  private:
    void _softStart();
    void _softwarePWM(int high, int low);
    void _initializeServos(bool defaultPos);
    void _setServo(int joint, int value, bool updateTarget);
    void _moveServo(int joint);
    Servo _base;
    Servo _shoulder;
    Servo _elbow;
    Servo _wrist_rot;
    Servo _wrist;
    Servo _gripper;
    int _jointMax[7] = {180, 165, 180, 180, 180, 73};
    int _jointMin[7] = {0, 15, 0, 0, 0, 10};
    int _jointCenter[7] = {90, 90, 90, 90, 90, 50};
    int _jointDelta[7] = {1, 1, 1, 1, 1, 1};
    int _currentJointPositions[7];
    int _targetJointPositions[7];
};

#endif
