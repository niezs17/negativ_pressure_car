/**
  ******************************************************************************
  * @author  Pump_Li
  * @version V1.0.0
  * @date  2023.3.13  
  * @brief   电机控制头文件
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */
#ifndef _MOTOR_H
#define _MOTOR_H

#define AIN_L P64
#define AIN_R P60
#define PWM_L PWMA_CH2P_P62
#define PWM_R PWMA_CH4P_P66

//定义脉冲引脚
#define SPEEDL_PLUSE   CTIM0_P34
#define SPEEDR_PLUSE   CTIM3_P04
//定义方向引脚
#define SPEEDL_DIR     P35
#define SPEEDR_DIR     P53


extern int16 Left_Encoder ;
extern int16 Right_Encoder ;

//extern int16 Average_Encoder;  //左右编码器平均值
//extern int16 Sum_Encoder;      //编码器脉冲和
//extern int32 distance;         //车子行走的路程
extern Right_Duty;
extern Left_Duty;
//extern float K_P ;
//extern float K_I ;
//extern float K_D ;
extern float right_wheel_goal;
extern float left_wheel_goal;
extern int16 pid_Delta_L;
extern int16 pid_Delta_R;

void motor_init(void);
void Get_Encoder(void);
void Moto_L(int16 moto1);
void Moto_R(int16 moto2);
void set_pwm(int16 moto1,int16 moto2);
void left_motor_pid(float goal);
void right_motor_pid(float goal);


#endif