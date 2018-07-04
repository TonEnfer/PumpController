/*
 * pressure.c
 *
 * Created: 13.08.2015 17:25:50
 *  Author: tonik_000
 */ 

#include "pressure.h"




uint16_t middle_of_3(uint16_t a, uint16_t b, uint16_t c)
{
	uint16_t middle;
	
	if ((a <= b) && (a <= c)){
		middle = (b <= c) ? b : c;
	}
	else{
		if ((b <= a) && (b <= c)){
			middle = (a <= c) ? a : c;
		}
		else{
			middle = (a <= b) ? a : b;
		}
	}
	
	return middle;
}

uint16_t pressure_midle ()
{
	uint16_t pressure1[3];
	uint16_t pressure2[3];
	uint16_t pressure_mid;
	for (uint8_t i = 0; i<3;i++)
	{
		
		for (uint8_t i = 0; i<3;i++)
		{
			StartConvAdc();
			while (bit_is_set(ADCSRA, ADSC));
			uint8_t low  = ADCL;
			uint8_t high = ADCH;
			pressure1[i] = ((uint16_t)low)|((uint16_t)high<<8);
		}
		pressure2[i] = middle_of_3(pressure1[0],pressure1[1],pressure1[2]);
	}
	return pressure_mid = middle_of_3(pressure2[0],pressure2[1],pressure2[2]);
}

void Set_Pressure (uint16_t _param,uint16_t _menu,uint16_t _old_menu)
{
	uint8_t stat = 0;
	float pressure_atm = ConvToAtm(_param);
	char temp[30];
	sprintf(temp, "%.2f a¿¼", pressure_atm);
	LCD_Clear();
	LCD_SendStr(temp);
	while(!stat)
	{
		// 					uint16_t pressure_max_din_old = pressure_max_din;
		if (but_plus_IsSet())
		{
			
			// 						_delay_ms(250);
			_param++;
			float pressure_atm = ConvToAtm(_param);
			char temp[30];
			sprintf(temp, "%.2f a¿¼", pressure_atm);
			if (_menu!=_old_menu)
			{
				LCD_Clear();
			}
			else
			{
				LCD_ReturnHome();
			}
			LCD_SendStr(temp);
		}
		if (but_minus_IsSet())
		{
			// 						_delay_ms(250);
			if((_param)>0)
			{
				_param--;
				float pressure_atm = ConvToAtm(_param);
				char temp[30];
				sprintf(temp, "%.2f a¿¼", pressure_atm);
				if (_menu!=_old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr(temp);
			}
			else
			{
				_param=0;
				float pressure_atm = ConvToAtm(_param);
				char temp[30];
				sprintf(temp, "%.2f a¿¼", pressure_atm);
				if (_menu!=_old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr(temp);
			}
		}
		if (but_ok_IsSet())
		{
			_delay_ms(250);
			stat = 1;
			_menu/=10;
		}
		
		_delay_ms(350);
	}
}


