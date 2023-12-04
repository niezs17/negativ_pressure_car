#ifndef __ADC_Sovle_H
#define __ADC_Sovle_H


extern float  Left_Adc,Right_Adc,Mid_Adc,Left_Xie_Adc,Right_Xie_Adc;//���ֵ
extern float  adc_max[4];               //���ֵ
extern float  adc_min[4];               //��Сֵ  (����һ����Ҫ)
extern uint16 adc_date[4];
extern float temp;
extern uint8 Adc_Solve_Left_IN;
extern uint8 Adc_Solve_Right_IN;
extern uint8 Adc_Solve_Stright_Out;


void normalize_date();
uint16 Final_Average_Filter(uint16 *Date);
void adc_Init_all();
void normalize_date();
void ADC_Date_Fitier();
float Cha_BI_He(float Date_1,float Date_2);
void Adc_Element_Solve();


#endif