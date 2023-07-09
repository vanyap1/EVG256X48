/*
 * EVG256X48.c
 *
 * Created: 04.07.2023 18:05:09
 * Author : princ
 */ 

#include <avr/io.h>
#include <u8g2.h>
#include <util/delay.h>
#include "lib/display_fn.h"
#include "lib/uart_hal.h"
#include "lib/gpio_driver.h"
#include "lib/spi_hal.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>

static FILE mystdout = FDEV_SETUP_STREAM((void *)uart_send_byte, NULL, _FDEV_SETUP_WRITE);

u8g2_t u8g2;
gpio ld = {(uint8_t *)&PORTB , PORTB5};
	
int main(void)
{
	sei();
	stdout = &mystdout;
	char char_array[128]="255\0";
	spi_master_init();
	
	
	set_pin_dir(&ld , PORT_DIR_OUT); set_pin_level(&ld, false);
	u8g2_Setup_gp1287ai_256x50_1(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_avr_gpio_and_delay);
	u8g2_InitDisplay(&u8g2);
	
	u8g2_SetPowerSave(&u8g2, 0);
	u8g2_SetContrast(&u8g2, 7);
	
	set_pin_dir(&ld , PORT_DIR_OUT); set_pin_level(&ld, false);
	uart_init(250000,0);
	
	uint8_t y_pos = 0;
	
	u8g2_ClearBuffer(&u8g2);
	//u8g2_SetFont(&u8g2, u8g2_font_artosserif8_8u); 
	u8g2_SetFont(&u8g2, u8g2_font_iconquadpix_m_all);
	//u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);
    /* Replace with your application code */
    while (1) 
    {
		//u8g2_SetFontMode(&u8g2, 0);
		
		
		
		

		
		u8g2_FirstPage(&u8g2);
		//printf("test\n\r");
		//W5500_Write_byte(0x80);
		sprintf(char_array, "%d" , y_pos);
		do 
		{
			u8g2_DrawRFrame(&u8g2, 200, 4, 40 ,40, 5);
			u8g2_DrawStr(&u8g2, 0, 12, char_array);
			u8g2_DrawStr(&u8g2, 0, 26, "Bb");
			u8g2_SendBuffer(&u8g2);
		} while (u8g2_NextPage(&u8g2));
		
		
		//_delay_ms(200);
		y_pos++;
		
    }
}

