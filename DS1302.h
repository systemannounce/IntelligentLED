#ifndef __DS1302_H__
#define __DS1302_H__

void DS1302_init();
void DS1302_writebyte(unsigned char command,unsigned char Data);
unsigned char DS1302_readbyte(unsigned char command);
void DS1302_SetTime();
void DS1302_ReadTime();

extern unsigned char DS1302_TIME[];

#endif