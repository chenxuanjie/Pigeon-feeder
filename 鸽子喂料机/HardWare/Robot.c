#include "ROBOT.H"
#include "TSDA.H"
#include "DELAY.H"

int16_t SpeedLeft=0,SpeedRight=0;

void Robot_SetSpeed(int16_t data)
{
	SpeedLeft=data;
	SpeedRight=data;
}

void Robot_SetSpeedLeft(int16_t Data)
{
	SpeedLeft = Data;
}

void Robot_SetSpeedRight(int16_t Data)
{
	SpeedRight = Data;
}

int16_t Robot_GetSpeedLeft(void)
{
	return SpeedLeft;
}

int16_t Robot_GetSpeedRight(void)
{
	return SpeedRight;
}

/**
  * @brief  ��λ���㣨cm/s -> ͨѶ���ݣ�
  * @param  data:ι�ϻ��ƶ��ٶ� ��λcm/s ֻ��������0~100��
  * @retval ��
  */
int16_t SpeedConversion(int32_t data)
{
	if(data==0);
	else if(data>=-100&&data<=100)
	{
	data=data*60*ReductionRatio;		 //r/s����ΪRPM
	data=data*10/TireDiameter/3.14;  //cm/s����Ϊr/s
	data=data*8192/3000;						 //RPM����Ϊ�ֽ�����
	
	}
	else data=0;					 //���޻���Ϊ3000RPM
	return (int16_t)data;
}

/**
  * @brief  ��ʼ������ι�ϻ�
  * @param  ��
  * @retval ��
  */
void Robot_Init(void)
{
	TSDA_Order(LeftWheel,MotorStop);
	TSDA_Order(RightWheel,MotorStop);
	//����ͨѶ�ж��Զ�ͣ��
	TSDA_Order(LeftWheel,CommunicationOutage_ON);
	TSDA_Order(RightWheel,CommunicationOutage_ON);
	Delay_ms(10);
	//ѡ���ٶ�ģʽ����
	TSDA_Order(LeftWheel,SpeedMod);
	TSDA_Order(RightWheel,SpeedMod);
	Delay_ms(10);	
	
}

/**
  * @brief  ι�ϻ��ƶ�����
* @param  LeftSpeed:�����ٶȣ�RightSpeed�������ٶȣ�
  * @retval ��
  */
void Robot_Move(int16_t LeftSpeed,int16_t RightSpeed)
{
	TSDA_Data(LeftWheel,SpeedSetting,LeftSpeed);
	TSDA_Data(RightWheel,SpeedSetting,RightSpeed);
	
}
/**
  * @brief  ����ͣ��
  * @param  ��
  * @retval ��
  */
void Robot_ShutDown(void)
{
	
	
  TSDA_Data(LeftWheel,SpeedSetting,0X00);
	TSDA_Data(RightWheel,SpeedSetting,0X00);
}

