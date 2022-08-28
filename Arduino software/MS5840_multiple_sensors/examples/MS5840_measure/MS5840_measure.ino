#include <MS5840.h>
int n_sensor=8;

MS5840 sensor_array[]={MS5840(0x76),MS5840(0x76),MS5840(0x76),MS5840(0x76),MS5840(0x76),MS5840(0x76),MS5840(0x76),MS5840(0x76)};
//MS5840 sensor_array[]={MS5840(0x76)};

//Create variables to store results

// Select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  uint8_t i2c_status=Wire.endTransmission();
//  Serial.print("TCA9548A status: ");
//  Serial.println(i2c_status);
}


void setup() {
  Wire.begin();
  Serial.begin(250000);
  Serial.setTimeout(9);
  Serial.println("begin");
  for (int i =0; i < n_sensor;i++){
    TCA9548A(i);
//    sensor_array[i].reset();
//  uint8_t sensor_status=sensor_array[i].reset();
//    if (!sensor_status){
////      Serial.print("Could not find a valid sensor at bus ");
////      Serial.println(i);
//    }

    sensor_array[i].read_coefficient();
    uint16_t* coefficient;
    coefficient=sensor_array[i].get_coefficient();
    Serial.print("sensor ");
    Serial.print(i);
    Serial.print(" coefficient: ");
    for (int j =0; j < 8;j++){
      Serial.print(coefficient[j]);
      Serial.print(" ");
    }
    Serial.println(" ");
//    delay(100);
  }
//  Serial.println("finish reset sensors");

}

void loop() {
    for (int i =0; i < n_sensor;i++){
        TCA9548A(i);
        sensor_array[i].sendADCconversion(PRESSURE,ADC_8192);
    }
    delay(12);
    for (int i =0; i < n_sensor;i++){
        TCA9548A(i);
        uint32_t D1=sensor_array[i].getADCconversion(PRESSURE);
        char str[19];
        sprintf(str,"%d p %ld",i,D1);
        Serial.println(str);
 
    }
    for (int i =0; i < n_sensor;i++){
        TCA9548A(i);
        sensor_array[i].sendADCconversion(TEMPERATURE,ADC_8192);
    }
    delay(12);
    for (int i =0; i < n_sensor;i++){
        TCA9548A(i);
        uint32_t D2=sensor_array[i].getADCconversion(TEMPERATURE);
        char str[19];
        sprintf(str,"%d t %ld",i,D2);
        Serial.println(str);
    }
//    for (int i =0; i < n_sensor;i++){
//      sensor_array[i].process_data();
//      Serial.print("sensor ");
//      Serial.print(i);
//      Serial.print(" pressure: ");
//      Serial.print(sensor_array[i].get_pressure());
//      Serial.print(" temperature: ");
//      Serial.println(sensor_array[i].get_temprature());
//    }
//    delay(100);
}
