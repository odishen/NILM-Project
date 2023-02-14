#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "Data_P.h"
#include "math.h"
#include "string.h"
#include "LWM2M.h"
#include "stdio.h"
#include "stdlib.h"
// 资源分配： 5805：active_power; 5706: 下行：learn_flag,learn_num 上行：返回命令的learn_num( is？learn over);
//           5851: judge_res（0 是无电器）
void Data_Processing(void);
u8 k=0;
u16 old_reg=0;
u8 judge_res= 0;

extern u8 tran_flag;
extern char learn_num;
extern char learn_text;
double V_current=0,C_current=0,P_active=0,E_use=0; //电压有效值，电路有效值，有功功率，已用电能

extern mea_val  current_val;
extern elec_app elec_app1;  
extern elec_app elec_app2; 
extern elec_app elec_app3; 
extern elec_app elec_app4; 
extern elec_app elec_app5; 
extern elec_app elec_app6; 
extern elec_app elec_app7; 
 int main(void)
 {	
	u8 len;
	 LED_Init();
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(9600);	 //串口1 printf
	USART2_Init(4800);   //串口2初始化为4800，与HLW8112通信
	usart3_init(9600);				//串口3与BC95通信
	 
	printf("this is uart1\r\n");
  BC35_Init();
 
//	 float se=22.124;
//	 u8 aa[20]={0};
//	 aa[0]='a';
//	 
//	 sprintf(aa+1,"%.3f",se);
//	 Uart_SendStr(aa);
//	 u8 aa[5]={'a','1','s','\r','\n'};
//	 Uart_SendStr((char*)"at+MIPLCREATE=");
//	 Uart_SendStr(aa);
//	  Cmd_MIPLNOTIFY_judge_res(  220.234 );
	while(1)
	{
		// 每次开启设备时 初始化平台资源数据(init resource)
			while(init_flag){
			Cmd_MIPLNOTIFY_float( 5805, 0 ); //init activePower 
			Cmd_MIPLNOTIFY_integer( 5851, 0 ); //init judgeRes
	 		Cmd_MIPLNOTIFY_strInit( );   //init  cmd 反馈
			delay_ms(2000);
			init_flag = 0;	//init_flag = 0 
			}
			printf("text: %c, num: %c", learn_text,learn_num);
			// printf("THIS IS NUM: %c", learn_num);
	delay_ms(300);
		
		// if(tran_flag)
		// {
		// 	Cmd_MIPLNOTIFY_float(  5805, 110.747 );
		// delay_ms(300);
		// }
		//  Cmd_MIPLNOTIFY_OK( learn_num);
        
		if(USART2_RX_STA&0x8000)
		{					   
			len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
			if(len==24&&USART2_RX_BUF[1]==0x5a)//判断数据是否为HLW8032数据
			{
				Data_Processing();//数据处理
			 	 current_val.voltage = V_current;
	      		current_val.Current = C_current;
				current_val.Pactive_pow = P_active;
				if(learn_flag)  // 学习模式
				{
					learn_flag=0;
				Elec_learn(learn_num, V_current, P_active);  //设备添加后会返回OK
				 
				}
				judge_res= elec_jude();  
				printf("结果; %u ",judge_res);
				if(judge_res!=0)
				{
					Cmd_MIPLNOTIFY_integer( 5851, judge_res );
				}
				else
				Cmd_MIPLNOTIFY_integer( 5851, 0 );
		    }
			USART2_RX_STA=0;//清零接收标志
		}
		// delay_ms(300);
	}	 
}
 


 void Data_Processing()
{
	u32 VP_REG=0,V_REG=0,CP_REG=0,C_REG=0,PP_REG=0,P_REG=0,PF_COUNT=0,PF=0;
	double V=0,C=0,P=0,E_con=0;
	if(USART2_RX_BUF[0]!=0xaa)//芯片误差修正功能正常，参数正常
	{
		VP_REG=USART2_RX_BUF[2]*65536+USART2_RX_BUF[3]*256+USART2_RX_BUF[4];//计算电压参数寄存器
		V_REG=USART2_RX_BUF[5]*65536+USART2_RX_BUF[6]*256+USART2_RX_BUF[7];//计算电压寄存器
		V=(VP_REG/V_REG)*1.88;//计算电压值，1.88为电压系数，根据所采用的分压电阻大小来确定
		V_current=V;
	printf("电压值：%0.2fV; ",V);

		
		CP_REG=USART2_RX_BUF[8]*65536+USART2_RX_BUF[9]*256+USART2_RX_BUF[10];//计算电流参数寄存器
		C_REG=USART2_RX_BUF[11]*65536+USART2_RX_BUF[12]*256+USART2_RX_BUF[13];//计算电流寄存器
		C=((CP_REG*100)/C_REG)/100.0;//计算电流值
		C_current=C;
		//printf("电流值：%0.3fA; ",C);
		//printf(" %X ",USART2_RX_BUF[0]);
		
		if(USART2_RX_BUF[0]>0xf0)//判断实时功率是否未溢出
		{
			Cmd_MIPLNOTIFY_float(5805, 0.00); 
		}
		else
		{
			PP_REG=USART2_RX_BUF[14]*65536+USART2_RX_BUF[15]*256+USART2_RX_BUF[16];//计算功率参数寄存
			P_REG=USART2_RX_BUF[17]*65536+USART2_RX_BUF[18]*256+USART2_RX_BUF[19];//计算功率寄存器
			P=(PP_REG/P_REG)*1.88*1;//计算有效功率
			P_active=P;
			//printf("有效功率：%0.2fW; ",P);
			Cmd_MIPLNOTIFY_float(5805, P_active); 
		}
		
		if((USART2_RX_BUF[20]&0x80)!=old_reg)//判断数据更新寄存器最高位有没有翻转
		{
			k++;
			old_reg=USART2_RX_BUF[20]&0x80;
		}
		PF=(k*65536)+(USART2_RX_BUF[21]*256)+USART2_RX_BUF[22];//计算已用电量脉冲数
		PF_COUNT=((100000*3600)/(PP_REG*1.88))*10000;//计算1度电对应的脉冲数量
		E_con=((PF*10000)/PF_COUNT)/10000.0;//计算已用电量
		E_use=E_con;
		//printf(" %d %d ",PF,PF_COUNT);
		//printf("已用电量：%0.4f°\r\n",E_con);
	}
	else//芯片误差修正功能失效
	{
		printf("data error\r\n");
	}
}

