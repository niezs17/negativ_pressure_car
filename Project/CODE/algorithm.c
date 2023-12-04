/**
  ******************************************************************************
  * @author  Pump_Li
  * @version V1.0.0
  * @date    2023.3.13
  * @brief   一些基本控制算法
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "headfile.h"
/* Private Define ------------------------------------------------------------*/
uint8 annulus_flag=0;           //环岛标志位
uint8 annulus_success=0;        //进环成功次数计数
uint8 Round_Start =0;           //圆环编码器、陀螺仪积分标志位
uint8 Adc_Solve_annulus=0;
uint8 Left_Annulus=0;           //左环岛标志位
uint8 Right_Annulus=0;          //右环岛标志位
float Annulus_Angle = 0;            //积分出圈角度
float Average_Encoder = 0;  //左右编码器平均值
float Annulus_Distance = 0;         //车子行走的路程

uint16 Out_Time=0;//出环延时时间
uint32 T_annulus=0;//入环时间
uint16 T_Clear=60;//出环清标志位的时间，以防二次识别

uint8 flag_1=0;
uint8 flag_2=0;
uint8 flag_3=0;
uint16 flag_4=0;

uint16 count_delay_In=0;


uint8 flag_obstacle=0;//路障标志位
uint8 temp_TOF=0;
float Position_TOF=0;
float obstacle_Current_Dir[]={
                              -0.6,-0.57,-0.54,-0.51,-0.48,-0.45,-0.42,-0.39,-0.36,-0.33,
                              -0.30,-0.27,-0.24,-0.21,-0.18,-0.15,-0.12,-0.09,-0.06,-0.03,0,
                              0.03,0.06,0.09,0.12,0.15,0.18,0.21,0.24,0.27,0.30,
                              0.33,0.36,0.39,0.42,0.45,0.48,0.51,0.54,0.57,0.6
                             };


/* Private Functions ---------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**
  * @brief  绝对值函数
  * @param  None
  * @retval None
  */
int myabs(int16 a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

float abs_adc(float adc)
{
  	 float temp;
		if(adc<0)  temp=-adc;  
	  else temp=adc;
	  return temp;
}

/**
  * @brief  环岛元素识别标志位，环岛分为四段控制，1、预测环岛距离积分到转向点2、转向点固定打角进入环岛3、环岛内恢复正常循迹4、陀螺仪积分出环岛//分别对应annulus_flag
  * @param  None
  * @retval None
  */
void element_annulus_detect(){
  ////------------------预测圆环------------------1-----------------------//
  
	if((Left_Xie_Adc+Right_Xie_Adc)>300&&annulus_flag==0)        //左环岛标志位检测
	{
		//Buzz = 1;
    annulus_flag=1;//环岛标志位
    //Left_Annulus=1; //左环岛标志
	}
//  	if(Right_Xie_Adc>200&&Left_Xie_Adc>200&&annulus_flag==0)        //右环岛标志位检测
//	{
//		//Buzz = 1;
//    annulus_flag=1;//环岛标志位
    //Right_Annulus=1;//右环岛标志
//	} 
//  	if((Left_Adc-Right_Adc)>100&&(Left_Xie_Adc-Right_Xie_Adc)>0&&annulus_flag==0)        //左环岛标志位检测
//	{
//		//Buzz = 1;
//    annulus_flag=1;//环岛标志位
//    Left_Annulus=1; //左环岛标志
//	}
//  	if((Right_Adc-Left_Adc)>100&&(Right_Xie_Adc-Left_Xie_Adc)>0&&annulus_flag==0)        //右环岛标志位检测
//	{
//		//Buzz = 1;
//    annulus_flag=1;//环岛标志位
//    Right_Annulus=1;//右环岛标志
//	} 

//----------圆环入---------------------------------2---------------------//
//	 if(annulus_flag==1)    
//	{
//		Round_Start =1;   //编码器积分标志位
//    flag_1=1;
//    if((Left_Adc>250||Right_Adc>250)&&annulus_flag==1)//&&Annulus_Distance>60
//    {
//     annulus_flag = 2; //走到圆环转弯处标志位，在此处打角
//    }
//  }
  	 if(annulus_flag==1)    
	{
		Round_Start =1;   //编码器、陀螺仪积分标志位
    flag_1=1;
    count_delay_In++;
//    if((Right_Adc>250&&Right_Xie_Adc>300&&Left_Adc<50&&Left_Xie_Adc<200||Left_Adc>250&&Left_Xie_Adc>300&&Right_Adc<50&&Right_Xie_Adc<200)&&annulus_flag==1&&count_delay_In>10)//&&Annulus_Distance>60
//    {
//     annulus_flag = 2; //走到圆环转弯处标志位，在此处打角
//    }
      if(Left_Adc>500&&annulus_flag==1&&Annulus_Distance>=10&&count_delay_In>50)//&&Annulus_Distance>60
    {
     annulus_flag = 2; //走到圆环转弯处标志位，在此处打角
      Left_Annulus=1; //左环岛标志
    } 
      if(Right_Adc>300&&annulus_flag==1&&Annulus_Distance>=10&&count_delay_In>50)//&&Annulus_Distance>60
    {
     annulus_flag = 2; //走到圆环转弯处标志位，在此处打角
     Right_Annulus=1;//右环岛标志
    } 
  }
  if(annulus_flag==2)
		{
     T_annulus++;      //进去时间控制
      //flag_2=1;
							//如果打角过早的话加大T_annulus的下限，如果打角过迟的话就减小下限，或采用陀螺仪控制，尝试先用时间控制
						if(T_annulus>0&&T_annulus<100) //控制固定打角的时间0---50
						{
             
												if(Left_Annulus==1)  //左环
												{
													Adc_Solve_annulus=1;  //强行向左打角左环进入
                          flag_2=1;
                         
												}
												else if(Right_Annulus==1) //右环
												{
													Adc_Solve_annulus=2; //强行向右打角右环进入
                          flag_2=1;
                         
												}
            }
//            else if(Annulus_Distance<300)
//            {
//              annulus_flag=3;//环岛中恢复正常循迹
//            }
            else if(T_annulus>=100)
            {
             annulus_flag=3;//环岛中恢复正常循迹
            }
						
		}
if(annulus_flag==3)
{
             Adc_Solve_annulus=3;
             flag_3=1;
}
  
//---------出环处理-----------------------------------3-------------------//
	if(annulus_flag==4)
	{
      if (Out_Time>0&&Out_Time<50)
      {
      Adc_Solve_annulus=4;//强行直线离开避免再次识别
      annulus_flag = 0;
        flag_4=1;
      }
      else if(Out_Time>=50)
      {
        annulus_success++;
        annulus_flag=0;
        Round_Start=0;
        T_annulus=0;
        Out_Time=0;
        Adc_Solve_annulus=0;
        count_delay_In=0;
        flag_1=0;
        flag_2=0;
        flag_3=0;
        flag_4=0;
        Annulus_Angle=0;
        Annulus_Distance=0;
      }
	}
   
/********进环过程对编码器陀螺仪等测量***************************/
	if(Round_Start ==1)
	{
      Annulus_Angle += my_gyro_z*0.00012480f*47.75;      //陀螺仪积分换算出真实角度
      Average_Encoder = (Left_Encoder + Right_Encoder)*0.05;
      Annulus_Distance += (Average_Encoder*0.08735f);  //转化为距离  distance=<——>距离1米
      if(Annulus_Angle>=330||Annulus_Angle<=(-330))     //陀螺仪判断圆环
      {
 			 Round_Start =2;//绕出来了
			 annulus_flag =4;//出环标志位
      }
//    if((Left_Adc>430||Right_Adc>430)&&annulus_flag==3)
//    {
//  		 Round_Start =2;//绕出来了
//			 annulus_flag =4;//出环标志位
//    }
	}
	
  if(Round_Start ==2)         //出环岛时间
	 { 
	 Out_Time++;
    //出环岛时间
	 }
	 
  
      if(Out_Time>T_Clear)    //所有标志位清零
   {
        //Buzz = 0;
        annulus_success++;
        annulus_flag=0;
        Round_Start =0;
        T_annulus=0;
        Out_Time=0;
     Adc_Solve_annulus=0;
     count_delay_In=0;
             flag_1=0;
        flag_2=0;
        flag_3=0;
        flag_4=0;
     Annulus_Angle=0;
     Annulus_Distance=0;
   }

}



/**
  * @brief  路障tof识别
  * @param  None
  * @retval TOF避障模块检测，使用软件模拟IIC通信，理论上任何引脚都可以使用，但是要注意不能引脚
            复用。TOF模块离障碍物越远数值越大，越近数值越小
  */
void obstacle_avoidance(void)
{
	dl1a_get_distance();                                       //距离测量
	if(dl1a_finsh_flag==1&&dl1a_distance_mm<SET_DLLA_DISTANCE) //测量距离小于设定值标志位成立
	{
		dl1a_finsh_flag=0;
		flag_obstacle=1;
	}
  		//避障误差处理
	  if(flag_obstacle==1)
		{
				 //BUZZ_ON;
				 //aim_speed=100;
				 Position_TOF = obstacle_Current_Dir[temp_TOF];
				 temp_TOF++;
				 if(temp==40)
				 {
					 temp=0;
					 flag_obstacle=0;
				   //aim_speed=400;
				   //BUZZ_OFF;
				 }
		}
}