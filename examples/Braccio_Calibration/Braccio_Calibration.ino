/*
 Braccio_Calibration.ino - version 0.2
 Written by Lukas Severinghaus
 Calibration program for the Braccio Arm. To use, upload the program
 Open the serial monitor with baud rate 9600 and no line endings
 and follow the prompts. Copy the final values to your program
 to use the calibration parameters.

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

#include "BraccioV2.h"
Braccio arm;



//Set these values from the min/max values below.
#define GRIPPER_CLOSED 85
#define GRIPPER_OPENED 20

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing... Please Wait");//Start of initialization, see note below regarding begin method.

  //Set these trim values to get each joint exactly straight, this is the default starting position.
  arm.setJointCenter(WRIST_ROT, 90);//Wrist rotation joint should be parallel to rest of the arm
  arm.setJointCenter(WRIST, 90);
  arm.setJointCenter(ELBOW, 90);
  arm.setJointCenter(SHOULDER, 90);
  arm.setJointCenter(BASE_ROT, 90);//Base should be pointing at 0° mark on base platform
  arm.setJointCenter(GRIPPER, 50);//Rough center of gripper, default opening position

  //Set max/min values for joints as needed. Default is min: 0, max: 180
  //The only two joints that should need this set are gripper and shoulder.
  arm.setJointMax(GRIPPER, 100);//Gripper closed, can go further, but risks damage to servos
  arm.setJointMin(GRIPPER, 15);//Gripper open, can't open further

  //There are two ways to start the arm:
  //1. Start to default position.
  arm.begin(true);// Start to default vertical position.
  //This method moves the arm to the values specified by setJointCenter
  //and by default will make the arm be roughly straight up.

  //2. Start to custom position.
  //arm.begin(false);
  //arm.setAllNow(base_rot_val, shoulder_val, elbow_val, wrist_val, wrist_rot_val, gripper_val);
  //This method allows a custom start position to be set, but the setAllNow method MUST be run
  //immediately after the begin method and before any other movement commands are issued.


  //NOTE: The begin method takes approximately 8 seconds to start, due to the time required
  //to initialize the power circuitry.
  Serial.println("Initialization Complete, Begin Calibration"); //All setup is done, the arm is ready to move.
  Serial.println("Available Commands: 'c' change position, 's' set position");
  calibrateJoint(BASE_ROT, "BASE_ROT", arm);
  calibrateJoint(SHOULDER, "SHOULDER", arm);
  calibrateJoint(ELBOW, "ELBOW", arm);
  calibrateJoint(WRIST, "WRIST", arm);
  calibrateJoint(WRIST_ROT, "WRIST_ROT", arm);
  calibrateJoint(GRIPPER, "GRIPPER", arm);
  Serial.println("Calibration done, copy values into your program to use");
}

void loop() {

}
#define UPDATE_TIME 1000
void calibrateJoint(int joint, String joint_name, Braccio _arm){
  Serial.print("Now calibrating: ");
  Serial.println(joint_name);
  int centered_pos, min_pos, max_pos, currentPosition;
  if(joint != GRIPPER){
    Serial.println("Calibrating joint center");
    _arm.setOneAbsolute(joint, 90);
    _arm.safeDelay(UPDATE_TIME);
    bool centered = false;
    currentPosition = 90;

    while(!centered){
      Serial.print("Current position: ");
      Serial.println(currentPosition);
      Serial.println("Use c### to change the position, and s to save the position");
      while(Serial.available() < 1);
      char c = Serial.read();
      if(c == 'c'){
        currentPosition = Serial.parseInt();
        _arm.setOneAbsolute(joint, currentPosition);
        for(int i = 0; i < 50; i++){_arm.update();}
      }else if(c == 's'){
        Serial.print("Joint value: "); Serial.print(currentPosition); Serial.println(" saved. Moving to next joint");
        centered = true;
      }else{
        Serial.print("Unrecognized input, please try again");
      }
    }
    centered_pos = currentPosition;


    Serial.println("Calibrating joint minimum");
    _arm.setOneAbsolute(joint, 0);
    _arm.safeDelay(UPDATE_TIME);
    currentPosition = 0;
    bool minSet = false;
    while(!minSet){
      Serial.print("Current position: ");
      Serial.println(currentPosition);
      Serial.println("Use c### to change the position, and s to save the position");
      while(Serial.available() < 1);
      char c = Serial.read();
      if(c == 'c'){
        currentPosition = Serial.parseInt();
        _arm.setOneAbsolute(joint, currentPosition);
        for(int i = 0; i < 50; i++){_arm.update();}
      }else if(c == 's'){
        Serial.print("Joint value: "); Serial.print(currentPosition); Serial.println(" saved. Moving to next joint");
        minSet = true;
      }else{
        Serial.print("Unrecognized input, please try again");
      }
    }
    min_pos = currentPosition;


    Serial.println("Calibrating joint maximum");
    _arm.setOneAbsolute(joint, 180);
    _arm.safeDelay(UPDATE_TIME);
    _arm.safeDelay(UPDATE_TIME);
    currentPosition = 180;
    bool maxSet = false;
    while(!maxSet){
      Serial.print("Current position: ");
      Serial.println(currentPosition);
      Serial.println("Use c### to change the position, and s to save the position");
      while(Serial.available() < 1);
      char c = Serial.read();
      if(c == 'c'){
        currentPosition = Serial.parseInt();
        _arm.setOneAbsolute(joint, currentPosition);
        for(int i = 0; i < 50; i++){_arm.update();}
      }else if(c == 's'){
        Serial.print("Joint value: "); Serial.print(currentPosition); Serial.println(" saved. Moving to next joint");
        maxSet = true;
      }else{
        Serial.print("Unrecognized input, please try again");
      }
    }
    max_pos = currentPosition;

    Serial.println("Begin commands:");
    Serial.print("/***** Begin "); Serial.print(joint_name); Serial.println(" Configuration *****/");
    Serial.print("arm.setJointCenter("); Serial.print(joint_name); Serial.print(", "); Serial.print(centered_pos); Serial.println(");");
    Serial.print("arm.setJointMin("); Serial.print(joint_name); Serial.print(", "); Serial.print(min_pos); Serial.println(");");
    Serial.print("arm.setJointMax("); Serial.print(joint_name); Serial.print(", "); Serial.print(max_pos); Serial.println(");");
    Serial.print("/***** End "); Serial.print(joint_name); Serial.println(" Configuration *****/");
    _arm.setOneAbsolute(joint, centered_pos);
    _arm.safeDelay(UPDATE_TIME);
  } else {
    //Gripper Calibration
    Serial.println("Calibrating gripper center");
    _arm.setOneAbsolute(joint, 40);
    _arm.safeDelay(UPDATE_TIME);
    bool centered = false;
    currentPosition = 40;

    while(!centered){
      Serial.print("Current position: ");
      Serial.println(currentPosition);
      Serial.println("Use c### to change the position, and s to save the position");
      while(Serial.available() < 1);
      char c = Serial.read();
      if(c == 'c'){
        currentPosition = Serial.parseInt();
        _arm.setOneAbsolute(joint, currentPosition);
        for(int i = 0; i < 50; i++){_arm.update();}
      }else if(c == 's'){
        Serial.print("Joint value: "); Serial.print(currentPosition); Serial.println(" saved. Moving to next joint");
        centered = true;
      }else{
        Serial.print("Unrecognized input, please try again");
      }
    }
    centered_pos = currentPosition;


    Serial.println("Calibrating joint minimum");
    _arm.setOneAbsolute(joint, 15);
    _arm.safeDelay(UPDATE_TIME);
    currentPosition = 15;
    bool minSet = false;
    while(!minSet){
      Serial.print("Current position: ");
      Serial.println(currentPosition);
      Serial.println("Use c### to change the position, and s to save the position");
      while(Serial.available() < 1);
      char c = Serial.read();
      if(c == 'c'){
        currentPosition = Serial.parseInt();
        _arm.setOneAbsolute(joint, currentPosition);
        for(int i = 0; i < 50; i++){_arm.update();}
      }else if(c == 's'){
        Serial.print("Joint value: "); Serial.print(currentPosition); Serial.println(" saved. Moving to next joint");
        minSet = true;
      }else{
        Serial.print("Unrecognized input, please try again");
      }
    }
    min_pos = currentPosition;


    Serial.println("Calibrating joint maximum");
    _arm.setOneAbsolute(joint, 80);
    _arm.safeDelay(UPDATE_TIME);
    currentPosition = 80;
    bool maxSet = false;
    while(!maxSet){
      Serial.print("Current position: ");
      Serial.println(currentPosition);
      Serial.println("Use c### to change the position, and s to save the position");
      while(Serial.available() < 1);
      char c = Serial.read();
      if(c == 'c'){
        currentPosition = Serial.parseInt();
        _arm.setOneAbsolute(joint, currentPosition);
        for(int i = 0; i < 50; i++){_arm.update();}
      }else if(c == 's'){
        Serial.print("Joint value: "); Serial.print(currentPosition); Serial.println(" saved. Moving to next joint");
        maxSet = true;
      }else{
        Serial.print("Unrecognized input, please try again");
      }
    }
    max_pos = currentPosition;

    Serial.println("Begin commands:");
    Serial.print("/***** Begin "); Serial.print(joint_name); Serial.println(" Configuration *****/");
    Serial.print("arm.setJointCenter("); Serial.print(joint_name); Serial.print(", "); Serial.print(centered_pos); Serial.println(");");
    Serial.print("arm.setJointMin("); Serial.print(joint_name); Serial.print(", "); Serial.print(min_pos); Serial.println(");");
    Serial.print("arm.setJointMax("); Serial.print(joint_name); Serial.print(", "); Serial.print(max_pos); Serial.println(");");
    Serial.print("/***** End "); Serial.print(joint_name); Serial.println(" Configuration *****/");
  }
}
