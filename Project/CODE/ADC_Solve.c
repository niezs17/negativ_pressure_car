#include "headfile.h"


//  建立一个四位数组，存储4路adc的滤波结果
uint16 adc_data[4];

/*
    定义用于电磁检测的电容信号，信号经过放大器放大后，由ADC产生
    用到了四个分别位于，
    upper_left_adc    --->   左上方    --->    L1(upper)   --->  引脚: 0.1 
    upper_right_adc   --->   右上方    --->    L7(upper)   --->  引脚: 0.0
    lower_left_adc    --->   左下方    --->    L4(lower)   --->  引脚: 0.6 
    lower_right_adc   --->   右下方    --->    L1(lower)   --->  引脚: 0.5 
*/
float upper_left_adc=0;
float upper_right_adc=0;
float lower_left_adc=0;
float lower_right_adc=0;

/*
    定义ADC最大，最小值
*/
float  adc_max[4]={3800.0,  3800.0,  3800.0, 3800.0};  
float  adc_min[4]={   0.0,     0.0,     0.0,    0.0};  


uint8 Adc_Solve_Left_IN        =0;
uint8 Adc_Solve_Right_IN       =0;
uint8 Adc_Solve_Stright_Out    =0;

/*
    初始化ADC
*/
void adc_Init_all(){
  adc_init(upper_left_adc, ADC_SYSclk_DIV_2); 
  adc_init(upper_right_adc, ADC_SYSclk_DIV_2); 
  adc_init(lower_left_adc, ADC_SYSclk_DIV_2);        
  adc_init(lower_right_adc, ADC_SYSclk_DIV_2); 
} 

//  建立一个ADC采集数据均值滤波器，大小为5个采样点，每5个点取一次平均值，作为ADC最终采集到的有效数字
void ADC_Date_Filter()
{
    uint8 i;
    uint16 filter_buf_lower_left[FILTER_N];
    uint16 filter_buf_upper_left[FILTER_N];
    uint16 filter_buf_lower_right[FILTER_N];
    uint16 filter_buf_upper_right[FILTER_N];

    //经过平均滤波器
    for (i = 0; i < FILTER_N; i++)
    {
        filter_buf_upper_left[i]  = adc_mean_filter(UPPER_LEFT_ADC_PIN,  ADC_12BIT, 5);
        filter_buf_upper_right[i] = adc_mean_filter(UPPER_RIGHT_ADC_PIN, ADC_12BIT, 5);
        filter_buf_lower_left[i]  = adc_mean_filter(LOWER_LEFT_ADC_PIN,  ADC_12BIT, 5);
        filter_buf_lower_right[i] = adc_mean_filter(LOWER_RIGHT_ADC_PIN, ADC_12BIT, 5);
    }

    //存入结果数组
    adc_data[0] = Final_Average_Filter(filter_buf_lower_left);
    adc_data[1] = Final_Average_Filter(filter_buf_lower_right);
    adc_data[2] = Final_Average_Filter(filter_buf_upper_left);
    adc_data[3] = Final_Average_Filter(filter_buf_upper_right);

    //归一化缓存区
    normalize_data();
    Adc_Element_Solve();
}

uint16 Final_Average_Filter(uint16 *data)
{
    uint8 i;
    uint16 max, min;
    uint16 sum = 0;

    max = data[0];
    min = data[0];

    for (i = 0; i < 5; i++)
    {
        if (max < data[i])
            max = data[i];
        if (min > data[i])
            min = data[i];
        sum += data[i];
    }

    sum = (sum - max - min) / (5 - 2);
    return sum;
}

void normalize_data()
{
    float temp_data[4];
    uint16 i;

    for (i = 0; i < 4; i++)
    {
        temp_data[i] = (adc_data[i] - adc_min[i]) / (adc_max[i] - adc_min[i]) * 500;

        if (temp_data[i] < 0)
        {
            temp_data[i] = 0.0;
        }
        else if (temp_data[i] > 500.0)
        {
            temp_data[i] = 500.0;
        }
    }

    lower_left_adc = temp_data[0];
    lower_right_adc = temp_data[1];
    upper_left_adc = temp_data[2];
    upper_right_adc = temp_data[3];
}

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


//uint8 Adc_Solve_annulus 的定义在 algorithm.c

//在不同情况下强制选择adc输出待控制量
void Adc_Element_Solve()
{
    switch (Adc_Solve_annulus)
    {
    case 1:
        upper_left_adc = 140;
        upper_right_adc = 40;
        break;
    case 2:
        upper_left_adc = 40;
        upper_right_adc = 140;
        break;
    case 3:
        upper_left_adc = lower_left_adc;
        upper_right_adc = lower_right_adc;
        break;
    case 4:
        upper_left_adc = upper_right_adc;
        break;
    default:
        break;
    }
}
