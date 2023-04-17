#include <REGX52.H>
#include "LCD1602.h"
#include "XPT2046.h"
#include <INTRINS.H>
#include "DS1302.h"
unsigned int CODEC , PTC , LDR , LDR_low , l1=0 , time , pwm , pwm_count;
sbit key1=P3^1;
sbit key2=P3^0;
sbit key3=P3^2;
sbit key4=P3^3;
sbit led=P2^0;
sbit led2=P2^1



void i0_server_(void) interrupt 1
{
    TH0 = 0xFC;
    TL0 = 0x18;
    // time++;
    // if(time>60000)time =0;
    if(l1 == 1)
    {
        pwm_count++;
        if(pwm_count==pwm)led=0;
        if(pwm_count==14){led=1;pwm_count=0;}        
    }

}

void Init_t0(void) // 定时器初始化
{
    TMOD = 0x01; // 选择方式1
    TH0 = 0xFC;
    TL0 = 0x18; // 1ms
    EA = 1;
    ET0 = 1;
    TR0 = 1; // 开始计数
}


void Delay1ms(int ms)		//@11.0592MHz
{
    while(ms--)
    {
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
    }

}

int interface = 3 , mode = 1 , et2 = 1 , et3 = 1 , temlimit = 27;

void main(void)
{
    Init_t0();
	LCD_Init();
    DS1302_init();
    DS1302_SetTime();
    LCD_ShowString(2,11,"MODE:");
    pwm_count = 0;
	// LCD_ShowString(1,1,"ADJ  NTC  GR");
	while(1)
	{
		CODEC = XPT2046_ReadAD(XPT2046_XP);		//读取AIN0，可调电阻
		// LCD_ShowNum(2,1,ADValue,3);				//显示AIN0
		PTC = XPT2046_ReadAD(XPT2046_YP)-118;		//读取AIN1，热敏电阻
		// LCD_ShowNum(2,6,ADValue,3);				//显示AIN1
		LDR = XPT2046_ReadAD(XPT2046_VBAT);	//读取AIN2，光敏电阻
		// LCD_ShowNum(2,11,ADValue,3);			//显示AIN2
        pwm = CODEC/20+1;
        DS1302_ReadTime();
        LCD_ShowNum(2,16,mode,1);


        while(key1==0){}

        //界面区分
        if(interface == 1)
        {
            et2 = 1;
            et3 = 1;
            LCD_ShowString(1,1,"Tem:");
            LCD_ShowNum(1,6,PTC,2);
        }
        if(interface == 2)
        {
            et3 = 1;
            if(et2==1)LCD_ShowString(1,1,"Timer:   -  -  ");
            LCD_ShowNum(1,8,DS1302_TIME[3],2);
            LCD_ShowNum(1,11,DS1302_TIME[4],2);
            LCD_ShowNum(1,14,DS1302_TIME[5],2);
            et2 = 0;

        }
        if(interface == 3)
        {
            et2 = 1;
            if(et3==1)
            {
            LCD_ShowString(1,1,"LDR:     L1:");
            LCD_ShowString(2,1,"CODEC");                
            }

            LCD_ShowNum(1,5,LDR,3);
            // LCD_ShowNum(1,10,LDR_low,3);
            LCD_ShowNum(2,7,CODEC,3);
            if(l1==1)LCD_ShowString(1,13,"ON");
            if(l1==0)LCD_ShowString(1,13,"OFF");
            et3 = 0;
        }
        if(interface == 4)
        {
            et2=1;
            et3=1;
            LCD_ShowString(1,1,"Temlimit:");
            LCD_ShowNum(1,10,temlimit,2);
        }
	}
}
