#ifndef DATA_P_H_
#define DATA_P_H_
#include "sys.h"

typedef struct mea_val{
	float voltage;   //电压有效值
	float Current;	 //电流有效值		
	float Pactive_pow;		//有功功率
}mea_val; // 

typedef struct elec_app{
	float voltage;			
	float Current;					
	float Pactive_pow;	
	char	*sta;  //电器状态
}elec_app;

extern mea_val  current_val;
extern elec_app elec_app1;  
extern elec_app elec_app2; 
extern elec_app elec_app3; 
extern elec_app elec_app4; 
extern elec_app elec_app5; 
extern elec_app elec_app6; 
extern elec_app elec_app7; 

uint8_t elec_jude (void);
void Elec_learn(char learn_number, float vol, float power);

#endif
