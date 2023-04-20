#include <reg52.h>
// #include "lcd1602.h"
#include <INTRINS.H>

sbit DS18B20 = P3^7; //DS18B20引脚
unsigned char ds18b20Flag; //DS18B20是否响应标志位
unsigned char temperature[2] = {'\0'}; //存放温度整数和小数
unsigned char code array[14] = {"DS18B20 error!"};
unsigned char code array1[5] = {"Temp:"};


void Delay10us(int mss)		//@11.0592MHz
{
    do
    {
 	unsigned char i;

	i = 2;
	while (--i);
    } while (mss--);
}
void delay45us()		//@11.0592MHz
{
	unsigned char i;

	i = 18;
	while (--i);
}



/*DS18B20初始化函数*/
void initDs18b20()
{
	DS18B20 = 1;
	Delay10us(2);
	DS18B20 = 0;   //拉低电平 
	Delay10us(48);  //480us-960us之间均可
	DS18B20 = 1;   //拉高电平
	Delay10us(5);   //等待15us-60us
	if(DS18B20 == 1) //如果未响应
	{
		ds18b20Flag = 1; //DS18B20未响应 标志位置1
	}
	Delay10us(24);	
}

	
/*DS18B20写命令函数*/
void ds18b20WriteData(unsigned char com)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DS18B20 = 0; //拉低电平
		//延时至少1us
		DS18B20 = com&0x01;
	    Delay10us(6);
		DS18B20 = 1;
		com = com>>1;
	}		
}

/*DS18B20读数据函数*/
unsigned char ds18b20ReadData()
{
	unsigned char i;
	unsigned char dat = 0; //接收数据

	for(i=0;i<8;i++)
	{
		DS18B20 = 0;  //拉低电平至少1us
		dat=dat>>1;	  //右移一位
		DS18B20 = 1; //释放总线
		if(DS18B20 == 1)
		{
			dat = dat|0x80;
		}
		delay45us(); //保持45us
		DS18B20 = 1; //释放总线
	}
	return dat;
}

/*读取温度函数*/
void readTemperature()
{
	unsigned char temperatureLow;  //温度低字节
	unsigned char temperatureHigh; //温度高字节
	unsigned char tempFlag = 0; //温度正负标志位
	unsigned char disposeValue; //处理值
	

	initDs18b20();	//初始化DS18B20
	ds18b20WriteData(0xcc); //跳过ROM
	ds18b20WriteData(0x44); //启动温度转换指令
	//延时一段时间
	Delay10us(60);
	
	initDs18b20(); //每一次写命令都需要初始化
	ds18b20WriteData(0xcc); //跳过ROM
	ds18b20WriteData(0xbe); //读暂存器内容
	
	temperatureLow = ds18b20ReadData();	 //获取温度的第一个字节
	temperatureHigh = ds18b20ReadData(); //获取温度的第二个字节
	
	disposeValue = (temperatureHigh<<4)|(temperatureLow>>4);  //获取温度整数部分
	if(disposeValue>=128)  //温度是负数
	{
		disposeValue = ~disposeValue+1;
		tempFlag = 1;
	}
	temperature[0] = disposeValue; //温度整数
	temperature[1] = temperatureLow & 0x0f; //温度小数 
		
}

// void displayTemperature()
// {
// 	write_com(0x85);
// 	write_data((temperature[0]/100) + 0x30); //温度百位
// 	write_com(0x86);
// 	write_data((temperature[0]/10%10) + 0x30); //温度十位
// 	write_com(0x87);
// 	write_data((temperature[0]%10) + 0x30);   //温度个位

// 	write_com(0x88);
// 	write_data('.');

// 	write_com(0x89);
// 	write_data((temperature[1]*10/16) + 0x30);	//小数第一位
// //	write_data((tempterature[1]*625/1000));
// 	write_com(0x8a);
// 	write_data((temperature[1]*100/16%10) + 0x30);	//小数第二位
// //	write_data((tempterature[1]*625/100%10));

// 	/* 显示 ℃ */
// 	write_com(0x8b);
// 	write_data(0xdf);
// 	write_com(0x8c); 
// 	write_data('C');
// }


// void main()
// {
// 	unsigned char i;

// 	init_lcd(); //初始化1602
// 	write_com(0x80);
// 	for(i=0;i<5;i++)
// 	{
// 		write_data(array1[i]);
// 	}
// 	while(1)
// 	{
// 		readTemperature();	  //读取温度
			
// 		if(ds18b20Flag == 1)  //如果初始化错误
// 		{
// 			for(i=0;i<14;i++)
// 			{
// 				write_com(0x80);
// 				write_data(array[i]);	//显示DS18B20 error!
// 			}
// 		}
// 		else
// 		{
// 			displayTemperature(); //温度显示
// 		}
				
// 	}
// }
