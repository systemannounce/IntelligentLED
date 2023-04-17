#include <REGX52.H>


//宏定义

#define DS1302_second   0x80
#define DS1302_minute   0x82
#define DS1302_hour     0x84
#define DS1302_date     0x86
#define DS1302_month    0x88
#define DS1302_day      0x8a
#define DS1302_year     0x8c
#define DS1302_wp       0x8e



unsigned char DS1302_TIME[]={       //1302时间数组
    22,12,31,23,59,55
};

sbit DS1302_SCLK=P3^6;
sbit DS1302_IO=P3^4;
sbit DS1302_CE=P3^5;

/**
  * @brief  DS1302初始化函数
  * @param  无
  * @retval 无
  */
void DS1302_init()
{
    DS1302_CE=0;
    DS1302_SCLK=0;
}

/**
  * @brief  DS1302写入函数
  * @param  command 命令+地址
  * @param  data 数据
  * @retval 无
  */
void DS1302_writebyte(unsigned char command,unsigned char Data)
{
    unsigned char i;
    DS1302_CE=1;
    for(i=0;i<8;i++)
    {
        DS1302_IO=command&(0x01<<i);
        DS1302_SCLK=1;
        DS1302_SCLK=0;
    }

        for(i=0;i<8;i++)
    {
        DS1302_IO=Data&(0x01<<i);
        DS1302_SCLK=1;
        DS1302_SCLK=0;
    }
    DS1302_CE=0;
}

/**
  * @brief  DS1302读取函数
  * @param  command 命令+地址
  * @retval 无
  */
unsigned char DS1302_readbyte(unsigned char command)
{
    unsigned char i,Data=0x00;
    command|=0x01;
    DS1302_CE=1;
    for (i=0; i<8; i++)
    {
        DS1302_IO=command&(0x01<<i);
        DS1302_SCLK=0;
        DS1302_SCLK=1;

    }
    
    for ( i = 0; i < 8; i++)
    {
        DS1302_SCLK=1;
        DS1302_SCLK=0;
        if (DS1302_IO){Data|=(0x01<<i);}
    }
    DS1302_CE=0;
    DS1302_IO=0;
    return Data;
}

/**
  * @brief  DS1302芯片从DS1302_TIME数组设置时间函数
  * @retval 无
  */
void DS1302_SetTime()
{
    DS1302_writebyte(DS1302_wp,0x00);       //关闭写保护
    DS1302_writebyte(DS1302_year,DS1302_TIME[0]/10*16+DS1302_TIME[0]%10);  
    DS1302_writebyte(DS1302_month,DS1302_TIME[1]/10*16+DS1302_TIME[1]%10); 
    DS1302_writebyte(DS1302_date,DS1302_TIME[2]/10*16+DS1302_TIME[2]%10); 
    DS1302_writebyte(DS1302_hour,DS1302_TIME[3]/10*16+DS1302_TIME[3]%10); 
    DS1302_writebyte(DS1302_minute,DS1302_TIME[4]/10*16+DS1302_TIME[4]%10); 
    DS1302_writebyte(DS1302_second,DS1302_TIME[5]/10*16+DS1302_TIME[5]%10); 
    DS1302_writebyte(DS1302_day,DS1302_TIME[6]/10*16+DS1302_TIME[6]%10); 
    DS1302_writebyte(DS1302_wp,0x80);       //打开写保护
}

/**
  * @brief  DS1302读取时间函数，时间存放在数组DS1302_TIME中
  * @param  command 命令+地址
  * @retval 无
  */
void DS1302_ReadTime()
{
    unsigned char temp;
    temp=DS1302_readbyte(DS1302_year);
    DS1302_TIME[0]=temp/16*10+temp%16;
    temp=DS1302_readbyte(DS1302_month);
    DS1302_TIME[1]=temp/16*10+temp%16;
    temp=DS1302_readbyte(DS1302_date);
    DS1302_TIME[2]=temp/16*10+temp%16;
    temp=DS1302_readbyte(DS1302_hour);
    DS1302_TIME[3]=temp/16*10+temp%16;
    temp=DS1302_readbyte(DS1302_minute);
    DS1302_TIME[4]=temp/16*10+temp%16;
    temp=DS1302_readbyte(DS1302_second);
    DS1302_TIME[5]=temp/16*10+temp%16;
    temp=DS1302_readbyte(DS1302_day);
    DS1302_TIME[6]=temp/16*10+temp%16;
}