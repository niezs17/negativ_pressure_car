/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"


/*
 * 系统频率，可查看board.h中的 FOSC 宏定义修改。
 * board.h文件中FOSC的值设置为0,则程序自动设置系统频率为33.1776MHZ
 * 在board_init中,已经将P54引脚设置为复位
 * 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */
void main()
{
  DisableGlobalIRQ();     																	//关闭总中断
	board_init();			// 初始化寄存器,勿删除此句代码。
	
	// 此处编写用户代码(例如：外设初始化代码等)
	//lcd_init();
  motor_init();
  imu660ra_init();
  while(dl1a_init())
	{
		delay_ms(500);
		printf("g:VL53L0X init try again.\r\n");
	}
//  servo_init();
  adc_Init_all();
  //pit_timer_ms(TIM_4,1);	   //定时器4  2ms中断
   bluetooth_ch9141_init();
  pwm_init(PWMB_CH3_P33,50,0);       //频率为50hz无刷电机初始化
  pwm_init(PWMB_CH4_P77,50,0);       
    if(fan_start_flag==0)   //等待无刷风扇开启
    {
    delay_ms(4000);
    fan_start_flag=1;
    }
  pit_timer_ms(TIM_1,1);	   //定时器1  5ms中断
	EnableGlobalIRQ();                                         //初始化完毕，开启总中断
    while(1)
	{ 
   Handle_Barn_Out(1);     //执行出库函数后flag_start标志位置一才会进中断
//flag_start=1;         //平时调车直接给1，不用出库
	 if(flag_start)
	 {
		 HALL_Detect();               //干簧管停车检测
	 }
	 Handle_Barn_In(1);             //停车入库处理
   
   printf("g:%f,%f,%f,%f,%f,%f,%f,%f,%d,%d\r\n",Left_Encoder*0.0017f*10,Right_Encoder*0.0017f*10,Right_Adc,Left_Adc,Left_Xie_Adc,Right_Xie_Adc,right_wheel_goal,left_wheel_goal,Right_Duty,Left_Duty);
   //printf("g:%f,%f,%f,%f,%d,%d\r\n",Left_Encoder*0.0017f*10,Right_Encoder*0.0017f*10,pid_Delta_L,pid_Delta_R,right_wheel_goal,left_wheel_goal,Right_Duty,Left_Duty);
    //printf("g:%d,%d,%d,%d,%d,%f,%d,%f\r\n",flag_1,flag_2,flag_3,flag_4,Out_Time,Annulus_Angle,count_delay_In,Annulus_Distance);
    //printf("g:%d,%d,%f\r\n",flag_4,Out_Time,Annulus_Angle);
    //printf("%d\r\n",fan_start_flag);
   //printf("g:%d,%d,%d\r\n",T_outku,flag_end,dl1a_distance_mm);
//   		if(dl1a_finsh_flag)
//		{
//			printf("Range : %d.\r\n", dl1a_distance_mm);
//			dl1a_finsh_flag = 0;
//		}
		 // 此处编写需要循环执行的代码
    }
}

