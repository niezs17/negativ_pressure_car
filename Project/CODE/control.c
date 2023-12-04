/**
  ******************************************************************************
  * @author  Pump_Li
  * @version V1.0.0
  * @date    2023.3.27
  * @brief   差速串级pid控制
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "headfile.h"
/* Private Define ------------------------------------------------------------*/
float speed_output=0;
float Position_Get,Position_pre_Get,Position_Temp,Position_XIE_Temp;
uint8 COUNTS=0;     //内外环计数器
float target_vel_z=0;  //目标角度
float target_vel_z_pre=0;
float my_gyro_z;
float ZX_kp=0.0;    //转向环PID（内环）
float ZX_ki=0.0;//float ZX_ki=0.0032;
float ZX_kd=0;
float Kp_angel_out;//float Kp_angel_out=90;
float Ki_angel_out=0;
float Kd_angel_out=10;       //外环pid
float price_Velocite=0;  //转向输出差速
uint8 stop_flag=0;
uint8 fan_start_flag=0; //负压开启标志
/* Private Functions ---------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
/**
  * @brief  电机差速控制
  * @param  float speed
  * @retval None
  */
void motor_speed(float speed)
{
//	left_motor_pid(speed+price_Velocite);
//  right_motor_pid(speed-price_Velocite);
  left_motor_pid(speed);
  right_motor_pid(speed);
}
/**
  * @brief  速度输出，反二次型
  * @param  float a,b
  * @retval None
  */
void set_speed_target(float a,float b)
{
  Position_Get = Cha_BI_He(Left_Xie_Adc,Right_Xie_Adc);
//  Position_Temp = Cha_BI_He(Left_Adc,Right_Adc);
//  Position_XIE_Temp = Cha_BI_He(Left_Xie_Adc,Right_Xie_Adc);
//  Position_Get = (1-abs_adc(Position_Temp))*Position_Temp + abs_adc(Position_Temp)*Position_XIE_Temp;
 // if(flag_obstacle=1)Position_Get = Position_TOF;//路障
  speed_output = -a*Position_Get*Position_Get + b;

  Kp_angel_out = 400*Position_Get*Position_Get + 200;//70,20

    //Kp_angel_out =  1000*Position_Get*Position_Get + 120;//70,20
  K_diff =0.5*target_vel_z*0.2*(2*3.1415926/360);
  if(K_diff<=speed_output && K_diff>=(-speed_output))K_diff = K_diff;
  else if (K_diff>speed_output)K_diff=speed_output;
  else if (K_diff<(-speed_output))K_diff=(-speed_output);
  motor_speed(speed_output);
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void DifferControl(){
//转向外环50ms,转向内环10ms
	//三次内环，执行一次内环
//  if(COUNTS==5){
//    COUNTS=0;
    DerectError_Get();
//  }
//  COUNTS++;
  my_gyro_z = imu660ra_gyro_transition(imu660ra_gyro_z);
  if(my_gyro_z<0.5 && my_gyro_z>(-0.5))
    my_gyro_z = 0;    //陀螺仪死区
  //target_vel_z=target_vel_z_pre+(target_vel_z-target_vel_z_pre)*COUNTS/5;
  //Direction_out();
}

void DerectError_Get(void)  //外环
{ 
    //target_vel_z=Kp_angel_out*Position_Get+Kd_angel_out*(Position_Get-Position_pre_Get);
    target_vel_z=Kp_angel_out*Position_Get+my_gyro_z;//陀螺仪
   //target_vel_z=20; //开启这句话，表转向环只有内环，外环输出为0
     Position_pre_Get=Position_Get;
     target_vel_z_pre=target_vel_z;
}

/**
  * @brief  角速度内环，串级
  * @param  None
  * @retval None
  */
void Direction_out(void)
{
  float error;
  static  float last_error;
  float price_Differention;
  float price_Intergration;
  float price_Proportion;
  static float Pre1_Error[4];
  float Direct_Parameter;
  static float sum;
	/*直线环，如果左摇右摆调内环KP、KD*/
  error = target_vel_z-my_gyro_z;  
	//转向PID控制
  sum = error++;
  price_Proportion=ZX_kp * error;
  price_Intergration=ZX_ki * sum;
  price_Differention= ZX_kd* (error - last_error);
  
  Direct_Parameter =price_Proportion  + price_Differention + price_Intergration;
  
  last_error =  error; 
  Pre1_Error[3]=Pre1_Error[2];
  Pre1_Error[2]=Pre1_Error[1];
  Pre1_Error[1]=Pre1_Error[0];
  Pre1_Error[0]=Direct_Parameter;
  Direct_Parameter=Pre1_Error[0]*0.8+Pre1_Error[1]*0.1+Pre1_Error[2]*0.06+Pre1_Error[3]*0.04 ;
  price_Velocite= - Direct_Parameter;
	//转向差速速度限制幅度
 if(price_Velocite>=1) price_Velocite=1;
  if(price_Velocite<=-1) price_Velocite=-1;
 
}
/**
  * @brief  开启负压
  * @param  None
  * @retval None
  */
void fan_start()
{
      pwm_duty(PWMB_CH3_P33,700);
      pwm_duty(PWMB_CH4_P77,700);
}
/**
  * @brief  出赛道保护关闭总中断
  * @param  None
  * @retval None
  */
void Out_protect(void)
{
	if(Left_Adc<10&&Right_Adc<10)
	{
		DisableGlobalIRQ();//关闭总中断
		motor_speed(0);
	}
	else
	{
		EnableGlobalIRQ();
	}
}