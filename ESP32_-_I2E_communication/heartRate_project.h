#ifndef heartRate_header_h
#define heartRate_header_h

#include "BluetoothSerial.h"

#include <Wire.h>

class HeartRate{
  public:

  BluetoothSerial SerialBT;
  
  int x0, x1, x2, x3; // variables for read data
  uint16_t ledOn = 0; // used to store LED on in data
  uint16_t ledOff = 0; // used to store LED off data

  const int NUMBER_OF_MEASUREMENTS = 80;
  int data_holder[80];
  
  int i =0;
  int peak_start_index = 0;
  int peak_finish_index = 0;
  int counter = 0;
  int peak_counter = 0;
 
  uint16_t biggest_value = 0;
  uint16_t second_biggest_value = 0;
  uint16_t change_of_the_biggest_value = 0;
  uint16_t change_of_the_second_biggest_value = 0;
  uint16_t array_data = 0;
  uint16_t final_result = 0;
  uint16_t mean_hr = 0;
  
  float final_result_smooth = 0;
  float time_between_peaks = 0;
  float current_heart_rate_data = 0;
  float previous_heart_rate_data = 0;

  bool two_peaks_detected = false;
  
  void peak_to_peak_detection(uint16_t filtered_value);  
  
  void system_initialize(int baud_rate, int clock_speed);
  void set_control_one_register(int osc_block, int led_freq, int data_read_freq);
  void set_control_two_register(int led_driver_mode, int led_emitting_time, int led_current);
  void start_measurement();
  void read_collected_data();

  private:
};

#endif heartRate_header_h
