#include "led.h"
#include "delay.h"
#include "sys.h"
#include "dht11.h"
#include "usart.h"
#include "bh1750.h"

u8 humidityH; //湿度整数部分
u8 humidityL; //湿度小数部分
u8 temperatureH;//温度整数
u8 temperatureL;//温度小数
float Light=0; //光照度


 int main(void)
 {	
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	DHT11_Init();      //初始化温湿度
	BH1750_Init();			//初始化光照
	Usart1_Init(115200); //初始化串口
	while(1)
	{
		//温湿度读取数据
		DHT11_Read_Data(&humidityH,&humidityL,&temperatureH,&temperatureL);
		//温湿度输出到串口
		UsartPrintf(USART_DEBUG,"湿度:%d.%d % 温度:%d.%d °C ",humidityH,humidityL,temperatureH,temperatureL);
		
		if(!i2c_CheckDevice(BH1750_Addr)){
			Light = LIght_Intensity();
			UsartPrintf(USART_DEBUG,"光照度:%1.f lx\r\n",Light);
		}
		
		
		delay_ms(1000);
	}
 }

