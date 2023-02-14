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
// ��Դ���䣺 5805��active_power; 5706: ���У�learn_flag,learn_num ���У����������learn_num( is��learn over);
//           5851: judge_res��0 ���޵�����
void Data_Processing(void);
u8 k=0;
u16 old_reg=0;
u8 judge_res= 0;

extern u8 tran_flag;
extern char learn_num;
extern char learn_text;
double V_current=0,C_current=0,P_active=0,E_use=0; //��ѹ��Чֵ����·��Чֵ���й����ʣ����õ���

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
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(9600);	 //����1 printf
	USART2_Init(4800);   //����2��ʼ��Ϊ4800����HLW8112ͨ��
	usart3_init(9600);				//����3��BC95ͨ��
	 
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
		// ÿ�ο����豸ʱ ��ʼ��ƽ̨��Դ����(init resource)
			while(init_flag){
			Cmd_MIPLNOTIFY_float( 5805, 0 ); //init activePower 
			Cmd_MIPLNOTIFY_integer( 5851, 0 ); //init judgeRes
	 		Cmd_MIPLNOTIFY_strInit( );   //init  cmd ����
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
			len=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			if(len==24&&USART2_RX_BUF[1]==0x5a)//�ж������Ƿ�ΪHLW8032����
			{
				Data_Processing();//���ݴ���
			 	 current_val.voltage = V_current;
	      		current_val.Current = C_current;
				current_val.Pactive_pow = P_active;
				if(learn_flag)  // ѧϰģʽ
				{
					learn_flag=0;
				Elec_learn(learn_num, V_current, P_active);  //�豸��Ӻ�᷵��OK
				 
				}
				judge_res= elec_jude();  
				printf("���; %u ",judge_res);
				if(judge_res!=0)
				{
					Cmd_MIPLNOTIFY_integer( 5851, judge_res );
				}
				else
				Cmd_MIPLNOTIFY_integer( 5851, 0 );
		    }
			USART2_RX_STA=0;//������ձ�־
		}
		// delay_ms(300);
	}	 
}
 


 void Data_Processing()
{
	u32 VP_REG=0,V_REG=0,CP_REG=0,C_REG=0,PP_REG=0,P_REG=0,PF_COUNT=0,PF=0;
	double V=0,C=0,P=0,E_con=0;
	if(USART2_RX_BUF[0]!=0xaa)//оƬ�������������������������
	{
		VP_REG=USART2_RX_BUF[2]*65536+USART2_RX_BUF[3]*256+USART2_RX_BUF[4];//�����ѹ�����Ĵ���
		V_REG=USART2_RX_BUF[5]*65536+USART2_RX_BUF[6]*256+USART2_RX_BUF[7];//�����ѹ�Ĵ���
		V=(VP_REG/V_REG)*1.88;//�����ѹֵ��1.88Ϊ��ѹϵ�������������õķ�ѹ�����С��ȷ��
		V_current=V;
	printf("��ѹֵ��%0.2fV; ",V);

		
		CP_REG=USART2_RX_BUF[8]*65536+USART2_RX_BUF[9]*256+USART2_RX_BUF[10];//������������Ĵ���
		C_REG=USART2_RX_BUF[11]*65536+USART2_RX_BUF[12]*256+USART2_RX_BUF[13];//��������Ĵ���
		C=((CP_REG*100)/C_REG)/100.0;//�������ֵ
		C_current=C;
		//printf("����ֵ��%0.3fA; ",C);
		//printf(" %X ",USART2_RX_BUF[0]);
		
		if(USART2_RX_BUF[0]>0xf0)//�ж�ʵʱ�����Ƿ�δ���
		{
			Cmd_MIPLNOTIFY_float(5805, 0.00); 
		}
		else
		{
			PP_REG=USART2_RX_BUF[14]*65536+USART2_RX_BUF[15]*256+USART2_RX_BUF[16];//���㹦�ʲ����Ĵ�
			P_REG=USART2_RX_BUF[17]*65536+USART2_RX_BUF[18]*256+USART2_RX_BUF[19];//���㹦�ʼĴ���
			P=(PP_REG/P_REG)*1.88*1;//������Ч����
			P_active=P;
			//printf("��Ч���ʣ�%0.2fW; ",P);
			Cmd_MIPLNOTIFY_float(5805, P_active); 
		}
		
		if((USART2_RX_BUF[20]&0x80)!=old_reg)//�ж����ݸ��¼Ĵ������λ��û�з�ת
		{
			k++;
			old_reg=USART2_RX_BUF[20]&0x80;
		}
		PF=(k*65536)+(USART2_RX_BUF[21]*256)+USART2_RX_BUF[22];//�������õ���������
		PF_COUNT=((100000*3600)/(PP_REG*1.88))*10000;//����1�ȵ��Ӧ����������
		E_con=((PF*10000)/PF_COUNT)/10000.0;//�������õ���
		E_use=E_con;
		//printf(" %d %d ",PF,PF_COUNT);
		//printf("���õ�����%0.4f��\r\n",E_con);
	}
	else//оƬ�����������ʧЧ
	{
		printf("data error\r\n");
	}
}

