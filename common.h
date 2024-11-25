#ifndef __COMMON_H__
#define __COMMON_H__	 
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "string.h"   
#include "stdio.h"   
#include "usart3.h" 
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� ������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
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





