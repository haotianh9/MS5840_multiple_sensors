#ifndef MS5840_I2C
#define MS5840_I2C

#include <Wire.h>
// Define measurement type.
enum measurement
{
	PRESSURE = 0x00,
	TEMPERATURE = 0x10
};

// Define constants for Conversion precision
enum precision
{
	ADC_256  = 0x00,
	ADC_512  = 0x02,
	ADC_1024 = 0x04,
	ADC_2048 = 0x06,
	ADC_4096 = 0x08,
	ADC_8192 = 0x0A
};
#define CMD_RESET 0x1E // reset command
#define CMD_ADC_READ 0x00 // ADC read command
#define CMD_ADC_CONV 0x40 // ADC conversion command
#define CMD_PROM 0xA0 // Coefficient location

class MS5840 {

public:
    MS5840(int);
    uint8_t reset();
    uint8_t read_coefficient();
    void sendADCconversion(measurement, precision );
    uint32_t getADCconversion(measurement);
    void process_data();
    int32_t get_temprature();
    int32_t get_pressure();
    uint16_t * get_coefficient();
private:
    uint8_t sendCommand(uint8_t);
    uint16_t coefficient[8];// Coefficients;
    int address;
    uint32_t D1, D2;
    int32_t _temperature_actual;
	int32_t _pressure_actual;
};


#endif