#include "led.h"
#include "delay.h"
#include "sys.h"
#include "dht11.h"
#include "usart.h"
#include "bh1750.h"
#include "oled.h"
#include "timer.h"
#include  "beep.h"
#include 	"key.h"
#include 	"stdio.h"
#include 	"exti.h"
#include "esp8266.h"
#include "onenet.h"
u8 alarmFlag = 0; //蜂鸣器标志位
u8 alarm_is_free = 10;//报警器是否被手动操作
u8 humidityH; //湿度整数
u8 humidityL; //湿度小数
u8 temperatureH;//温度整数
u8 temperatureL;//温度小数
float Light=0; //光照
u8 LED_STATUS = 0;
char PUB_BUF[256];
const char *SubTopics[] = {"/smart/sub"};
const char PubTopic[] = "/smart/sub";

 int main(void)
 {	
	unsigned short timeCount = 0; //发送间隔变量
	unsigned char *dataPtr = NULL;
	delay_init();	    	 //初始化延时函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级分组2
	LED_Init();		  	//初始化led
	KEY_Init();  				//KEY初始化
	EXTIX_Init();  			//外部中断初始化
	BEEP_Init();				//初始化蜂鸣器
	DHT11_Init();      //初始化dht11
	BH1750_Init();			//初始化bh1750
	Usart1_Init(115200); //初始化串口1
	Usart2_Init(115200); //初始化串口2 8266通讯
	OLED_Init();					//初始化oled屏幕
	OLED_ColorTurn(0);    //0正常显示 1 反色显示
	OLED_DisplayTurn(0); //0正常显示 1屏幕翻转显示
	OLED_Clear();
	TIM2_Int_Init(4999,7199); 	//500ms 
	TIM3_Int_Init(2499,7199);
	UsartPrintf(USART_DEBUG,"Hardware Init OK!\r\n");
	ESP8266_Init();				//8266初始化
	while(OneNet_DevLink())		//接入onenet
		delay_ms(500);
	
	BEEP = 0;   //叫一下表示接入成功
	delay_ms(250);
	BEEP = 1;
	
	OneNet_Subscribe(SubTopics,1);
	while(1)
	{
			
			if(timeCount % 40 == 0)
			{
			DHT11_Read_Data(&humidityH,&humidityL,&temperatureH,&temperatureL);
			UsartPrintf(USART_DEBUG,"humidity:%d.%d  temperature:%d.%d  ",humidityH,humidityL,temperatureH,temperatureL);
			if(!i2c_CheckDevice(BH1750_Addr)){
				Light = LIght_Intensity();
				UsartPrintf(USART_DEBUG,"Light:%1.f lx\r\n",Light);
			}
			if(alarm_is_free == 10)
			{
			if(humidityH < 80 && temperatureH < 35 && Light < 10000)alarmFlag = 0;
			else alarmFlag = 1;
			
			}
			if(alarm_is_free < 10)alarm_is_free++;
			//UsartPrintf(USART_DEBUG,"alarm_is_free = %d\r\n",alarm_is_free);
			//UsartPrintf(USART_DEBUG,"alarmFlag = %d\r\n",alarmFlag);
			}
			if(++timeCount >= 200)									//发送间隔5s
			{
				LED_STATUS = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);//读取LED0的状态
				UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
				sprintf(PUB_BUF,"{\"Temp\":%d.%d,\"Hum\":%d.%d,\"Light\":%.1f,\"LED\":%d,\"BEEP\":%d}",temperatureH,temperatureL,humidityH,humidityL,Light,LED_STATUS,alarmFlag);
				OneNet_Publish(PubTopic,PUB_BUF);
				
				timeCount = 0;
				ESP8266_Clear();
			}
			
			dataPtr = ESP8266_GetIPD(3);
			if(dataPtr != NULL)
				OneNet_RevPro(dataPtr);
			delay_ms(10);
			
	}
 }

