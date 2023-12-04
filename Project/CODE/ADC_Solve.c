// ADC采集工作


#include "headfile.h"
#define FILTER_N 5 
uint16 adc_date[4];
float Left_Adc=0,Right_Adc=0,Left_Xie_Adc=0,Right_Xie_Adc=0;
float  adc_max[4]={3800.0,3800.0,3800.0,3800.0};  
float  adc_min[4]={0.0,0.0,0.0,0.0};              
float Cha,He;
float temp=0;
uint8 Adc_Solve_Left_IN=0;
uint8 Adc_Solve_Right_IN=0;
uint8 Adc_Solve_Stright_Out=0;


void adc_Init_all(){
  adc_init(Left_ADC_Pin,ADC_SYSclk_DIV_2);        
  adc_init(LeftXie_ADC_Pin,ADC_SYSclk_DIV_2); 
  adc_init(RightXie_ADC_Pin,ADC_SYSclk_DIV_2); 
  adc_init(Right_ADC_Pin,ADC_SYSclk_DIV_2); 
} 


 void ADC_Date_Fitier()
{
   uint8 i;
  
  uint16 filter_buf_L[FILTER_N]; 
  uint16 filter_buf_LC[FILTER_N];
  uint16 filter_buf_R[FILTER_N]; 
  uint16 filter_buf_RC[FILTER_N];
  
 

	for(i = 0; i <FILTER_N; i++)
  {
       filter_buf_L[i]  =   adc_mean_filterLeft_ADC_Pin,ADC_12BIT,5); //
       filter_buf_LC[i] =   adc_mean_filter LeftXie_ADC_Pin,ADC_12BIT,5); 
       filter_buf_RC[i] =  adc_mean_filter RightXie_ADC_Pin,ADC_12BIT,5);  
       filter_buf_R[i]  =  adc_mean_filter Right_ADC_Pin,ADC_12BIT,5);   //
   }

  adc_date[0]= Final_Average_Filter(filter_buf_L);
  adc_date[1] =Final_Average_Filter(filter_buf_R);
  adc_date[2] = Final_Average_Filter(filter_buf_LC);
  adc_date[3] = Final_Average_Filter(filter_buf_RC);

	normalize_date();
  Adc_Element_Solve();
	 
//  Left_Adc = adc_date[0]
//  Right_Adc = adc_date[1];
//  Left_Xie_Adc = adc_date[2];
//  Right_Xie_Adc = adc_date[3];


 


}
uint16 Final_Average_Filter(uint16 *Date)
{
  uint8 i;
  uint16 max,min;
  uint16 sum=0;
  
  max = Date[0];
  min = Date[0];
  
  for(i=0;i<5;i++)
  {
       if(max<Date[i]) max = Date[i];
       if(min>Date[i]min = Date[i];
       sum += Date[i];
  }       
  sum = (sum-max-min)/(5-2);
  return sum;
}


void normalize_date()
{
  float temp_Date[4];
  uint16 i;
  for(i=0;i<4;i++)
  {
//    if(adc_date[i]<adc_min[i])
//    {
//      adc_min[i]=adc_date[i];
//    }
//   else if(adc_date[i]>adc_max[i])
//   {  
//   adc_max[i]=adc_date[i];
//   } 
     temp_Date[i] = (adc_date[i]-adc_min[i])/(adc_max[i]-adc_min[i])*500;
    if (temp_Date[i]<0)
    {
      temp_Date[i]=0.0;
    }
      else if(temp_Date[i]>500.0)
      {
      temp_Date[i]=500.0;
      }
      }
  Left_Adc = temp_Date[0];
  Right_Adc = temp_Date[1];
  Left_Xie_Adc = temp_Date[2];
  Right_Xie_Adc = temp_Date[3];
}







//int16 Cha_BI_He_Cha(uint16 Date_1,uint16 Date_2,uint16 Date_3,uint16 Date_4,float a,float b,uint16 X)
//{
//  int16 Resault;
//	int16 FengZ,FengM;
//  Cha_H = int16)((Date_1 - Date_2));  
//	Cha_X = int16)((Date_3 - Date_4));  
//	
//  He =   Date_1 + Date_2; 
//	Sum = Date_1 + Date_2 + Date_3 + Date_4;  
//	He_X = Date_3 + Date_4;
//	
//  FengZ = a*Cha_H+b*Cha_X;  
//	FengM = a*He+b*myabs(Cha_X);
//	Resault = ((FengZ<<X)/(FengM+1)); 
//  return   Resault;
//}



//int16 I_Median_Average_Filter(int16 *DATE)
//{
//    uint8 i;
//    int16 max,min;  
//    int16 sum = 0;

//    max = DATE[0];
//    min = DATE[0];

//    for(i=0;i<sizeof(DATE);i++)
//    {
//        if(max<DATE[i]max = DATE[i];
//        if(min>DATE[i]min = DATE[i];
//        sum += DATE[i];
//    }

//    sum =(sum-max-min)/(sizeof(DATE)-2);    //>>3
//    return sum;
//}

float Cha_BI_He(float Date_1,float Date_2)
{
  float Resault;
  Cha =  (Date_1 - Date_2);   
  He =   Date_1 + Date_2;   
  //Resault =Cha<<X/(He+1)) ; 
  Resault = Cha / (He+1);


  return   Resault;
}

void Adc_Element_Solve()
{
  switch(Adc_Solve_annulus){         
    case 1:                          
        Left_Xie_Adc = 140;
        Right_Xie_Adc =40;        
       break;
    case 2:                          
        Left_Xie_Adc = 40;
        Right_Xie_Adc =140;
    break;
    case 3:
        Left_Xie_Adc =Left_Adc;
        Right_Xie_Adc = Right_Adc;
    break;
    case 4:                           
        Left_Xie_Adc = Right_Xie_Adc;  
    break;
    default:
     break; 
  
  }
}