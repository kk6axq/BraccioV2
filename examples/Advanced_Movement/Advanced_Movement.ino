#include "BraccioV2.h"
Braccio arm;
/*
 Advanced_Movement.ino - version 0.1
 Written by Lukas Severinghaus
 Demonstrates advanced movement of the arm utilizing whole arm addressing,
 in both absolute and relative positions, custom speeds, direct control,
 and manual updating.

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
//Set these values from the min/max gripper joint values below.
#define GRIPPER_CLOSED 85
#define GRIPPER_OPENED 20

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing... Please Wait");//Start of initialization, see note below regarding begin method.

  //Update these lines with the calibration code outputted by the calibration program.
  arm.setJointCenter(WRIST_ROT, 90);
  arm.setJointCenter(WRIST, 90);
  arm.setJointCenter(ELBOW, 90);
  arm.setJointCenter(SHOULDER, 90);
  arm.setJointCenter(BASE_ROT, 90);
  arm.setJointCenter(GRIPPER, 50);//Rough center of gripper, default opening position

  //Set max/min values for joints as needed. Default is min: 0, max: 180
  //The only two joints that should need this set are gripper and shoulder.
  arm.setJointMax(GRIPPER, 100);//Gripper closed, can go further, but risks damage to servos
  arm.setJointMin(GRIPPER, 15);//Gripper open, can't open further

  //There are two ways to start the arm:
  //1. Start to default position.
  //arm.begin(true);// Start to default vertical position.
  //This method moves the arm to the values specified by setJointCenter
  //and by default will make the arm be roughly straight up.

  //2. Start to custom position.
  arm.begin(false);
  //Set each joint immediately to a set position
  arm.setAllNow(90, 90, 45, 45, 90, 50);
  //This method allows a custom start position to be set, but the setAllNow method MUST be run
  //immediately after the begin method and before any other movement commands are issued.


  //NOTE: The begin method takes approximately 8 seconds to start, due to the time required
  //to initialize the power circuitry.
  Serial.println("Initialization Complete");

  //Set the base rotation joint to move at 3x speed. This may cause the arm to jerk.
  arm.setDelta(BASE_ROT, 3);
}

void loop() {
  //Begin loop
  //Set the arm to absolute positions on all joints
  arm.setAllAbsolute(45, 135, 135, 135, 90, 50);

  //Below is an example of running some other code while waiting for the arm to move.
  //Exit the while loop after 3000ms has passed
  int endTime = millis() + 3000;
  while(millis() < endTime){
    //Run some code
    Serial.println("Working while waiting for movement...");
    //Update the arm
    arm.update();
    //Delay a short time to prevent the updates from happening too close together.
    delay(10);
  }

  //Set all the arm joints to relative positions
  arm.setAllRelative(90, -90, -90, -90, 0, 0);
  //Delay for 3000ms while still updating movement, by default every 10ms
  arm.safeDelay(3000);

}

void openGripper(){
  //Set the gripper position to open
  arm.setOneAbsolute(GRIPPER, GRIPPER_OPENED);
}

void closeGripper(){
  //Set the gripper position to closed
  arm.setOneAbsolute(GRIPPER, GRIPPER_CLOSED);
}
