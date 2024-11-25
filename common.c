#include "common.h"
#include "string.h"   
#include "stdio.h"
#include "DS3231.h"
#include "tjc.h"
char * res;
char  str[16] = "0";
int tem[16];
int time[9];
int star=0;
int set_ok=0;
int csq=0;
int sok=0;
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART3_RX_STA;
//     1,����USART3_RX_STA;
void NBIOT_response(u8 mode)
{
   int i;
	if((USART3_RX_STA&0X8000))		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
  	printf("����3���յ���ԭʼ������%s\r\n",USART3_RX_BUF);	//���͵�����
		res = (char*)USART3_RX_BUF;
//		if(set_ok == 0)//��û����֮ǰ�����޸�����
//			if(send_ok == 0)//��û����֮ǰ�����޸�����
//		{
		  if (strstr((char*)res, "+NNMI:") !=NULL) //�Ƿ�����ش�����
      {					
			  printf("NBģ�������ƽ̨������%s",res);	//���͵�����
				sscanf(res,"\r\n+NNMI:%d,%s\r\n",&tem[0],str);
			  printf("������%d,��һλ������%s",tem[0],str);	//���͵�����
				for(i=0;i<16;i++)
					 {
						tem[i] = str[i]-48;
						printf("%d\r\n",tem[i]);
					 }
					 if(send_ok == 0)//��û����֮ǰ�����޸�����
					 {
					 kind_flag = tem[3];
					 star_hour = tem[5]*10+tem[7];
					 star_min  = tem[9]*10+tem[11];
					 }
					 sok      = tem[15];
           send_ok = 1;					 
			}
      if (strstr((char*)res, "+CCLK:") !=NULL) //�Ƿ�����ش�����
      {					
			  printf("��ǰ�����ʱ����%s",res);	//���͵�����
				sscanf(res,"\r\n+CCLK:%d/%d/%d,%d:%d:%d+%d\r\n", &time[0], &time[1], &time[2],&time[3],&time[4],&time[5],&time[6]);
				if(time[3]+8 >= 24)//ת��ʱ��
				{
					time[3]=time[3]+8-24;
				}
				DS3231_Set(time[0],time[1],time[2],time[3]+8,time[4],30);  //�״�����ʱ�䣬������ɺ�ע�͵�����������һ�γ��򼴿ɵ��籣����,�£��գ�ʱ���֣���	 
			  printf("���ڵ�ǰ��ʱ���ǣ�%d:%d\r\n",time[3],time[4]);	//���͵�����
			}
			if (strstr((char*)res, "+CSQ") !=NULL) //�Ƿ�����ش�����
      {					
			  printf("��ǰ���ź���%s",res);	//���͵�����
				sscanf(res,"\r\n+CSQ:%d,%d\r\n", &time[7],&time[8]);
				csq=time[7];
			  printf("���ڵ�ǰ���ź��ǣ�%d\r\n",csq);	//���͵�����
			}
//		}
		if(mode)//�����黺������
		{
			memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
		  USART3_RX_STA=0;
		}
	}
}
//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* NBIOT_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
		memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
		USART3_RX_STA=0;
	} 
	return (u8*)strx;
}
//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��

u8 NBIOT_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(NBIOT_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
					break;//�õ���Ч���� 
				}
				
					USART3_RX_STA=0;
			} 
		 }
		if(waittime==0)res=1; 
	}
	return res;
} 
//��ATK-ESP8266����ָ������
//data:���͵�����(����Ҫ��ӻس���)
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)luojian
u8 NBIOT_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(NBIOT_check_cmd(ack))break;//�õ���Ч���� 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}

//ATK-ESP8266ģ�����������
void NBIOT_test(void)
{
//	char wbuf[10];
	while(NBIOT_send_cmd("AT","OK\r\n",20))//���WIFIģ���Ƿ�����
	printf("%s\r\n", "ATָ���������"); //
	while(NBIOT_send_cmd("AT+CSQ", "OK\r\n", 20));
	printf("%s\r\n", "ģ���źŲ�������"); //����ָʾ��
	while (NBIOT_send_cmd("AT+CGATT?", "+CGATT:1\r\n\r\nOK\r\n", 20));
	printf("%s\r\n", "ģ��������������"); //����ָʾ��
  while (NBIOT_send_cmd("AT+NCDP=49.4.85.232,5683", "OK\r\n", 20));
	printf("%s\r\n", "ģ��������ƽ̨����"); //
	while (NBIOT_send_cmd("AT+NCDP?", "+NCDP:49.4.85.232,5683\r\n\r\nOK\r\n", 20));
	printf("%s\r\n", "ģ��������ƽ̨��������"); //
	while (NBIOT_send_cmd("AT+NNMI=1", "OK\r\n", 20));
	printf("%s\r\n", "ģ�������Ϣ����"); //
//	snprintf(wbuf, 1024, "%s%d,%d","AT+NMGS=",1,12);
//	printf("%s\r\n", wbuf); //
//	while (NBIOT_send_cmd((char*)wbuf,"OK\r\n", 20));//�������仰���������
	 u3_printf("%s%d,%02X%02X%02X%02X%02X%02X%02X\r\n","AT+NMGS=",7,1,1,1,1,1,1,1);//����ָ��������Ҫ�л���
//	if(NBIOT_check_cmd("OK\r\n"))
//	{
	  printf("%s\r\n", "ģ�鷢����Ϣ�ɹ�"); //
//	}
//	  NBIOT_response(1);//���ATK-ESP8266ģ�鷢�͹���������,��ʱ�ϴ�������
	memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
	USART3_RX_STA=0;
}

void Ask_nowtime(void)//��ѯ���ڵ�ʱ��
{
	u3_printf("%s\r\n","AT+CCLK?");//����ָ��ѯ�����ڵ�ʱ��
	NBIOT_response(1);
//	DS3231_Set(time[0],time[1],time[2],time[3]+8,time[4],30);  //�״�����ʱ�䣬������ɺ�ע�͵�����������һ�γ��򼴿ɵ��籣��
//             //��,�£��գ�ʱ���֣���
	
}


















































