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
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART3_RX_STA;
//     1,清零USART3_RX_STA;
void NBIOT_response(u8 mode)
{
   int i;
	if((USART3_RX_STA&0X8000))		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
  	printf("串口3接收到的原始数据是%s\r\n",USART3_RX_BUF);	//发送到串口
		res = (char*)USART3_RX_BUF;
//		if(set_ok == 0)//还没锁死之前可以修改数据
//			if(send_ok == 0)//还没锁死之前可以修改数据
//		{
		  if (strstr((char*)res, "+NNMI:") !=NULL) //是否包含回传数据
      {					
			  printf("NB模块接收云平台数据是%s",res);	//发送到串口
				sscanf(res,"\r\n+NNMI:%d,%s\r\n",&tem[0],str);
			  printf("长度是%d,第一位数据是%s",tem[0],str);	//发送到串口
				for(i=0;i<16;i++)
					 {
						tem[i] = str[i]-48;
						printf("%d\r\n",tem[i]);
					 }
					 if(send_ok == 0)//还没锁死之前可以修改数据
					 {
					 kind_flag = tem[3];
					 star_hour = tem[5]*10+tem[7];
					 star_min  = tem[9]*10+tem[11];
					 }
					 sok      = tem[15];
           send_ok = 1;					 
			}
      if (strstr((char*)res, "+CCLK:") !=NULL) //是否包含回传数据
      {					
			  printf("当前网络的时间是%s",res);	//发送到串口
				sscanf(res,"\r\n+CCLK:%d/%d/%d,%d:%d:%d+%d\r\n", &time[0], &time[1], &time[2],&time[3],&time[4],&time[5],&time[6]);
				if(time[3]+8 >= 24)//转化时间
				{
					time[3]=time[3]+8-24;
				}
				DS3231_Set(time[0],time[1],time[2],time[3]+8,time[4],30);  //首次设置时间，设置完成后注释掉，重新下载一次程序即可掉电保存年,月，日，时，分，秒	 
			  printf("现在当前的时间是：%d:%d\r\n",time[3],time[4]);	//发送到串口
			}
			if (strstr((char*)res, "+CSQ") !=NULL) //是否包含回传数据
      {					
			  printf("当前的信号是%s",res);	//发送到串口
				sscanf(res,"\r\n+CSQ:%d,%d\r\n", &time[7],&time[8]);
				csq=time[7];
			  printf("现在当前的信号是：%d\r\n",csq);	//发送到串口
			}
//		}
		if(mode)//把数组缓存清零
		{
			memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
		  USART3_RX_STA=0;
		}
	}
}
//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* NBIOT_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
		memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
		USART3_RX_STA=0;
	} 
	return (u8*)strx;
}
//向ATK-ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败

u8 NBIOT_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(NBIOT_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
					break;//得到有效数据 
				}
				
					USART3_RX_STA=0;
			} 
		 }
		if(waittime==0)res=1; 
	}
	return res;
} 
//向ATK-ESP8266发送指定数据
//data:发送的数据(不需要添加回车了)
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)luojian
u8 NBIOT_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s",data);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(NBIOT_check_cmd(ack))break;//得到有效数据 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}

//ATK-ESP8266模块测试主函数
void NBIOT_test(void)
{
//	char wbuf[10];
	while(NBIOT_send_cmd("AT","OK\r\n",20))//检查WIFI模块是否在线
	printf("%s\r\n", "AT指令测试正常"); //
	while(NBIOT_send_cmd("AT+CSQ", "OK\r\n", 20));
	printf("%s\r\n", "模块信号测试正常"); //点亮指示灯
	while (NBIOT_send_cmd("AT+CGATT?", "+CGATT:1\r\n\r\nOK\r\n", 20));
	printf("%s\r\n", "模块入网测试正常"); //点亮指示灯
  while (NBIOT_send_cmd("AT+NCDP=49.4.85.232,5683", "OK\r\n", 20));
	printf("%s\r\n", "模块连接云平台正常"); //
	while (NBIOT_send_cmd("AT+NCDP?", "+NCDP:49.4.85.232,5683\r\n\r\nOK\r\n", 20));
	printf("%s\r\n", "模块连接云平台测试正常"); //
	while (NBIOT_send_cmd("AT+NNMI=1", "OK\r\n", 20));
	printf("%s\r\n", "模块接收消息开启"); //
//	snprintf(wbuf, 1024, "%s%d,%d","AT+NMGS=",1,12);
//	printf("%s\r\n", wbuf); //
//	while (NBIOT_send_cmd((char*)wbuf,"OK\r\n", 20));//用这三句话程序会死掉
	 u3_printf("%s%d,%02X%02X%02X%02X%02X%02X%02X\r\n","AT+NMGS=",7,1,1,1,1,1,1,1);//发送指令后面必须要有换行
//	if(NBIOT_check_cmd("OK\r\n"))
//	{
	  printf("%s\r\n", "模块发送消息成功"); //
//	}
//	  NBIOT_response(1);//检查ATK-ESP8266模块发送过来的数据,及时上传给电脑
	memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
	USART3_RX_STA=0;
}

void Ask_nowtime(void)//查询现在的时间
{
	u3_printf("%s\r\n","AT+CCLK?");//发送指令询问现在的时间
	NBIOT_response(1);
//	DS3231_Set(time[0],time[1],time[2],time[3]+8,time[4],30);  //首次设置时间，设置完成后注释掉，重新下载一次程序即可掉电保存
//             //年,月，日，时，分，秒
	
}


















































