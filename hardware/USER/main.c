#include "led.h"
#include "delay.h"
#include "sys.h"
#include "dht11.h"
#include "usart.h"
#include "bh1750.h"

u8 humidityH; //ʪ����������
u8 humidityL; //ʪ��С������
u8 temperatureH;//�¶�����
u8 temperatureL;//�¶�С��
float Light=0; //���ն�


 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	DHT11_Init();      //��ʼ����ʪ��
	BH1750_Init();			//��ʼ������
	Usart1_Init(115200); //��ʼ������
	while(1)
	{
		//��ʪ�ȶ�ȡ����
		DHT11_Read_Data(&humidityH,&humidityL,&temperatureH,&temperatureL);
		//��ʪ�����������
		UsartPrintf(USART_DEBUG,"ʪ��:%d.%d % �¶�:%d.%d ��C ",humidityH,humidityL,temperatureH,temperatureL);
		
		if(!i2c_CheckDevice(BH1750_Addr)){
			Light = LIght_Intensity();
			UsartPrintf(USART_DEBUG,"���ն�:%1.f lx\r\n",Light);
		}
		
		
		delay_ms(1000);
	}
 }

