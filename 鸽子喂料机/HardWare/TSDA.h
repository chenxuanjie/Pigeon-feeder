#ifndef __TSDA_H_
#define __TSDA_H_

#include "stm32f10x.h"


#define TSDA_Usart			USART3					//�ŷ������Ӧrs232����
#define TSDA_Usart2			USART2					//�ŷ������Ӧrs232����

#define MotorStart 					0X00000101	//�������
#define	MotorStop						0X00000000	//���ֹͣ
#define	SpeedMod						0X0200C4C6	//ѡ���ٶ�ģʽ����
#define	PositionMod					0X0200d0d2	//ѡ��λ��ģʽ����
#define	MaxSpeed						0x1D				//����ٶ����ƣ������ٶȸ�8λ�ڵ�8λ�ͼ��λ��λ��ģʽ��Ч
#define	ClearCommand				0X4C00004C	//ǿ�����λ������Ϊ0
#define	StopOrder						0X4D00004D	//ͣ�ڵ�ǰλ�ã����ʣ��δ�������Ŀ��λ��
#define CommunicationOutage_ON  	0X1C000723	//����ͨѶ�ж��Զ�ͣ��
#define CommunicationOutage_OFF  	0X1C00001C	//�ر�ͨѶ�ж��Զ�ͣ��
#define	SpeedRatio					0X40				//�ٶȱ�������
#define	SpeedIntegral				0X41				//�ٶȻ�������	
#define	SpeedDifferential		0X42				//�ٶ�΢������
#define	PositionRatio				0X1A				//λ�ñ�������
#define	PositionDifferent		0X1B				//λ��΢������
#define	PositionFeedforward	0X1C				//λ��ǰ������
#define	A_D_Time						0X0A				//�ٶ�ģʽ�ļӼ���ʱ���趨����8λ���٣���8λ���٣���λx100MS
#define	SpeedSetting				0X06				//�ٶȸ�����8192��Ӧ3000RPM
#define	PositionA_D_Time		0X09				//λ��ģʽ�µļӼ���ʱ���趨�����ٶ�ģʽͬ��
#define	PositionText_H			0X50				//λ�õ��Ը�16λ
#define	PositionText_L			0X05				//λ�õ��Ե�16λ
#define	AbsolutePosition		0x51000051	//λ��ģʽ�л�����λ�ÿ���
#define	RelativePosition		0X51000152	//λ��ģʽ�л����λ�ÿ���
#define	ReadParameter				0X800080		//��ȡ��ز�������
#define	OutputSpeed					0XE4				//��ȡ���ת��
#define	PositionSet_H				0XE6				//λ�ø�����16λ
#define	PositionSet_L 			0XE7				//λ�ø�����16λ
#define	PositionFeedback_H	0XE8				//λ�÷�����16λ
#define	PositionFeedback_L	0XE9				//λ�÷�����16λ



void TSDA_Order(USART_TypeDef* pUSARTx,uint32_t order32);
void TSDA_Data(USART_TypeDef* pUSARTx,int8_t order8,int16_t data);
#endif
