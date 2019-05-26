# BraccioV2: An updated library for the Tinkerkit Braccio arm
This library provides additional software functionality to the Tinkerkit Braccio arm.

# Getting Started
First, install the library. Download the repository as a zip file and unpack into your ```Arduino/libraries/``` folder.
Second, calibrate the arm using the ```calibration.ino``` file in the ```examples/``` directory.
Third, try out the other examples.

# Features
* **Support for individual joint control.** The library allows moving one joint without needing to command the other joints.
* **Allows both absolute and relative joint control.** The library allows the programmer to move any joint to either an absolute position, or to a position relative to the current joint location.
* **Ability to disable or customize startup position.** The library can either move to a default location on startup(the arm will point straight up), move to a customized location, or move to the first position in the program.
* **Joints can be calibrated.** The library allows setting the center, minimum, and maximum point for each joint, allowing the user to correct for minor calibration issues from assembly.
* **Controllable speed.** By using the ```setDelta``` method, the speed of each joint can be independently controlled.
* **Non-blocking control.** The library uses an update method that can be called at any point within the program. This method takes very little time to run and does not wait for the completion of the movements, allowing the program to process other input during the time the arm is moving.
* **Safe delay functionality.** Using the library's ```safeDelay``` method, the program can delay while still continuing to update the arm.

# Source
This library was built using parts of the Braccio library released by Arduino(https://github.com/arduino-org/arduino-library-braccio), which was originally licensed under the GNU GPL V1.2 License.

# Reference
These sources provide some more background information on the Braccio arm and original software:
https://github.com/arduino-org/arduino-library-braccio

http://www.arduino.org/learning/reference/Braccio

# License
This library is licensed under the GNU GPL V3 license. Please see the license file for more information.
