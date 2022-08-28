#include "MS5840.h"
#include "math.h"
/**
* \brief Class constructor
*
*/
MS5840::MS5840(int address) {this->address=address;}

uint8_t MS5840::reset(){
    uint8_t res= sendCommand(CMD_RESET);
	return res;
    // delayMicroseconds(9);
}

uint8_t MS5840::read_coefficient()
// Initialize library for subsequent pressure measurements
{
	uint8_t i;
	uint8_t highByte,lowByte;
	for(i = 0; i <= 7; i++)
  {
		sendCommand(CMD_PROM + (i * 2));
		Wire.requestFrom( this->address, 2);
		
		while(Wire.available()){
			highByte = Wire.read();
			lowByte = Wire.read();
		}
		
		this->coefficient[i] = (highByte << 8)|lowByte;
	// Uncomment below for debugging output.
	//	Serial.print("C");
	//	Serial.print(i);
	//	Serial.print("= ");
	//	Serial.println(coefficient[i]);
	}

	return 0;
}

void MS5840::sendADCconversion(measurement _measurement, precision _precision)
{
	sendCommand(CMD_ADC_CONV + _measurement + _precision);
}

uint32_t MS5840::getADCconversion(measurement _measurement)
{
	sendCommand(CMD_ADC_READ);
    Wire.requestFrom(this->address, 3);
	uint8_t highByte = 0, midByte = 0, lowByte = 0;
    while(Wire.available())
	{
		highByte = Wire.read();
		midByte = Wire.read();
		lowByte = Wire.read();
	}
    
	switch( _measurement )
	{
		case PRESSURE : this->D1= ((uint32_t)highByte << 16) + ((uint32_t)midByte << 8) + lowByte; return this->D1;
		case TEMPERATURE : this->D2= ((uint32_t)highByte << 16) + ((uint32_t)midByte << 8) + lowByte; return this->D2;
	}

	
}

uint8_t MS5840::sendCommand(uint8_t command)
{
	Wire.beginTransmission( this->address);
	Wire.write(command);
	uint8_t i2c_status=Wire.endTransmission();
	return i2c_status;
}



void MS5840::process_data()

{
	//Create Variables for calculations
	int32_t temp_calc;
	int32_t pressure_calc;

	int32_t dT;

	//Now that we have a raw temperature, let's compute our actual.
	dT = this->D2 - ((int32_t)coefficient[5] << 8);
	temp_calc = (((int64_t)dT * coefficient[6]) >> 23) + 2000;

	// TODO TESTING  _temperature_actual = temp_calc;

	//Now we have our first order Temperature, let's calculate the second order.
	int64_t TI, OFFI, SENSI, OFF, SENS, OFF2, SENS2; //working variables

	if (temp_calc > 2000)
	// If temp_calc is above 20.0C
	{
		TI = 0;
		OFFI = 0;
		SENSI = 0;

    }
	else
	// If temp_calc is below 20.0C
	{
        if (temp_calc > 1000)
        // If temp_calc is above 10.0C
        {
            TI = 12 * ((uint64_t)dT * dT)/pow(2,35);
    		OFFI = 30 * ((temp_calc - 2000) * (temp_calc - 2000)) / 256;
    		SENSI = 0;
        }
        else
        // If temp_calc is below 10.0C
        {
            TI = 14 * ((uint64_t)dT * dT)/pow(2,35);
    		OFFI = 35 * ((temp_calc - 2000) * (temp_calc - 2000)) / 8;
    		SENSI = 63 * ((temp_calc - 2000) * (temp_calc - 2000)) / 32;
        }

	}

	// Now bring it all together to apply offsets

	OFF = ((int64_t)coefficient[2] << 17) + (((coefficient[4] * (int64_t)dT)) >> 6);
	SENS = ((int64_t)coefficient[1] << 16) + (((coefficient[3] * (int64_t)dT)) >> 7);

	temp_calc = temp_calc - TI;
	OFF2 = OFF - OFFI;
	SENS2 = SENS - SENSI;

	// Now lets calculate the pressure


	pressure_calc = (((SENS2 * this->D1) / 2097152 ) - OFF2) / 32768;

	this->_temperature_actual = temp_calc ;
	this->_pressure_actual = pressure_calc ; // 10;// pressure_calc;


}

int32_t MS5840::get_temprature(){
	return _temperature_actual;
}


int32_t MS5840::get_pressure(){
	return _pressure_actual;
}

uint16_t * MS5840::get_coefficient(){
	return coefficient;
}