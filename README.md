基于STC89C52的电子钟
====
> 这是一个基于51单片机的电子钟<br/>

## 硬件

### 原理图

![Schematic](./extras/images/Schematic.png)

### PCB

![Schematic](extras/images/PCB.png)



## 上手指南

> 以下指南将帮助你如何运行本项目

### 安装要求
1. Windows系统
2. 安装Keil C51 uVision5
3. stc-isp 烧录软件
4. Proteus(可选)
### 单片机硬件要求
1. STC89C52RC
2. DHT21 (又名 AM2302)
3. DS1302
4. 红外接收
5. 声音传感器
6. 语音播报模块
7. LCD1602
8. 电路连接 (参照 extras/images/Schematic.png)
### 安装步骤
1. 将本项目代码 Clone 到本地
2. 使用Keil C51打开项目根目录下的 EClock.uvproj 文件
3. Build 本工程
4. 使用 stc-isp 将本项目烧录到单片机中
## 功能
1. 开机显示时间, 10s后切换到显示温湿度, 再10s后熄灭显示屏
2. 按键调整时间
3. 拍手两下, 显示屏开启, 并语音播报当前时间
4. 遥控, 1键使显示屏常亮, 2键播报时间/温湿度, 3键切换显示