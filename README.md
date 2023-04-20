# IntelligentLED

## 型号

单片机：STC89c52-RC

晶振：11.0592(延迟函数都靠这玩意)

## 功能描述

通过读取DS18B20温度传感器，获取环境温度数据
通过读取DS1302时钟芯片，获取时，分，秒数据
通过XPT2046读取光敏电阻和编码电阻的阻值。
通过LCD1602完成题目要求的数据显示功能。
通过按键完成题目要求的显示界面切换和设置功能。

## 显示功能（LCD1602）
1）温度界面，显示温度传感器的数据，如23.0625℃（精确到小数点两位），
	温度最大阈值参数默认27℃。
2）时间界面，如Timer：23-59-33
3）参数界面，显示光敏（ADJ）、编码器的电阻（GR）、灯的开启状态（Key）
（在三个界面最后显示模式号）
## 按键功能
Key1：在模式一中，手动关闭和开启L1。
Key2：切换界面。
Key3；切换模式。

### 模式说明
模式1，智能灯：环境为亮的时候L1关闭，环境为暗的时候L1点亮。

可以在环境亮的时候按下Key1将L1开启，也可以在环境暗的时候按下Key1手动关闭L1。

模式2，定时灯：在6.00~18.00的时间内（白天）L1处于关闭，其余时间（晚上）开启。

说明：

1.通过光敏电阻的阻值来判断环境的亮与暗。

2.在模式1中，可以通过编码器来动态调节灯的亮暗程度（PWM）。

### 附加：

3.增加一个新的温度参数界面，显示温度阈值27℃。

可以通过Key4对温度阈值增加1℃，最大为35℃，但为35℃时增加1℃变为20℃，如此循环。

当读取的温度大于温度阈值的时候L2以间隔0.1秒闪烁3秒后关闭。


## bug:

1.已知DS18B20和DS1302冲突，导致温度传感器出现异常读数影响高温提醒的判断，已经在代码中追加了修正补偿功能，
但是不适用于当地气温高于30度已经低于10度的地区修正，会导致温度数据无法刷新，请自行在main.c文件里更改PTC初值以适应当地变化。
又或者你有更好的想法可以issue提出或者直接提交一份pull request
