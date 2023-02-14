#include "Data_P.h"
#include "usart2.h"
#include "math.h"
#include "LWM2M.h"
#define ju_max_num 7   
//#define zuhe_1_max 1
//#define zuhe_2_max 3
//#define zuhe_3_max 7
//#define zuhe_4_max 15
//#define zuhe_5_max 31
//#define zuhe_6_max 63
//#define zuhe_7_max 127
#define zuhe_max 127
 uint8_t elec_result=0;
 extern char learn_num;
extern char learn_text;
mea_val  current_val = {0,0,0};  //当前测量值

//存储的电器库
elec_app elec_app1={223.72,0,142.88,"off"}; //elec_app elec_app1={223.72,0,142.88,"off"}; 
elec_app elec_app2={0,0,0,"off"};  //elec_app elec_app2={223.72,0,28.20,"off"};
elec_app elec_app3={0,0,0,"off"};
elec_app elec_app4={0,0,0,"off"};
elec_app elec_app5={0,0,0,"off"};
elec_app elec_app6={0,0,0,"off"};
elec_app elec_app7={0,0,0,"off"};


uint8_t elec_jude (void)
{
	int ElectricalSta[ju_max_num]={0};	  //ElectricalSta[i]为当前穷举项
	//int len=ju_max_num;
	int i=0;
	float p_sta[ju_max_num]={0};				
	float p_sum=0;			
	//float current;
//	float power[7]={0};
//	power[0]=elec_app1.Pactive_pow;
//	power[1]=elec_app2.Pactive_pow;
//	power[2]=elec_app3.Pactive_pow;
//	power[3]=elec_app4.Pactive_pow;
//	power[4]=elec_app5.Pactive_pow;
//	power[5]=elec_app6.Pactive_pow;
//	power[6]=elec_app7.Pactive_pow;
float  power[7]={elec_app1.Pactive_pow,elec_app2.Pactive_pow,elec_app3.Pactive_pow,elec_app4.Pactive_pow,elec_app5.Pactive_pow,elec_app6.Pactive_pow,elec_app7.Pactive_pow};
	int times=0,timestmp=0;
//	for( i=0;i<ju_max_num;i++)   //初始化各电器状态
//	{
//			switch(i)
//			{
//				case 0:elec_app1.sta="off"; break;
//				case 1:elec_app2.sta="off"; break;
//				case 2:elec_app3.sta="off"; break;
//				case 3:elec_app4.sta="off"; break;
//				case 4:elec_app5.sta="off"; break;
//				case 5:elec_app6.sta="off"; break;
//				case 6:elec_app7.sta="off"; break;
//	 		}
//	}
	
	for(times=0;times<zuhe_max;times++)  //穷举所有组合
	{ 
		timestmp=times;
		for(i=0;i<ju_max_num;i++)  //初始化
		ElectricalSta[i]=0;   
		i=0;
		
		while(timestmp)    //转化成二进制
		{
			ElectricalSta[i]=timestmp%2;   
			timestmp=timestmp/2;
			i++;
		}

		for(i = 0; i <ju_max_num; i ++)
		{
			p_sta[i] = ElectricalSta[i]*power[i];
			p_sum+=p_sta[i];   //此次穷举项的总功率
		}

		
		if(fabsf(p_sum-current_val.Pactive_pow)<5.0)  //与当前测量值对比
		{
			p_sum=0;
			return times; //如果判断有，则返回各电器状态,跳出循环
		}
//	else if(fabsf(current_val.Pactive_pow-p_sum)<5.0)
//	{
//		p_sum=0;
//		return times; //如果判断有，则返回各电器状态,跳出循环
//	}
		p_sum=0;
	
		
	}
	return 0;
}

void Elec_learn(char learn_number, float vol, float power)
{
		switch(learn_number)
		{
			case '1':
						elec_app1.voltage=0;
						elec_app1.Pactive_pow=0;
						elec_app1.voltage= vol;
						elec_app1.Pactive_pow=power;
						break;
			case '2':
						elec_app2.voltage=0;
						elec_app2.Pactive_pow=0;
						elec_app2.voltage= vol;
						elec_app2.Pactive_pow=power;
						break;
			case '3':
						elec_app3.voltage=0;
						elec_app3.Pactive_pow=0;
						elec_app3.voltage= vol;
						elec_app3.Pactive_pow=power;
						break;
			case '4':
						elec_app4.voltage=0;
						elec_app4.Pactive_pow=0;
						elec_app4.voltage= vol;
						elec_app4.Pactive_pow=power;
						break;
			case '5':
						elec_app5.voltage=0;
						elec_app5.Pactive_pow=0;
						elec_app5.voltage= vol;
						elec_app5.Pactive_pow=power;
						break;
			case '6':
						elec_app6.voltage=0;
						elec_app6.Pactive_pow=0;
						elec_app6.voltage= vol;
						elec_app6.Pactive_pow=power;
						break;
			case '7':
						elec_app7.voltage=0;
						elec_app7.Pactive_pow=0;
						elec_app7.voltage= vol;
						elec_app7.Pactive_pow=power;
						break;		
		}
	 Cmd_MIPLNOTIFY_OK( learn_text,  learn_num);
}
