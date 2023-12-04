/**
  ******************************************************************************
  * @author  Pump_Li
  * @version V1.0.0
  * @date    2023.5.4
  * @brief   出入库，干簧管检测头文件
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
 #ifndef _park_h
#define _park_h


#define START_T          100
#define T_OUT_PARK1      200
#define T_OUT_PARK2      100
#define T_IN_PARK1       10
#define T_IN_PARK2       100
#define T_IN_PARK3       100
#define Reed_Switch_HALL_Pin P26

//变量声明
extern int8   flag_start;          //出库完毕
extern int8   flag_open_reed;      //屏蔽干簧管
extern int8   reed_state;          //干簧管状态
extern int8   flag_end;            //开始停车标志位
extern uint16 T_outku;
extern uint16 T_inku;
extern uint16 Open_pack_time;
extern uint8 stop_flag_IN;

//函数申明
extern void Handle_Barn_Out(uint8 type);
extern void HALL_Detect(void);
extern void Handle_Barn_In(uint8 type);

#endif