/**
  ******************************************************************************
  * @author  Pump_Li
  * @version V1.0.0
  * @date    2023.2.13
  * @brief   算法头文件
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */
#ifndef _ALGORITHM_H
#define _ALGORITHM_H
/* Functions -----------------------------------------------------------------*/

//避障模块设定距离(根据实际情况修改)
#define SET_DLLA_DISTANCE         400
int myabs(int16 a);
float abs_adc(float adc);

void element_annulus_detect();

extern uint8 Adc_Solve_annulus;
extern float Annulus_Angle;
extern float Average_Encoder;  //左右编码器平均值
extern float Annulus_Distance;         //车子行走的路程



extern uint8 flag_1;
extern uint8 flag_2;
extern uint8 flag_3;
extern uint16 flag_4;
extern uint32 T_annulus;//入环时间
extern uint16 Out_Time;//出环延时时间
extern uint16 count_delay_In;

extern float Position_TOF;
extern uint8 flag_obstacle;//路障标志位



void obstacle_avoidance(void);

#endif