# 42_stepper_motor_drive
MAIN分支：版本保留分支（旧版本文件）

42步进电机驱动程序

--------------------
主机程序--ESP32_WEB9

v1.1

提供web页面远程遥控

提供电机驱动程序

提供与从机串口通信

--------------------
从机程序--ESP32_UART

v1.1

提供电机驱动程序

提供PID闭环控制程序

提供与主机串口通信

--------------------
PCB文件--AS5600

问题汇总：

1、驱动能力较差，速度慢，力矩小，MX1508最高供电电压为<10V（为节省成本）

2、存在电机反电动势回灌问题，外力转动电机可能导致器件损坏（测试最高瞬时反电动势可达8V）

3、控制板逻辑部分与驱动部分单独供电，无DCDC电压转换电路（为节省成本）

