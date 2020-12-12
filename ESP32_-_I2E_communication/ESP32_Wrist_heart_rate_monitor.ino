#include "heartRate_project.h"

HeartRate heartRate;

void setup() {
  //set the baud rate and clock speed
  heartRate.system_initialize(115200, 4000000);
  //set the OSC (active or not), led emitting frequency, data reading frequecny
  heartRate.set_control_one_register(1, 128, 32);
  //set led driver mode, led emitting time, led driver current
  heartRate.set_control_two_register(0, 0.3, 20);
  //start the measurement
  heartRate.start_measurement();
      
  delay(10);
}

void loop() {
  heartRate.read_collected_data();
}


/* Initialize can have:
    clock speed up to 400k
*/


/* First address can be set to:
    OSC can be 0 or 1
    LED emitting frequency can be: 128 or 64
    Data reading frequency can be: 64 or 32
*/

/* Second address can be set to:
   LED driver mode: 0, 1, 10, 11
   LED emitting time: 0.3, 0.6 (ms)
   LED driver current: 1, 2, 3, 6 ,10, 20, 30, 60 (mA)
*/
