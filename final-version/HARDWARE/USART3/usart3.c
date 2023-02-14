#include "delay.h"
#include "usart3.h"
#include "sys.h" 	 
#include "stdio.h"	 	 
#include "string.h"	 
#include "LWM2M.h"
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

//串口接收缓存区 	

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
//#define USART_REC_LEN  			100  	//  MAX RECEIVE LENGTH  
char USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.  
u8 RX3_Counter=0; //接收计数
u8 Usart3_RecvLen=0;
u8 RX3_STA_FLAG=0;

//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void usart3_init(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口3时钟使能

 	USART_DeInit(USART3);  //复位串口3
		 //USART3_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10
   
    //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11
	
	USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART3, &USART_InitStructure); //初始化串口	3
  

	USART_Cmd(USART3, ENABLE);                    //使能串口 
	
	//使能接收中断
 USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启串口1总线空闲中断
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	

}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节


 void Uart3_SendStr(char*SendBuf)//
{
	while(*SendBuf)
	{
        while((USART3->SR&0X40)==0);//等待发送完成 
        USART3->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}

 void Uart3_SendArr(u8*SendBuf)//
{
	while(*SendBuf)
	{
        while((USART3->SR&0X40)==0);//等待发送完成 
        USART3->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}

void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收到数据中断
	{
		if(RX3_Counter<=USART3_REC_LEN)
		{
			USART3_RX_BUF[RX3_Counter++] =USART_ReceiveData(USART3);	//读取接收到的数据		
		}
		else
		{
			RX3_Counter=0;
		}		 
	 } 
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		USART_ReceiveData(USART3);
		Usart3_RecvLen=RX3_Counter;
		RX3_Counter=0;
  	RX3_STA_FLAG=1;
	 Cmd_MIPLEXECUTERSP();
	//  Cmd_MIPLWRITERSP();
	}
}  
















