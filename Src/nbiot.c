#include "nbiot.h"
#include "usart.h"	
#include "string.h"	
//#include "tim.h"	
#include "stdio.h"
#include <stdlib.h>


char *strx;
extern char USART1_RxBuffer[RxBuffer_MaxSize];   //��������






/* ģ�鷢������ */
void send_NB_IoT(const char* cmd) {
//	HAL_UART_Transmit_IT(&huart2,(uint8_t*)cmd,strlen(cmd));
    HAL_UART_Transmit(&huart1, (uint8_t*)cmd, strlen(cmd), HAL_MAX_DELAY);

}
/* ģ������ */
void nbiot_reset(void) {
    send_NB_IoT("AT+QRST=1\r\n");
    HAL_Delay(100);
	Clear_Buffer();
}

uint8_t NB_IoT_ack_check(const char* str) {
    HAL_Delay(100);
        if (strstr((const char*)USART1_RxBuffer, str)) 
			{
            return 1;
			} 
		else 
			{
            return 0;
			}
}
/* ������ջ����� */
void Clear_Buffer(void)
{
//memset(USART2_RxBuffer, 0, strlen((const char*)USART2_RxBuffer));
memset(USART1_RxBuffer, 0, 256);

}


/* ģ���ʼ�� */
void NB_IotConnect(void) 
{
	send_NB_IoT("AT\r\n");
		HAL_Delay(300); 
		strx=strstr((const char*)USART1_RxBuffer,(const char*)"OK");//����OK
		Clear_Buffer();
	while(strx==NULL)
    {
		Clear_Buffer();
		send_NB_IoT("AT\r\n");
        HAL_Delay(300);
        strx=strstr((const char*)USART1_RxBuffer,(const char*)"OK");//����OK
    }

	send_NB_IoT("AT+CPIN?\r\n");
		HAL_Delay(300);
		Clear_Buffer();
	send_NB_IoT("ATE0&W\r\n");
		HAL_Delay(300); 
		Clear_Buffer();
	send_NB_IoT("AT+QMTDISC=0\r\n");
		HAL_Delay(300); 
		Clear_Buffer();
	send_NB_IoT("AT+QMTCLOSE=0\r\n");
		HAL_Delay(300); 
		Clear_Buffer();
	send_NB_IoT("AT+CFUN=1\r\n");
		HAL_Delay(300);
		Clear_Buffer();
	send_NB_IoT("AT+CIMI\r\n");
		HAL_Delay(300);
		strx=strstr((const char*)USART1_RxBuffer,(const char*)"460");//��460������ʶ�𵽿���
		Clear_Buffer();	
		while(strx==NULL)
    {
        Clear_Buffer();	
		send_NB_IoT("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        HAL_Delay(300);
        strx=strstr((const char*)USART1_RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
    }
	send_NB_IoT("AT+CGATT=1\r\n");//�������磬PDP
        HAL_Delay(300);
        strx=strstr((const char*)USART1_RxBuffer,(const char*)"OK");//��OK
        Clear_Buffer();	
	send_NB_IoT("AT+CGATT?\r\n");//��ѯ����״̬
        HAL_Delay(300);
		strx=strstr((const char*)USART1_RxBuffer,(const char*)"+CGATT: 1");//����1,˵������
        Clear_Buffer();	
		while(strx==NULL)
		{
            Clear_Buffer();	
            send_NB_IoT("AT+CGATT?\r\n");//��ȡ����״̬
            HAL_Delay(300);
            strx=strstr((const char*)USART1_RxBuffer,(const char*)"+CGATT: 1");//����1,����ע���ɹ�
		}
	send_NB_IoT("AT+CESQ\r\n");//�鿴��ȡCSQֵ
		HAL_Delay(300);
		strx=strstr((const char*)USART1_RxBuffer,(const char*)"+CESQ");//����CSQ
		Clear_Buffer();
		while(strx==NULL)
		{
            Clear_Buffer();	
            send_NB_IoT("AT+CESQ\r\n");//��ȡ����״̬
            HAL_Delay(300);
            strx=strstr((const char*)USART1_RxBuffer,(const char*)"+CESQ");//����1,�����źſ���
			Clear_Buffer();

		}
}


void NB_IoT_connect_MQTT(void) {
	send_NB_IoT("AT+QMTOPEN=0,\"47.92.146.210\",1883\r\n");
		HAL_Delay(500);
      while(NB_IoT_ack_check("+QMTOPEN: 0,0")!=1)
	  {
			Clear_Buffer();	
			send_NB_IoT("AT+QMTOPEN=0,\"47.92.146.210\",1883\r\n");
			HAL_Delay(500);
	  }
	  Clear_Buffer();	
	send_NB_IoT("AT+QMTCONN=0,\"861428040040199\"\r\n");
		HAL_Delay(500);
      while(NB_IoT_ack_check("+QMTCONN: 0,0,0")!=1)
	  {
			Clear_Buffer();	
			send_NB_IoT("AT+QMTOPEN=0,\"47.92.146.210\",1883\r\n");
			HAL_Delay(500);
			send_NB_IoT("AT+QMTCONN=0,\"861428040040199\"\r\n");
			HAL_Delay(500);
	  }
		Clear_Buffer();	
	send_NB_IoT("AT+QMTSUB=0,1,\"bc25\",0\r\n");
		HAL_Delay(500);
      while(NB_IoT_ack_check("+QMTSUB: 0,1,0,0")!=1)
	  {
			Clear_Buffer();	
			send_NB_IoT("AT+QMTSUB=0,1,\"bc25\",0\r\n");
			HAL_Delay(500);
	  }

	  	Clear_Buffer();
	  
	  
	  
}


