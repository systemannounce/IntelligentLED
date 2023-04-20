#ifndef __DS18B02_H__
#define __DS18B02_H__


void initDs18b20();
void ds18b20WriteData(unsigned char com);
unsigned char ds18b20ReadData();
void readTemperature();
extern unsigned char ds18b20Flag;

extern unsigned char temperature[2]; //存放温度整数和小数
extern unsigned char code array[14];
extern unsigned char code array1[5];

#endif