#ifndef __USART3_H
#define __USART3_H	 
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口3驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

#define USART3_REC_LEN  			200  	//  MAX RECEIVE LENGTH  
	  	
extern char  USART3_RX_BUF[USART3_REC_LEN]; // USART1 REVEIVE BUFF
extern u8 Usart3_RecvLen;
extern u8  RX3_STA_FLAG;
void usart3_init(u32 bound);				//串口3初始化 
void Uart3_SendStr(char*SendBuf);//串口3打印数据
void Uart3_SendArr(u8*SendBuf);//
#endif












