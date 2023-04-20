#include <REGX52.H>
#include "LCD1602.h"
#include "XPT2046.h"
#include <INTRINS.H>
#include <math.h>
#include "DS1302.h"
#include "ds18b02.h"

unsigned int CODEC , LDR , LDR_low , l1=0 , time , pwm , pwm_count , control = 0 , current , tec = 0;//tec:1为环境为亮转手动，为2时则为暗转手动，0为自动
double PTC=20 , temptc , ins , temp;
unsigned int count , interrupttime , stop;
unsigned int interface = 1 , mode = 1 , et = 1 , et1 = 1 , et2 = 1 , et3 = 1 , et4 = 1, temlimit = 27 , te , uptem = 0;
sbit key1=P3^1;
sbit key2=P3^0;
sbit key3=P3^2;
sbit key4=P3^3;
sbit led=P2^0;
sbit led2=P2^1;



void i0_server_(void) interrupt 1
{
    TH0 = 0xFC;
    TL0 = 0x18;
    if(stop ==1 && count < 3100)count++;
    // time++;
    // if(time>60000)time =0;
    if(l1 == 1)
    {
        pwm_count++;
        if(pwm_count==pwm)led=0;
        if(pwm_count==14){led=1;pwm_count=0;}        
    }
    if(l1==0)led=1;

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


void Delay1ms(int s)		//@11.0592MHz
{
    while(s--)
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



void main(void)
{
    Init_t0();
	LCD_Init();
    DS1302_init();
    DS1302_SetTime();
    pwm_count = 0;
	// LCD_ShowString(1,1,"ADJ  NTC  GR");
	while(1)
	{
        LCD_ShowNum(2,16,mode,1);   //显示模式
        LCD_ShowString(2,11,"MODE:");
		CODEC = XPT2046_ReadAD(XPT2046_XP);		//读取AIN0，可调电阻
		// LCD_ShowNum(2,1,ADValue,3);				//显示AIN0
		// PTC = XPT2046_ReadAD(XPT2046_YP)*0.39-29.87;		//读取AIN1，热敏电阻
        // temptc = modf(PTC , &ins);
        
		readTemperature();	  //读取温度
        temp = ((temperature[0]/100) + 0x30)*100 + ((temperature[0]/10%10) + 0x30)*10 + ((temperature[0]%10) + 0x30)*1-5326;
        temptc = ((temperature[1]*10/16) + 0x30)*0.1 + ((temperature[1]*100/16%10) + 0x30)*0.01;
        if(temp-PTC>10 || temp-PTC<-10){}
        else PTC = temp;
		// LCD_ShowNum(2,6,ADValue,3);				//显示AIN1
		LDR = XPT2046_ReadAD(XPT2046_VBAT);	//读取AIN2，光敏电阻
		// LCD_ShowNum(2,11,ADValue,3);			//显示AIN2
        pwm = CODEC/20+1;
        DS1302_ReadTime();


        while(key1==0){Delay1ms(10);if(et==1)l1 = !l1;control = 1;et=0;}
        while(key2==0){Delay1ms(10);if(et==1)interface++;if(interface==5)interface=1;et=0;}
        while(key3==0){Delay1ms(10);if(et==1)mode++;if(mode==3)mode=1;et=0;}
        while(key4==0){Delay1ms(10);if(et==1){temlimit++;}if(temlimit==36)temlimit = 20 ; et = 0;}
        et = 1;
        //界面区分
        if(interface == 1)
        {
            et2 = 1;
            et3 = 1;
            et4 = 1;
            if(et1==1)LCD_Init();
            LCD_ShowString(1,1,"Tem:");
            LCD_ShowString(1,8,".");
            LCD_ShowString(2,1,"Limit:");
            LCD_ShowNum(1,6,PTC,2);
            // LCD_ShowNum(1,13,ds18b20Flag,1);
            LCD_ShowNum(1,9,temptc*100,2);
            LCD_ShowNum(2,7,temlimit,2);
            et1 = 0;
        }
        if(interface == 2)
        {
            et1 = 1;
            et3 = 1;
            et4 = 1;
            if(et2==1){LCD_Init();LCD_ShowString(1,1,"Timer:   -  -  ");}
            LCD_ShowNum(1,8,DS1302_TIME[3],2);
            LCD_ShowNum(1,11,DS1302_TIME[4],2);
            LCD_ShowNum(1,14,DS1302_TIME[5],2);
            et2 = 0;

        }
        if(interface == 3)
        {
            et1 = 1;
            et2 = 1;
            et4 = 1;
            if(et3==1)
            {
            LCD_Init();
            LCD_ShowString(1,1,"LDR:     L1:");
            LCD_ShowString(2,1,"CODEC");                
            }

            LCD_ShowNum(1,5,LDR,3);
            // LCD_ShowNum(1,10,LDR_low,3);
            LCD_ShowNum(2,7,CODEC,3);
            if(l1==1)LCD_ShowString(1,13,"ON ");
            if(l1==0)LCD_ShowString(1,13,"OFF");
            et3 = 0;
        }
        if(interface == 4)
        {
            if(et4==1)LCD_Init();
            et1 = 1;
            et2 = 1;
            et3 = 1;
            LCD_ShowString(1,1,"Temlimit:");
            LCD_ShowNum(1,10,temlimit,2);
            et4 = 0;
        }
        if(count%500<400 && count<3000 && stop ==1)led2 = 0;
        else led2 = 1;
        if(PTC>temlimit && uptem == 0)
            {
                uptem = 1;
                stop = 1;
            }
            if(PTC<=temlimit-1)
            {
                if(count>3000)
                {
                    stop = 0;
                    uptem = 0;
                    count = 0;

                }
            }
        // if(PTC>temlimit && uptem == 0)
        // {
        //     uptem = 1;
        //     for(te=1;te<7;te++)
        //     {
        //         led2 = 0;
        //         Delay1ms(400);
        //         led2=1;
        //         Delay1ms(100);
        //     }
        // }
        // if(PTC<=temlimit-1)uptem = 0;
        if(mode==1)
        {
            if(LDR<100)
            {
                if(tec==1)//防止自动复位失效
                {
                    tec=0;
                    control=0;
                    l1=0;
                }
                if(control==1)tec=2;//切换手动
                current = 0;
                if(tec==0)
                {
                    l1=1;
                }
                if(tec==2)
                {

                }
            }
            if(LDR>=100)
            {
                if(tec==2)//防止自动复位失效
                {
                    tec=0;
                    control=0;
                    l1=0;
                }
                if(control==1)tec=1;//切换手动
                current = 1;
                if(tec==0)
                {
                    l1=0;
                }
                if(tec==1)
                {

                }
            }
        }
        if(mode==2)
        {
            if(DS1302_TIME[3]>6 && DS1302_TIME[3]<18)l1=0;
            else l1=1;
        }
	}
}
