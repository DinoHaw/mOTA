#include "bsp_config.h"

static void delay_ms(uint16_t time)
{
	uint32_t i = 0;
	while(time--)
	{
		i = 8000;
		while(i--);
	}
}
void bsp_hardware_init(void)
{
//	bsp_key_hardware_init();
//	
//	if(KEY_SWITCH_STA_READ)
//		PWR_5V_STA_SET();
//	else
//	{
//		delay_ms(2000);
//		PWR_5V_STA_SET();	//打开供电
//	}
	
	bsp_beep_hardware_init();
	
	bsp_beep_start();
	
	delay_ms(50);
	
	bsp_beep_stop();
	
//	bsp_lptimer_hardware_init();
//	
//	bsp_wifi_hardware_init();
	
	bsp_usb_hardware_init();
	 
//	bsp_485_hardware_init();

	bsp_rtc_hardware_init();
	
	bsp_rtc_software_init();
	
	bsp_sensor_hardware_init();
	
	bsp_input_hardware_init();
	
//	bsp_flash_hardware_init();
	
	bsp_lcd_hardware_init();
	
//	bsp_timer8_hardware_init();
	
//	bsp_alarm_hardware_init();

//	bsp_ble_hardware_init();
}