#ifndef __ADC_Sovle_H
#define __ADC_Sovle_H

//均值滤波器阶数为5
#define FILTER_N 5

//ADC电磁信号引脚4个电感（实际上都可以，电感，根据需求修改即可）
#define UPPER_LEFT_ADC_PIN       ADC_P01
#define UPPER_RIGHT_ADC_PIN      ADC_P00  
#define LOWER_LEFT_ADC_PIN       ADC_P06
#define LOWER_RIGHT_ADC_PIN      ADC_P05 
//#define Mid_ADC_Pin        ADC_P05 


extern float upper_left_adc;
extern float upper_right_adc;
extern float lower_left_adc;
extern float lower_right_adc;

extern float  adc_max[4];             
extern float  adc_min[4];             
extern uint16 adc_data[4];


extern float temp;

extern uint8 Adc_Solve_Left_IN;
extern uint8 Adc_Solve_Right_IN;
extern uint8 Adc_Solve_Stright_Out;


uint16 Final_Average_Filter(uint16 *data);
void adc_Init_all();
void normalize_data();
void ADC_data_Filter();

void Adc_Element_Solve();


#endif