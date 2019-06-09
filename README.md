基于STC89C52的电子钟
====
这是一个基于51单片机的电子钟<br/>
## 上手指南
以下指南将帮助你如何运行本项目
### 安装要求
1. Windows系统
2. 安装Keil C51 uVision5
3. stc-isp 烧录软件
4. Proteus(可选)
### 单片机硬件要求
1. STC89C52RC
2. DHT21(又名AM2302)
3. DS1302
4. 红外接收
5. 声音传感器
6. ny3p035语音IC
### 安装步骤
1. 将本项目代码clone到本地
2. 使用Keil C51打开项目根目录下的 EClock.uvproj 文件
3. Build 本工程
4. 使用stc-isp将本项目烧录到单片机中