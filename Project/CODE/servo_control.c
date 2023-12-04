/**
  ******************************************************************************
  * @author  Pump_Li
  * @version V1.0.0
  * @date    2023.3.13
  * @brief   舵机打角PID控制
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "headfile.h"
/* Private Define ------------------------------------------------------------*/
int16 Last_error,integral,delta;  //偏差   上次偏差   积分   微分
int16 Now_duty;  //位置式pid输出
int16 angle;    //舵机占空比输出
int16 position;
float T=0.000055;
float Kp =1 ;
float Ki = 0;
float Kd = 4;

float Kp_turn = 3;
float Ki_turn = 0;
float Kd_turn = 0;

extern double K_diff = 0;  //过弯差速比

/* Private Functions ---------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
//舵机初始化
/**
  * @brief  舵机初始化回中
  * @param  None
  * @retval None
  */
void servo_init(void)
{

	pwm_init(PWMB_CH1_P74,50,705);       //频率为50hz。占空比为7%

  gpio_mode(P7_7,GPO_PP);	//蜂鸣器接口初始化
	Buzz=0;   //拉低，防止蜂鸣器一直叫
//  pwm_duty(PWMB_CH1_P74,700);   //舵机归中时在while调用，中值时占空比为7.5%，这里增大了100倍  中值定为700.限幅(620--->785).....
}
/**
  * @brief  舵机循线PID控制
  * @param  None
  * @retval None
  */
void servo_pid_street(float left, float right)
{
  float error;
  error = left-right;
//  error = position;
//	if(position>40)
//	{
//		Kp =T*position*position+0.2;
//		Kp=Kp>0.8?0.8:Kp;
//	}
//	else
//	{
//		Kp=0.5;
//	}
	integral += error;
	delta = error - Last_error;
	Now_duty = Kp*error + Ki*integral +Kd*delta;
	Last_error = error;
	 
	angle = 705 + Now_duty;
	
	angle = (angle > angle_max) ? angle_max:angle;    //舵机转角限幅
	angle = (angle < angle_min) ? angle_min:angle;
	pwm_duty(PWMB_CH1_P74, angle) ;
	
    //K_diff = ((15.5/39)*tan(0.18*Now_duty*DEC))+0.05;   //过弯差速比
		//K_diff = (15.5/39)*tan(0.18*Now_duty*DEC)-0.02;   //过弯差速比
}
//void Steering_Control_Out(int16 duty)
//{
//     duty = Steer_Duty_Midle + duty ;//在舵机中值的基础上进行偏移
//   if (duty >= Steer_Duty_Max) duty = Steer_Duty_Max;
//   else if(duty <= Steer_Duty_Min) duty = Steer_Duty_Min;
//     pwm_duty(Steer_Pin, duty);
//}
/**
  * @brief  舵机转向PID控制
  * @param  None
  * @retval None
  */
void servo_pid_turn(float left, float right)
{
  float error;
  error = left-right;
	integral += error;
	delta = error - Last_error;
	Now_duty = Kp_turn*error + Ki_turn*integral +Kd_turn*delta;
	Last_error = error;
	 
	angle = 705 + Now_duty;
	
	angle = (angle > angle_max) ? angle_max:angle;    //舵机转角限幅
	angle = (angle < angle_min) ? angle_min:angle;
	pwm_duty(PWMB_CH1_P74, angle) ;
	
    //K_diff = ((15.5/39)*tan(0.18*Now_duty*DEC))+0.05;   //过弯差速比
		//K_diff = (15.5/39)*tan(0.18*Now_duty*DEC)-0.02;   //过弯差速比
}
/**
  * @brief  舵机控制中断函数
  * @param  None
  * @retval None
  */
void Servo_IT()
{
  if(Left_Xie_Adc>140&&Right_Xie_Adc>140)
  servo_pid_street(Left_Xie_Adc,Right_Xie_Adc);
  else if(Left_Xie_Adc<140&&Right_Xie_Adc<140&&Left_Xie_Adc>10&&Right_Xie_Adc>10)
  servo_pid_turn(Left_Xie_Adc,Right_Xie_Adc);
}

/**
  * @brief  出圈保护
  * @param  None
  * @retval None
  */
void  Protect_Anticollision()
{
  if(Left_Xie_Adc<10&&Right_Xie_Adc<10)
  {
    while(1)
		{
        motor_speed(0.0);
		}
    }
}  
