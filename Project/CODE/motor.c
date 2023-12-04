#include "headfile.h"
#include "motor.h"

int16 Left_Encoder = 0;     //左编码器的值
int16 Right_Encoder = 0;    //右编码器的值
int16 Average_Encoder = 0;  //左右编码器平均值
int16 Sum_Encoder = 0;      //编码器脉冲和
int32 distance = 0;         //车子行走的路程

int16 K_diff = 0 ;
//int16 Left_Encoder = 0;


float K_P = 70;
float K_I = 8;
float K_D = 0;

int16 Right_Duty,Left_Duty;

void left_motor_pid(int16 goal)
{
  static int16 error,last_error=0,last_last_error=0,Goal_speed;
	Goal_speed = (int16)((1-K_diff)*goal);
//	Goal_speed = goal;
	error = Goal_speed - Left_Encoder;
	Left_Duty+=K_P*(error-last_error)+K_I*error+K_D*(error-2*last_error+last_last_error);
	
  last_error = error;
	last_last_error = last_error;
	
//	Left_Duty -=  position*0.1;
	Left_Duty = Left_Duty>8000 ? 8000:Left_Duty;
	Left_Duty = Left_Duty<-3000? -3000:Left_Duty;
	
	
  Moto_L(Left_Duty);
	
}	

void right_motor_pid(int16 goal)
{
  static int16 error,last_error=0,last_last_error=0,Goal_speed;
	Goal_speed = (1+K_diff)*goal;
//	Goal_speed = goal;
	error = Goal_speed - Right_Encoder;
	Right_Duty+=K_P*(error-last_error)+K_I*error+K_D*(error-2*last_error+last_last_error);
	
	
  last_error = error;
	last_last_error = last_error;
	
//	Right_Duty +=  position*0.1;
	
	Right_Duty = Right_Duty>8000 ? 8000:Right_Duty;
	Right_Duty = Right_Duty<-3000 ? -3000:Right_Duty;
	
  Moto_R(Right_Duty);
	
}	


void motor_init(void)
{
		pwm_init(PWM_R, 10000, 0); //初始化PWM1  使用P60引脚  初始化频率为10Khz
		pwm_init(PWM_L, 10000, 0); //初始化PWM2  使用P62引脚  初始化频率为10Khz
	
		gpio_mode(P6_4, GPO_PP);
		gpio_mode(P6_0, GPO_PP);

	//编码器初始化
	 ctimer_count_init(SPEEDL_PLUSE);	//初始化定时器0作为外部计数
	 ctimer_count_init(SPEEDR_PLUSE);	//初始化定时器3作为外部计数
}

//获取编码器的值
void Get_Encoder(void)
{
//读取采集到的编码器脉冲数
     Left_Encoder = ctimer_count_read(SPEEDL_PLUSE);
		 Right_Encoder = ctimer_count_read(SPEEDR_PLUSE);
	
//计数器清零
    ctimer_count_clean(SPEEDL_PLUSE);
		ctimer_count_clean(SPEEDR_PLUSE);

//采集方向信息
    if(1 == SPEEDL_DIR)    Left_Encoder = Left_Encoder;
    else                   Left_Encoder = -Left_Encoder;
		if(1 == SPEEDR_DIR)    Right_Encoder =-Right_Encoder;
    else                   Right_Encoder = Right_Encoder;
	
	 Average_Encoder = (Left_Encoder + Right_Encoder)*0.5;
   distance += (Average_Encoder * 0.07);  //转化为距离  distance=600<——>距离1米
	
}

void Moto_L(int16 moto1)
{
	if(moto1!=0)
		{
    	if(moto1<0)				AIN_L=1;
			else 	          	AIN_L=0;
		}
		pwm_duty(PWM_L,myabs(moto1));
}

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




