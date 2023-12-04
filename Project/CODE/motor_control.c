/**
  ******************************************************************************
  * @author  Pump_Li
  * @version V1.0.0
  * @date    2023.3.13
  * @brief    电机控制
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "headfile.h"

/* Private Define ------------------------------------------------------------*/
int16 Left_Encoder = 0;     //左编码器的值
int16 Right_Encoder = 0;    //右编码器的值
//int16 Average_Encoder = 0;  //左右编码器平均值
//int16 Sum_Encoder = 0;      //编码器脉冲和
//int32 distance = 0;         //车子行走的路程

//int16 K_diff = 0 ;          //差速系数



float K_P_L = 1000;
float K_I_L = 10;
float K_D_L = 0;
float K_P_R = 1000;
float K_I_R = 10;
float K_D_R = 0;
int16 Right_Duty=0;
int16 Left_Duty=0;
int16 pid_Delta_L,pid_Delta_R;
float right_wheel_goal;
float left_wheel_goal;


#define WHEEL_R 32.5       //单位mm
#define WHEEL_POSITION 2*WHELL_R      //
#define ENCODER_POSITON 
#define ENCODER_SPEED

/* Private Functions ---------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
/**
  * @brief  左轮速度pid
  * @param  goal
  * @retval None
  */
void left_motor_pid(float goal)
{
  static float error_L=0,last_error_L=0,last_last_error_L=0,Goal_speed_L,error_sum_L;
	Goal_speed_L = (float)(goal-K_diff);
  left_wheel_goal= Goal_speed_L;
  
//  if(Goal_speed<2*goal)
//    Goal_speed = Goal_speed;
//  else Goal_speed = 2*goal;  //两轮差速限幅
  
	error_L = Goal_speed_L - 0.0017f*10*Left_Encoder;            //V = N/1024*（30/68*20.1/100*20*10） = N*0.0017*10 m/s  30为编码器齿轮，68为传动齿轮，20.1为轮子周长，控制周期50ms
  if(error_L<-0.5)error_L=-0.5;
  //error_sum_L += error_L;
  pid_Delta_L =(int16)(K_P_L*(error_L-last_error_L)+K_I_L*error_L+K_D_L*(error_L-2*last_error_L+last_last_error_L));
  Left_Duty += pid_Delta_L;
  
  last_error_L = error_L;
	last_last_error_L = last_error_L;
	
//	Left_Duty -=  position*0.1;
	Left_Duty = (Left_Duty>4000) ? 4000:Left_Duty;
	Left_Duty = (Left_Duty<=(-4000)) ? (-4000):Left_Duty;
	
	
  Moto_L(Left_Duty);
	
}	
/**
  * @brief  右轮速度pid
  * @param  goal
  * @retval None
  */
void right_motor_pid(float goal)
{
  static float error_R=0,last_error_R=0,last_last_error_R=0,Goal_speed_R,error_sum_R;
	Goal_speed_R = (float)(goal+K_diff);
  right_wheel_goal = Goal_speed_R;
//  
//  if (Goal_speed_R<2*goal)
//    Goal_speed_R = Goal_speed_R;
//  else Goal_speed_R = 2*goal;
//  
	error_R = Goal_speed_R -0.0017f*10* Right_Encoder;
  if(error_R<-0.5)error_R=-0.5;
  error_sum_R += error_R;
	pid_Delta_R=(int16)(K_P_R*(error_R-last_error_R)+K_I_R*error_R+K_D_R*(error_R-2*last_error_R+last_last_error_R));
	Right_Duty += pid_Delta_R;
  
  last_error_R = error_R;
	last_last_error_R = last_error_R;
	
//	Right_Duty +=  position*0.1;
	
	Right_Duty = (Right_Duty>4000) ? 4000:Right_Duty;
	Right_Duty = (Right_Duty<=(-4000)) ? (-4000):Right_Duty;
  Moto_R(Right_Duty);
	
}	

/**
  * @brief  电机外设初始化
  * @param  None
  * @retval None
  */
void motor_init(void)
{
		pwm_init(PWM_R, 17000, 0); //初始化PWM1  使用P60引脚  初始化频率为17Khz
		pwm_init(PWM_L, 17000, 0); //初始化PWM2  使用P62引脚  初始化频率为17Khz
	
		gpio_mode(P6_4, GPO_PP);    //推挽输出
		gpio_mode(P6_0, GPO_PP);    //

	//编码器初始化
	 ctimer_count_init(SPEEDL_PLUSE);	//初始化定时器0作为外部计数
	 ctimer_count_init(SPEEDR_PLUSE);	//初始化定时器3作为外部计数
}


/**
  * @brief  获取编码器的值，编码器初始化
  * @param  None
  * @retval None
  */
void Get_Encoder(void)
{
//读取采集到的编码器脉冲数
     Left_Encoder = ctimer_count_read(SPEEDL_PLUSE);
		 Right_Encoder = ctimer_count_read(SPEEDR_PLUSE);
	
//计数器清零
    ctimer_count_clean(SPEEDL_PLUSE);
		ctimer_count_clean(SPEEDR_PLUSE);

//采集方向信息
    if(1 == SPEEDL_DIR)
      Left_Encoder =  Left_Encoder;
      else
      Left_Encoder = -Left_Encoder;
		if(1 == SPEEDR_DIR)
      Right_Encoder =-Right_Encoder;
    else
      Right_Encoder = Right_Encoder;
	
//	 Average_Encoder = (Left_Encoder + Right_Encoder)*0.5;
//   distance += (Average_Encoder * 0.07);  //转化为距离  distance=600<——>距离1米
	
}
/**
  * @brief  左轮控制
  * @param  motor1占空比
  * @retval None
  */
void Moto_L(int16 moto1)
{
	if(moto1!=0)
		{
    	if(moto1<0)				AIN_L=1;
			else 	          	  AIN_L=0;
		}
		pwm_duty(PWM_L,myabs(moto1));
}
/**
  * @brief  右轮控制
  * @param  右轮输出占空比
  * @retval None
  */
void Moto_R(int16 moto2)
{
	if(moto2!=0)
		{
		  if(moto2<0)	 AIN_R=1;
			else         AIN_R=0;
		}
		pwm_duty(PWM_R,myabs(moto2));
		
}

void set_pwm(int16 moto1,int16 moto2)
{
  Moto_L(moto1);
	Moto_R(moto2);
}
