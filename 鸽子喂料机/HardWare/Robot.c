#include "ROBOT.H"
#include "TSDA.H"
#include "DELAY.H"

/**
  * @brief  单位换算（cm/s -> 通讯数据）
  * @param  data:喂料机移动速度 单位cm/s 只容许输入0~100；
  * @retval 无
  */
int16_t SpeedConversion(int32_t data)
{
	if(data==0);
	else if(data>=-100&&data<=100)
	{
	data=data*60*ReductionRatio;		 //r/s换算为RPM
	data=data*10/TireDiameter/3.14;  //cm/s换算为r/s
	data=data*8192/3000;						 //RPM换算为字节数据
	
	}
	else data=0;					 //超限换算为3000RPM
	return (int16_t)data;
}

/**
  * @brief  初始化鸽子喂料机
  * @param  无
  * @retval 无
  */
void Robot_Init(void)
{
	TSDA_Order(LeftWheel,MotorStop);
	TSDA_Order(RightWheel,MotorStop);
	TSDA_Order(LeftWheel,CommunicationOutage_ON);
	TSDA_Order(RightWheel,CommunicationOutage_ON);
	Delay_ms(10);
	TSDA_Order(LeftWheel,SpeedMod);
	TSDA_Order(RightWheel,SpeedMod);
	Delay_ms(10);	
	
}

/**
  * @brief  喂料机移动参数
* @param  LeftSpeed:左轮速度；RightSpeed：右轮速度；
  * @retval 无
  */
void Robot_Move(int16_t LeftSpeed,int16_t RightSpeed)
{
	TSDA_Data(LeftWheel,SpeedSetting,LeftSpeed);
	TSDA_Data(RightWheel,SpeedSetting,RightSpeed);
	
}
/**
  * @brief  紧急停机
  * @param  无
  * @retval 无
  */
void Robot_ShutDown(void)
{
	
	
  TSDA_Data(LeftWheel,SpeedSetting,0X00);
	TSDA_Data(RightWheel,SpeedSetting,0X00);
}

