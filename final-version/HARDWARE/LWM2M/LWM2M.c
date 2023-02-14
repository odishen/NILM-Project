#include "LWM2M.h"
#include "usart.h"
#include "usart3.h"
#include "sys.h"
#include "string.h"
#include "delay.h"
char *strx;
extern u8 RX3_STA_FLAG;
u8 learn_flag=0;//学习flag
char learn_num='0'; //要学习电器编号
char learn_text = '0';//要学习电器文字编号
u8 tran_flag=1;  //调试接收响应的发送flag
u8 init_flag = 1;
void Clear_Buffer(void)
{
		for(u8 i=0; i<Usart3_RecvLen; i++)
	{
		printf("%c",USART3_RX_BUF[i]);
	}
	memset(USART3_RX_BUF,NULL,100);
	
		Usart3_RecvLen=0;
	
}
void Cmd_AT(void)
{
	 u8 wait=0;
	Uart3_SendStr((char*)"AT\r\n");
//	  while(RX3_STA_FLAG!=1);
//		RX3_STA_FLAG=0;
	delay_ms(300);
    strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");//·µ»ØOK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        Uart3_SendStr((char*)"AT\r\n"); 
			//	  while(RX3_STA_FLAG!=1);
//		RX3_STA_FLAG=0;
	delay_ms(300);
        strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");//·µ»ØOK
			wait++; 
			if(wait==3)
			{
				printf("AT ERROR");
			}
    }
	
}

void Cmd_CSQ(void)
{
	 u8 wait=0;
			    
	Uart3_SendStr((char*)"AT+CSQ\r\n"); //信号强度
         //	  while(RX3_STA_FLAG!=1);
//		RX3_STA_FLAG=0;
	delay_ms(300);
        strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");
				Clear_Buffer();
			 while(strx==NULL)
    {
        Clear_Buffer();	
        Uart3_SendStr((char*)"AT+CSQ\r\n");
			        //	  while(RX3_STA_FLAG!=1);
//		RX3_STA_FLAG=0;
	delay_ms(300);;
        strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");//·µ»ØOK
			wait++; 
			if(wait==3)
			{
				printf("CSQ ERROR");
			}
    }
    
}
void Cmd_CGATT(void)
{
	 u8 wait=0;
	Uart3_SendStr((char*)"AT+CGATT=1\r\n");// PS附着
        delay_ms(300); 
	Uart3_SendStr((char*)"AT+CGATT?\r\n");
               //	  while(RX3_STA_FLAG!=1);
//		RX3_STA_FLAG=0;
	delay_ms(300);	
        strx=strstr((const char*)USART3_RX_BUF,(const char*)"+CGATT:1");
        Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
       Uart3_SendStr((char*)"AT+CGATT?\r\n");
        	        //	  while(RX3_STA_FLAG!=1);
//		RX3_STA_FLAG=0;
	delay_ms(300);
        strx=strstr((const char*)USART3_RX_BUF,(const char*)"+CGATT:1");//·CGATT:1
			if(wait==3)
			{
				printf("CGATT ERROR");
			}
    }
Clear_Buffer();		
}
void Cmd_MIPLCREATE(void)  //创建对象
{
//	 char* error_sta=NULL;
//	u8 wait=0;
	Uart3_SendStr((char*)"AT+MIPLCREATE\r\n");
               //	  while(RX3_STA_FLAG!=1);
//		RX3_STA_FLAG=0;
	delay_ms(300);
	strx=strstr((const char*)USART3_RX_BUF,(const char*)"+MIPLCREATE:0");
	//error_sta=strstr((const char*)USART3_RX_BUF,(const char*)"error");
	Clear_Buffer();	
	 if(strx==NULL)	
				printf("MIPLCREATE ERROR");
    
				//error_sta = strstr((const char*)USART_RX_BUF,(const char*)"ERROR");
//	if(error_sta!=NULL) 
//		printf("MIPLCREATE ERROR");
        
}
void Cmd_MIPLADDOBJ(void)  //添加实例
{
	 u8 wait=0;
	Uart3_SendStr((char*)"AT+MIPLADDOBJ=0,3311,1,\"1\",4,2\r\n");
                      //	  while(RX3_STA_FLAG!=1);
//		RX3_STA_FLAG=0;
	delay_ms(300);
        strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");
				Clear_Buffer();
			if(strx==NULL)	
				printf("MIPLADDOBJ ERROR");
    
}
void Cmd_MIPLOPEN(void)
{
	 u8 wait=0;
	Uart3_SendStr((char*)"AT+MIPLOPEN=0,600,60\r\n");
                              //	  while(RX3_STA_FLAG!=1);
//		RX3_STA_FLAG=0;
	delay_ms(300);
        strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");
				Clear_Buffer();
			while(strstr((const char*)USART3_RX_BUF,(const char*)"+MIPLOBSERVE"))
			{
				printf("MIPLOPEN WAIT");
    
			}
//			if(strx==NULL)	
//				
}
void Cmd_Deinit(void)
{
	Uart3_SendStr((char*)"AT+MIPLDELOBJ=0,3311\r\n");
	delay_ms(100);
	Uart3_SendStr((char*)"AT+MIPLCLOSE=0\r\n");
	delay_ms(100);
	Uart3_SendStr((char*)"AT+MIPLDELETE=0\r\n");
	delay_ms(100);
	Clear_Buffer();	
}
//void Cmd_MIPLUPDATE(void)
//{
//	Uart3_SendStr((char*)"AT+MIPLUPDATE=0,68400,0\r\n");
//                              //	  while(RX3_STA_FLAG!=1);
////		RX3_STA_FLAG=0;
//	delay_ms(400);
//        strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");
//				Clear_Buffer();
//			if(strx==NULL)	
//				printf("UPDATE ERROR");
//}
void Cmd_MIPLNOTIFY_float( int resld, float val ) // parameter:对象ID（默认3311），实例ID（默认0），资源ID resld, 数值val(默认三位小数)
{
	u8 wait=0;
	char Send_buf[100]={0};
	sprintf(Send_buf,"AT+MIPLNOTIFY=0,0,3311,0,%u,4,4,%.2f,0,0\r\n",resld,val);
Uart3_SendStr(Send_buf);
	  while(RX3_STA_FLAG!=1);
		RX3_STA_FLAG=0;
strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");	
	Clear_Buffer();
				  while(strx==NULL)
  {
		Uart3_SendStr(Send_buf);
delay_ms(300);
    strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");//·µ»ØOK
		Clear_Buffer();
		wait++;
		if(wait>3) 
		{
			printf("MIPLNOTIFY ERROR");
			break;
		}
  }
	Clear_Buffer();

}
void Cmd_MIPLNOTIFY_integer( int resld, u8 val ) // parameter:对象ID（默认3311），实例ID（默认0），资源ID resld(5851), 数值val(默认三位小数)
{
	u8 wait=0;
	char Send_buf[100]={0};
	sprintf(Send_buf,"AT+MIPLNOTIFY=0,0,3311,0,%u,3,2,%u,0,0\r\n",resld,val);
Uart3_SendStr(Send_buf);
	  while(RX3_STA_FLAG!=1);
		RX3_STA_FLAG=0;
strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");	
	Clear_Buffer();
				  while(strx==NULL)
  {
		Uart3_SendStr(Send_buf);
delay_ms(300);
    strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");//·µ»ØOK
		Clear_Buffer();
		wait++;
		if(wait>3) 
		{
			printf("MIPLNOTIFY ERROR");
			break;
		}
  }
	Clear_Buffer();

}

void Cmd_MIPLNOTIFY_strInit( void ) // parameter:对象ID（默认3311），实例ID（默认0），资源ID resld, ，数据长度：数值val(默认三位小数)
{
	u8 wait=0;
	char Send_buf[100]={0};
	sprintf(Send_buf,"AT+MIPLNOTIFY=0,0,3311,0,5706,1,1,0,0,0\r\n");
Uart3_SendStr(Send_buf);
	  while(RX3_STA_FLAG!=1);
		RX3_STA_FLAG=0;
strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");	
	Clear_Buffer();
				  while(strx==NULL)
  {
		Uart3_SendStr(Send_buf);
delay_ms(300);
    strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");//·µ»ØOK
		Clear_Buffer();
		wait++;
		if(wait>3) 
		{
			printf("MIPLNOTIFY ERROR");
			break;
		}
  }
	Clear_Buffer();
	

}

void Cmd_MIPLNOTIFY_OK( char learn_text, char learn_number) // parameter:对象ID（默认3311），实例ID（默认0），资源ID resld, ，数据长度：数值val(默认三位小数)
{
	u8 wait=0;
	char Send_buf[120]={0};
	printf("this is num:%c",learn_number);
	sprintf(Send_buf,"AT+MIPLNOTIFY=0,0,3311,0,5706,1,3,1%c%c,0,0\r\n", learn_text, learn_number); //返回learn_num
Uart3_SendStr(Send_buf);
	  while(RX3_STA_FLAG!=1);
		RX3_STA_FLAG=0;
strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");	
	Clear_Buffer();
				  while(strx==NULL)
  {
		Uart3_SendStr(Send_buf);
delay_ms(300);
    strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");//·µ»ØOK
		Clear_Buffer();
		wait++;
		if(wait>3) 
		{
			printf("MIPLNOTIFY ERROR");
			break;
		}
  }
	Clear_Buffer();
}

void Cmd_MIPLEXECUTERSP(void) //上位机发出命令 ：learn_flag , 学习编号 learn_num
{
	tran_flag=1;
	char* str_id = NULL;
	char* str_num = NULL;
	char* str_learn_text = NULL;
	// char* str_learn_flag = NULL;
	char ID_temp[5]={0};
	 strx=strstr((const char*)USART3_RX_BUF,(const char*)"+MIPLEXECUTE");
	if(strx!=NULL)
	{		
	     tran_flag=0;
		 u8 i =0;
		 u8 j =0; //逗号个数计数
		//  u8 k =0;
		 	// 获取消息ID
			str_id=strstr((const char*)USART3_RX_BUF,(const char*)",");
			str_id++;
			while(*str_id!=',')
			{
				
				ID_temp[i] = *str_id;
				i++;
				str_id++;
			}
		for ( u8 k = 0; k < 40; k++)
		{
			if(USART3_RX_BUF[k]==',')
				j++;
			// if (j==5)
			// {
			// 	str_learn_flag = &USART3_RX_BUF[k]; //获得learn_flag位置
			// }
			
			if (j==6)
			{
				learn_flag  =*( &USART3_RX_BUF[k]+2);
				str_learn_text  = &USART3_RX_BUF[k]+3;
				str_num = &USART3_RX_BUF[k]+4; //获得learn_num 位置
				break;
				// j=0;
			}
		}
		
		
			//  char ID_temp[5]={USART3_RX_BUF[18],USART3_RX_BUF[19],USART3_RX_BUF[20],USART3_RX_BUF[21],USART3_RX_BUF[22]};
			//  写入命令中
			 char write_rsp[40]={0};
			 if(i==4)
			 sprintf(write_rsp,"AT+MIPLEXECUTERSP=0,%c%c%c%c,2\r\n",ID_temp[0],ID_temp[1],ID_temp[2],ID_temp[3]);
			 else if(i==5)
			 sprintf(write_rsp,"AT+MIPLEXECUTERSP=0,%c%c%c%c%c,2\r\n",ID_temp[0],ID_temp[1],ID_temp[2],ID_temp[3],ID_temp[4]);

			// 获取学习设备号 
			 if(learn_flag=='1')
			 {
				//  learn_flag=1;
				 learn_text = *str_learn_text;
				 learn_num = *str_num;

			 }
			 Uart3_SendStr(write_rsp);
			 
			//  Uart_SendStr(write_rsp);
	 		// Uart_SendStr(&j);
			// Uart_SendStr(&learn_num);
			// Uart_SendStr(&learn_text);
			// Uart_SendStr(&learn_flag);
			Uart_SendStr(USART3_RX_BUF);
			Clear_Buffer();
			
			//Uart_SendStr(ID_temp);//
			if(strstr((const char*)USART3_RX_BUF,(const char*)"OK")!=NULL)
			 tran_flag=1;
		
	}
	strx=NULL;
}
void BC35_Init(void)
{
	Cmd_Deinit();
	Cmd_AT();
	Cmd_CSQ();
	Cmd_CGATT();
	Cmd_MIPLCREATE();
	Cmd_MIPLADDOBJ();
	Cmd_MIPLOPEN(); //已配置好后不用MIPLDISCOVERSP
	delay_ms(30000); //延迟等待资源相应结束
	
	
}
// ==================================================================
//void Cmd_MIPLNOTIFY_String( int resld, int val_len, char val ) // parameter:对象ID（默认3311），实例ID（默认0），资源ID resld, ，数据长度：数值val(默认三位小数)
//{
//	u8 wait=0;
//	char Send_buf[100]={0};
//	sprintf(Send_buf,"AT+MIPLNOTIFY=0,0,3311,0,%u,1,%u,\"%s\",0,0\r\n",resld,val_len,val);
//Uart3_SendStr(Send_buf);
//	  while(RX3_STA_FLAG!=1);
//		RX3_STA_FLAG=0;
//strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");	
//	Clear_Buffer();
//				  while(strx==NULL)
//  {
//		Uart3_SendStr(Send_buf);
//delay_ms(300);
//    strx=strstr((const char*)USART3_RX_BUF,(const char*)"OK");//·µ»ØOK
//		Clear_Buffer();
//		wait++;
//		if(wait>3) 
//		{
//			printf("MIPLNOTIFY ERROR");
//			break;
//		}
//  }
//	Clear_Buffer();

//}
// ===================================================================================
// void Cmd_MIPLWRITERSP(void) //上位机发出命令 ：learn_flag , 学习编号 learn_num
// {
// 	tran_flag=1;
// 	 strx=strstr((const char*)USART3_RX_BUF,(const char*)"+MIPLWRITE");
// 	if(strx!=NULL)
// 	{		
// 	     tran_flag=0;
// 			 char ID_temp[5]={USART3_RX_BUF[16],USART3_RX_BUF[17],USART3_RX_BUF[18],USART3_RX_BUF[19],USART3_RX_BUF[20]};
			 
// 			 char write_rsp[40]={0};
// 			 sprintf(write_rsp,"AT+MIPLWRITERSP=0,%c%c%c%c%c,2\r\n",ID_temp[0],ID_temp[1],ID_temp[2],ID_temp[3],ID_temp[4]);
// 			 Uart3_SendStr(write_rsp);
// 			 if(USART3_RX_BUF[39]=='1')
// 			 {
// 				 learn_flag=1;
// 				 learn_num = USART3_RX_BUF[43];
// 			 }
// 			 Uart_SendStr(write_rsp);
// 	 Uart_SendStr(&learn_num);
			
// 			Clear_Buffer();
			
// 			//Uart_SendStr(ID_temp);//
// 			if(strstr((const char*)USART3_RX_BUF,(const char*)"OK")!=NULL)
// 			 tran_flag=1;
		
// 	}
// 	strx=NULL;
// }

