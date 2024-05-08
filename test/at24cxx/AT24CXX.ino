#include <Wire.h>
#include "AT24CXX.h"
#define EEPROM_I2C_ADDRESS 0x57

AT24C32 at24c32(EEPROM_I2C_ADDRESS);

void setup()
{
  Wire.begin();
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(8, HIGH); //Write protect test
  digitalWrite(12, LOW);
  Serial.begin(9600);
  while (!Serial);

  uint32_t data = 0xAE962B61;
  at24c32.write(4000, data); // Write protect area
  Serial.println(at24c32.read(4000)); // 255
  
  char *str = "abcdefghijklmnopqrstuvwxyz0123456789";
  char strR[strlen(str)+1];
  at24c32.write(50,(byte*)str,strlen(str)+1);
  at24c32.read(50,(byte*)strR,strlen(str)+1);
  Serial.println(strR);
  Serial.println(at24c32.read(91)); // 0
  Serial.println(at24c32.read()); // 255
  Serial.println(strcmp(str,strR));
}

void loop()
{

}