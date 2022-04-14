#ifndef __TSDA_H_
#define __TSDA_H_

#include "stm32f10x.h"


#define TSDA_Usart			USART3					//伺服电机对应rs232串口
#define TSDA_Usart2			USART2					//伺服电机对应rs232串口

#define MotorStart 					0X00000101	//电机启动
#define	MotorStop						0X00000000	//电机停止
#define	SpeedMod						0X0200C4C6	//选择速度模式输入
#define	PositionMod					0X0200d0d2	//选择位置模式输入
#define	MaxSpeed						0x1D				//最高速度限制，输入速度高8位于低8位和检测位，位置模式有效
#define	ClearCommand				0X4C00004C	//强制清除位置数据为0
#define	StopOrder						0X4D00004D	//停在当前位置，清除剩余未运行完的目标位置
#define CommunicationOutage_ON  	0X1C000723	//开启通讯中断自动停机
#define CommunicationOutage_OFF  	0X1C00001C	//关闭通讯中断自动停机
#define	SpeedRatio					0X40				//速度比例增益
#define	SpeedIntegral				0X41				//速度积分增益	
#define	SpeedDifferential		0X42				//速度微分增益
#define	PositionRatio				0X1A				//位置比例增益
#define	PositionDifferent		0X1B				//位置微分增益
#define	PositionFeedforward	0X1C				//位置前馈增益
#define	A_D_Time						0X0A				//速度模式的加减速时间设定，高8位加速，低8位减速，单位x100MS
#define	SpeedSetting				0X06				//速度给定，8192对应3000RPM
#define	PositionA_D_Time		0X09				//位置模式下的加减速时间设定，和速度模式同理
#define	PositionText_H			0X50				//位置调试高16位
#define	PositionText_L			0X05				//位置调试低16位
#define	AbsolutePosition		0x51000051	//位置模式切换绝对位置控制
#define	RelativePosition		0X51000152	//位置模式切换相对位置控制
#define	ReadParameter				0X800080		//读取监控参数请求
#define	OutputSpeed					0XE4				//读取输出转速
#define	PositionSet_H				0XE6				//位置给定高16位
#define	PositionSet_L 			0XE7				//位置给定低16位
#define	PositionFeedback_H	0XE8				//位置反馈高16位
#define	PositionFeedback_L	0XE9				//位置反馈低16位



void TSDA_Order(USART_TypeDef* pUSARTx,uint32_t order32);
void TSDA_Data(USART_TypeDef* pUSARTx,int8_t order8,int16_t data);
#endif
