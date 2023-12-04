/**
  ******************************************************************************
  * @author  Pump_Li
  * @version V1.0.0
  * @date    2023.5.4
  * @brief   出入库，干簧管检测
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */
/*全局变量定义：

flag_start: 出库完成标志位，初始值为0。
flag_open_reed: 打开干簧管标志位，初始值为0。
reed_state: 干簧管状态，初始值为0。
flag_end: 开始停车标志位，初始值为0。
T_outku, T_inku: 出库和入库的定时器，用于控制出库和入库的时间。
Open_pack_time: 打开干簧管的定时器。
Stop_time: 停车的定时器。
stop_flag_IN: 停车标志位，初始值为0。
出库函数 Handle_Barn_Out：

通过控制左右电机的速度，实现小车的出库动作。
使用定时器 T_outku 控制出库过程中不同阶段的速度。
干簧管检测函数 HALL_Detect：

在出库过程中，当小车走过一段距离后开启干簧管检测。
通过读取 Reed_Switch_HALL_Pin 的状态来获取干簧管的状态。
如果干簧管检测到磁场变化（状态为1），则设置 flag_end 为1，表示开始停车。
入库函数 Handle_Barn_In：

控制小车进行入库动作，包括停车和改变方向。
使用定时器 T_inku 控制入库过程中不同阶段的速度。
stop_flag_IN 用于标志停车状态。
注意事项：

代码中涉及的具体数值，如速度、时间等，可能需要根据实际机械和电气系统的性能进行调整。
部分变量，如 Reed_Switch_HALL_Pin，应该是由外部定义的引脚状态。
*/


 /* Includes ------------------------------------------------------------------*/
 #include "headfile.h"
/* Private Define ------------------------------------------------------------*/
int8   flag_start           = 0;    //出库完成标志位
int8   flag_open_reed       = 0;    //打开干簧管
int8   reed_state           = 0;    //干簧管状态
int8   flag_end             = 0;    //开始停车标志位
uint16 T_outku              = 0;    //出库直走和打角定时
uint16 T_inku               = 0;    //入库停车和打角定时
uint16 Open_pack_time = 0;   //打开干簧管定时
uint16 Stop_time = 0;        //停车定时
uint8 stop_flag_IN = 0;  //停车标志位
  /* Functions -----------------------------------------------------------------*/
  /* @brief  出库函数
     @param  type-----1为左出库，2为右出库
     @retval None
  */
void Handle_Barn_Out(uint8 type)
{//1为左出库，2为右出库
    if(type == 1)
    {
		 if(!flag_start)
		 {
      if(T_outku < T_OUT_PARK1)
      {
      Moto_L(2000);
      Moto_R(2000);
      }
			 if (T_outku >= T_OUT_PARK1) 
			 {
      Moto_L(1000);
      Moto_R(3000);
			 }
			 if(T_outku > T_OUT_PARK2)
			 {
				  flag_start = 1;
				  //T_outku=0;
			 }		
		  }	
	  }
		if(type ==2)
    {
			if(!flag_start)
			{
      if(T_outku < T_OUT_PARK1)
      {
//     Moto_L(2000);
//     Moto_R(2000);
      }
			 if (T_outku >= T_OUT_PARK1) 
			 {
				 
//     Moto_L(2000);
//     Moto_R(2000);
			 }
			 if(T_outku > T_OUT_PARK2)
			 {
				  flag_start = 1;
				  //T_outku=0;
			 }															
			}
    }
}

/**
  * @brief  干簧管检测停车
  * @param  None
  * @retval 干簧管使用方法就和按键类似，通过读取IO口电平即可
  */
void HALL_Detect(void)
{
	if(flag_start)//开始时不开启干簧管检测，防止出库时误测
	{
		//走过一段距离后开启干簧管检测
		if(T_outku > START_T)
		{
			flag_open_reed = 1;
			T_outku=0;
		}
	}
	if(flag_open_reed)             //干簧管检测标志位成立后才开始检测
	{
		reed_state = Reed_Switch_HALL_Pin;//干簧管状态
		if(reed_state==1)
		{
			flag_end = 1;              //识别到停车标志位开启
		}
	 }
}

/**
  * @brief  入库函数
  * @param  None
  * @retval None
  */
void Handle_Barn_In(uint8 type)
{
	if(type ==1)
  {
		if(flag_end==1)
		{
        stop_flag_IN = 1;
        motor_speed(0);
			 if (T_inku >= T_IN_PARK1) 
			 {
         stop_flag = 0;
        Moto_L(-2000);
        Moto_R(-2000);
			 }
        if(T_inku >= T_IN_PARK2)
			 {
        Moto_L(-1000);
        Moto_R(-3000);
			 }
        if(T_inku >= T_IN_PARK3)
			 {
        stop_flag_IN = 1;
        Moto_L(0);
        Moto_R(0);
        while(1);
			 }
       
		}
 }
	if(type ==2)
  {
		if(flag_end==1)
		{
        stop_flag_IN = 1;
        motor_speed(0);
			 if (T_inku >= T_IN_PARK1) 
			 {
        stop_flag_IN = 0;
        Moto_L(-2000);
        Moto_R(-2000);
			 }
        if(T_inku >= T_IN_PARK2)
			 {
        Moto_L(-3000);
        Moto_R(-1000);
			 }
        if(T_inku >= T_IN_PARK3)
			 {
        stop_flag_IN = 1;
        Moto_L(0);
        Moto_R(0);
        while(1);
			 }
		}
 }
	
}