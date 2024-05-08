/**

AT24CXX.cpp
Library for using the EEPROM AT24C32/64

Copyright (c) 2014 Christian Paul

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

 */
#include "AT24CXX.h"
#include <Wire.h>

/**
 * Constructor with AT24CXX EEPROM at i2c_address 0
 */
AT24CXX::AT24CXX() {
	init(I2C_BASE_ADDRESS, 32);
}

/**
 * Constructor with AT24CXX EEPROM at given i2c_address and size of page
 */
AT24CXX::AT24CXX(uint8_t i2c_address, uint8_t pageSize) {
	init(i2c_address, pageSize);
}

/**
 * Constructor with AT24C32 EEPROM at i2c_address 0
 */
AT24C32::AT24C32() {
	init(I2C_BASE_ADDRESS, 32);
}
/**
 * Constructor with AT24CXX EEPROM at given i2c_address
 */
AT24C32::AT24C32(uint8_t i2c_address) {
	init(i2c_address, 32);
}

/**
 * Constructor with AT24C64 EEPROM at i2c_address 0
 */
AT24C64::AT24C64() {
	init(I2C_BASE_ADDRESS, 32);
}
/**
 * Constructor with AT24C64 EEPROM at given i2c_address
 */
AT24C64::AT24C64(uint8_t i2c_address) {
	init(i2c_address, 32);
}

/**
 * Constructor with AT24C128 EEPROM at i2c_address 0
 */
AT24C128::AT24C128() {
	init(I2C_BASE_ADDRESS, 64);
}
/**
 * Constructor with AT24C128 EEPROM at given i2c_address
 */
AT24C128::AT24C128(uint8_t i2c_address) {
	init(i2c_address, 64);
}

/**
 * Constructor with AT24C256 EEPROM at i2c_address 0
 */
AT24C256::AT24C256() {
	init(I2C_BASE_ADDRESS, 64);
}
/**
 * Constructor with AT24C128 EEPROM at given i2c_address
 */
AT24C256::AT24C256(uint8_t i2c_address) {
	init(i2c_address, 64);
}

/**
 * Constructor with AT24C512 EEPROM at i2c_address 0
 */
AT24C512::AT24C512() {
	init(I2C_BASE_ADDRESS, 128);
}
/**
 * Constructor with AT24C512 EEPROM at given i2c_address
 */
AT24C512::AT24C512(uint8_t i2c_address) {
	init(i2c_address, 128);
}

/**
 * Init
 */
void AT24CXX::init(uint8_t i2c_address, uint8_t pageSize) {
	_id  = i2c_address;
	_pageSize = pageSize;
	Wire.begin();
}

bool AT24CXX::pollACK() {
	error = false;
	Wire.beginTransmission(_id);
	//digitalWrite(12, HIGH);
	uint32_t pollingStart = millis();
	while(millis()-pollingStart < 15)
		if(!Wire.endTransmission())return true;
	error = true;
	return false;
}

/**
 * Write uint8_t
 */
bool AT24CXX::write(unsigned int address, uint8_t data) {
  if(pollACK()){  
		Wire.beginTransmission(_id);
		Wire.write(address >> 8);
		Wire.write(address & 0xFF);
			Wire.write(data);
		Wire.endTransmission();
		return true;
	}
	return false;
}


bool AT24CXX::write(unsigned int address, uint8_t *data, int n) {
	int bytes_left = n;						// bytes left to write
	int offD = 0;					// current offset in data pointer
	int offP;						// current offset in page
	int bytes_to_write = 0;						// next n bytes to write

	// write alle bytes in multiple steps
	while (bytes_left > 0) {
		// calc offset in page
		offP = address % _pageSize;
		// max i2c buffer size bytes to write -2 address bytes
		bytes_to_write = min(min(bytes_left, BUFFER_LENGTH-2), _pageSize - offP);
		if (pollACK()) {
     	Wire.beginTransmission(_id);
    	Wire.write(address >> 8);
    	Wire.write(address & 0xFF);
    	uint8_t *adr = data+offD;
    	Wire.write(adr, bytes_to_write);
    	Wire.endTransmission();
    } else return false;
		bytes_left -= bytes_to_write;
		offD += bytes_to_write;
		address += bytes_to_write;
	}
	return true;
}

/**
 * Read current address
 */
uint8_t AT24CXX::read(){
	if(pollACK()){
		Wire.requestFrom(_id, 1);
		return Wire.read();
	}
	else return 0;
}

/**
 * Read uint8_t
 */
uint8_t AT24CXX::read(unsigned int address) {
	uint8_t b = 0;
	int r = 0;
	if(pollACK()) {
		Wire.beginTransmission(_id);
		Wire.write(address >> 8);
		Wire.write(address & 0xFF);
		if (Wire.endTransmission()==0) {
			Wire.requestFrom(_id, 1);
			while (Wire.available() > 0 && r<1) {
				b = (uint8_t)Wire.read();
				r++;
			}
		}
	}
	return b;
}

/**
 * Read sequence of n bytes
 */
uint8_t* AT24CXX::read(unsigned int address, uint8_t *data, int n) {
	int c = n;
	int offD = 0;
	// read until n bytes are read
	while (c > 0) {
		int nc = c;
		if (nc > BUFFER_LENGTH)
			nc = BUFFER_LENGTH;
		if(pollACK()){
			Wire.beginTransmission(_id);
			Wire.write(address >> 8);
			Wire.write(address & 0xFF);
			if (Wire.endTransmission()==0) {
				int r = 0;
				Wire.requestFrom(_id, nc);
				while (Wire.available() > 0 && r<nc) {
					data[offD+r] = (uint8_t)Wire.read();
					r++;
				}
			}
		}
		address+=nc;
		offD+=nc;
		c-=nc;
	}
  return data;
}