#ifndef __COMMON_H__
#define __COMMON_H__	 
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "string.h"   
#include "stdio.h"   
#include "usart3.h" 
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 公用驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
extern 	int tem[16];
extern 	int time[9];
extern  int csq;
extern  int star;
extern  int sok;
extern  int set_ok;
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void NBIOT_init(void);

extern void NBIOT_response(u8 mode);
extern void Ask_nowtime(void);
u8* NBIOT_check_cmd(u8 *str);
u8 NBIOT_send_data(u8 *data,u8 *ack,u16 waittime);
u8 NBIOT_send_cmd(u8 *cmd,u8 *ack,u16 waittime);


void NBIOT_test(void);


#endif





