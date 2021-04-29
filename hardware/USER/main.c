#include "led.h"
#include "delay.h"
#include "sys.h"
#include "dht11.h"
#include "usart.h"
#include "bh1750.h"
#include "oled.h"
#include "timer.h"
u8 humidityH; //湿度整数
u8 humidityL; //湿度小数
u8 temperatureH;//温度整数
u8 temperatureL;//温度小数
float Light=0; //光照


 int main(void)
 {	
	delay_init();	    	 //初始化延时函数
	LED_Init();		  	//初始化led
	DHT11_Init();      //初始化dht11
	BH1750_Init();			//初始化bh1750
	Usart1_Init(115200); //初始化串口
	OLED_Init();					//0正常显示 1 反色显示
	OLED_ColorTurn(0);    //0正常显示 1屏幕翻转显示
	OLED_DisplayTurn(0); 
	OLED_Clear();
	TIM2_Int_Init(4999,7199); 	//500ms 
	while(1)
	{
		DHT11_Read_Data(&humidityH,&humidityL,&temperatureH,&temperatureL);
		UsartPrintf(USART_DEBUG,"humidity:%d.%d  temperature:%d.%d  ",humidityH,humidityL,temperatureH,temperatureL);
		if(!i2c_CheckDevice(BH1750_Addr)){
			Light = LIght_Intensity();
			UsartPrintf(USART_DEBUG,"Light:%1.f lx\r\n",Light);
		}
		
		delay_ms(3000);
	}
 }

