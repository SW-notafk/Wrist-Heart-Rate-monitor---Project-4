#include "heartRate_project.h"


#define reset 0x40
#define meas_control_one 0x41
#define meas_control_two 0x42
#define meas_start 0x43
#define optical_sensor 0x5B
#define data_address_one 0x54
#define data_address_two 0x55
#define data_address_three 0x56
#define data_address_four 0x57

void HeartRate::system_initialize(int baud_rate, int clock_speed){
  Wire.begin();
  delay(10);
  Serial.begin(baud_rate);
  delay(10);
  SerialBT.begin("ESP32testing");
  Serial.println("Connecting to ESP32testing");
  delay(10);
  Wire.setClock(clock_speed);
  delay(10);
}

void HeartRate::set_control_one_register(int osc_block, int led_freq, int data_read_freq){
  if (osc_block == 0){
    osc_block = 0b00000000;
  } else if (osc_block == 1){
    osc_block = 0b10000000;
  }
  if (led_freq == 128){
    led_freq = 0b00000000;
  } else if (led_freq == 64) {
    led_freq = 0b00000100;
  }
  if (data_read_freq == 64){
    data_read_freq = 0b00000001;
  } else if (data_read_freq == 32) {
    data_read_freq = 0b00000010;
  }
  uint8_t final_setting_control_one = osc_block | led_freq | data_read_freq;

  Wire.beginTransmission(optical_sensor); 
  Wire.write(meas_control_one); // address number that you want to write to
  Wire.write(final_setting_control_one); // settings for that address
  Wire.endTransmission();
}

void HeartRate::set_control_two_register(int led_driver_mode, int led_emitting_time, int led_current){
  if (led_driver_mode == 0){ //corresponds to 00
    led_driver_mode = 0b00000000;
  } else if (led_driver_mode == 1){ // coresponds to 01
    led_driver_mode = 0b01000000;
  } else if (led_driver_mode == 10){
    led_driver_mode = 0b10000000;
  } else if (led_driver_mode == 11){
    led_driver_mode = 0b11000000;
  }

  if (led_emitting_time == 0.3){
    led_emitting_time = 0b00000000;
  } else if (led_emitting_time == 0.6){
    led_emitting_time = 0b00100000;
  }

  if (led_current == 1){
    led_current = 0b00001000;
  } else if (led_current == 2){
    led_current = 0b00001001;
  } else if (led_current == 3){
    led_current = 0b00001010;
  } else if (led_current == 6){
    led_current = 0b00001011;
  } else if (led_current == 10){
    led_current = 0b00001100;
  } else if (led_current == 20){
    led_current = 0b00001101;
  } else if (led_current == 30){
    led_current = 0b00001110;
  } else if (led_current == 60){
    led_current = 0b00001111;
  }
  
  uint8_t final_setting_control_two = led_driver_mode | led_emitting_time | led_current;

  Wire.beginTransmission(optical_sensor);
  Wire.write(meas_control_two);
  Wire.write(final_setting_control_two);
  Wire.endTransmission();
}

void HeartRate::start_measurement(){
  Wire.beginTransmission(optical_sensor);
  Wire.write(meas_start);
  Wire.write(1);
  Wire.endTransmission();
}

void HeartRate::read_collected_data(){
  Wire.beginTransmission(optical_sensor);
  Wire.write(data_address_one);
  Wire.write(data_address_two);
  Wire.write(data_address_three);
  Wire.write(data_address_four);
  Wire.endTransmission();
  
  Wire.requestFrom(optical_sensor, 4);
  
  while (Wire.available() < 4); //wait until there are 4 bytes of available space
  
  x0 = Wire.read();
  x1 = Wire.read();
  x2 = Wire.read();
  x3 = Wire.read();   

  //converting raw data into 2 byte int
  ledOn = (x3 << 8) | x2; //shifting the upper byte by 8 to make space for the lower byte (giving me the 16bit number)
  ledOff = (x1 << 8) | x0;
   
  //calculating final result
  final_result = (ledOn - ledOff);

  final_result_smooth = 0.1 * (float)final_result + (1 - 0.1) * final_result_smooth;//low pass filter

  peak_to_peak_detection((uint16_t)final_result_smooth);
  
  //Serial.println((uint16_t)final_result_smooth);

  delay(20);
}

void HeartRate::peak_to_peak_detection(uint16_t filtered_data){
  data_holder[counter] = filtered_data;
  
  if (counter == NUMBER_OF_MEASUREMENTS){
    
  for (i = 0; i < NUMBER_OF_MEASUREMENTS; i++){ 
    array_data = data_holder[i];
    Serial.println(array_data);

    if (i >= 1 && i <= 77){ //this was added
    if ((data_holder[i] > data_holder[i+1] && (data_holder[i] > data_holder[i-1]) && (data_holder[i] != data_holder[0]) && (data_holder[i] != data_holder[-1])) || ((data_holder[i] == data_holder[i + 1]) && (data_holder[i] > data_holder[i-1]) && (data_holder[i] > data_holder[i + 2]) && (data_holder[i] != data_holder[0]) && (data_holder[i] != data_holder[-1]))){      
      peak_counter++;
      if ((array_data > biggest_value)){
        second_biggest_value = biggest_value;
        biggest_value = array_data;
      }
      else if((array_data > second_biggest_value) && (array_data != biggest_value)){
        second_biggest_value = array_data;
        peak_finish_index = i;           
      }
      if ((second_biggest_value != change_of_the_second_biggest_value) && (second_biggest_value != array_data)){
        peak_finish_index = peak_start_index;            
      }        
      if (biggest_value != change_of_the_biggest_value){
        peak_start_index = i;    
      }  
      change_of_the_biggest_value = biggest_value;
      change_of_the_second_biggest_value = second_biggest_value;
      
      if (peak_counter == 1){    
       data_holder[0] = array_data;
       //i = 1;
       counter = 0; 
       break; 
      }
    }
  }
  } // this was added
  
  if (peak_counter > 1){
    previous_heart_rate_data = current_heart_rate_data;
      
    time_between_peaks = abs((peak_finish_index - peak_start_index) * 0.02);
    current_heart_rate_data = 60 / time_between_peaks;
  
    if (previous_heart_rate_data == 0 || previous_heart_rate_data < 30){
      previous_heart_rate_data = current_heart_rate_data;
    }
  
    int difference_hr = abs(previous_heart_rate_data - current_heart_rate_data);
  
    if (current_heart_rate_data > 100){
      current_heart_rate_data = previous_heart_rate_data;
    }
    if (difference_hr > 7){
      current_heart_rate_data = previous_heart_rate_data;
    }
    
    Serial.print("Peak finish index: ");
    Serial.println(peak_finish_index);
    Serial.print("Peak start index: ");
    Serial.println(peak_start_index);
    Serial.print("Amount of measurements taken between peaks: ");
    Serial.println(abs(peak_finish_index - peak_start_index));

    Serial.print("First peak: ");
    Serial.println(biggest_value);
    Serial.print("Second peak: ");
    Serial.println(second_biggest_value);
    Serial.print("Time between peaks: ");
    Serial.println(time_between_peaks);
    Serial.print("Difference between current and previous data: ");
    Serial.println(difference_hr);
    Serial.print("User's heart rate: ");
    Serial.println(current_heart_rate_data);
  
    
    SerialBT.println((int)current_heart_rate_data);
    
    biggest_value = 0;
    second_biggest_value = 0;
    change_of_the_second_biggest_value = 0;
    array_data = 0;
    time_between_peaks = 0;
    peak_start_index = 0;
    peak_finish_index = 0;
    counter = 0;
    peak_counter = 0;
  }
  }
counter++;
}
