//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru   
//
//  Target(s)...: ATMega8535
//
//  Compiler....: AtmelStudio 6
//
//  Description.: Библиотека для LCD в связке с макросами виртуальных портов
//                
//
//  Data........: 20.07.13
//
//***************************************************************************
#include "compilers_4.h"
#include "lcd_lib_2.h"
#include "Relay.h"
#include "ADC.h"
#include "pressure.h"
#include "buttons.h"

#define sbi(reg,bit) reg |= (1<<bit)
#define cbi(reg,bit) reg &= ~(1<<bit)

uint8_t first_run_eep EEMEM = 0;
uint16_t pressure_min_eep EEMEM = 0;
uint16_t pressure_max_eep EEMEM = 0;

uint8_t first_run;
uint8_t pump_stat;
uint16_t pressure_mid;
uint16_t pressure_max;
uint16_t pressure_min;


void set_pressure_const(uint16_t pressure_min,uint16_t pressure_max)
{
	uint16_t pressure_mid;
	uint16_t stat = 1;
	while(stat)
	{
		while(pressure_min == 0)
		{
			pressure_mid = pressure_midle();
			float pressure_atm = ConvToAtm(pressure_mid);
			char temp[30];
			sprintf(temp, "P min: %.2f aїј", pressure_atm);
			LCD_Goto(0,0);
			LCD_SendStr(temp);
			if (but_ok_IsSet())
			{
				eeprom_write_word(&pressure_min_eep,pressure_mid);
				pressure_min = eeprom_read_word(&pressure_min_eep);
				_delay_ms(150);
				relay1_Off();
			}
			if (but_plus_IsSet())
			{
				relay1_On();
				_delay_ms(150);
			}
			if (but_minus_IsSet())
			{
				relay1_Off();
				_delay_ms(150);
			}
		
			_delay_ms(300);
		
		
		}
		while(pressure_max == 0)
		{
		
			pressure_mid = pressure_midle();
			float pressure_atm = ConvToAtm(pressure_mid);
			char temp[30];
			sprintf(temp, "P max: %.2f aїј", pressure_atm);
			LCD_Goto(0,0);
			LCD_SendStr(temp);
		
			if (but_ok_IsSet())
			{
				eeprom_write_word(&pressure_max_eep,pressure_mid);
				pressure_max = eeprom_read_word(&pressure_max_eep);
				eeprom_write_byte(&first_run_eep,0x01);
				first_run = eeprom_read_byte(&first_run_eep);
				_delay_ms(150);
				relay1_Off();
			}
			if (but_plus_IsSet())
			{
				relay1_On();
				_delay_ms(150);
			}
			if (but_minus_IsSet())
			{
				relay1_Off();
				_delay_ms(150);
			}
			_delay_ms(300);
		}
		if (pressure_min >= pressure_max)
		{
			
			LCD_Clear();
			char Error[] = {0x4F,0xC1,0xB8,0xB2,0xBA,0x61,0x21,0x00};
			LCD_SendStr(Error);
			_delay_ms(1500);
			LCD_ReturnHome();
			LCD_SendStr((char*)"P max < P min");
			_delay_ms(1500);
			eeprom_write_byte(&first_run_eep,0);
			eeprom_write_word(&pressure_max_eep,0);
			eeprom_write_word(&pressure_min_eep,0);
			first_run = eeprom_read_byte(&first_run_eep);
			pressure_max = eeprom_read_word(&pressure_max_eep);
			pressure_min = eeprom_read_word(&pressure_min_eep);
			LCD_Clear();	
		}
		else
		{
			stat = 0;
		}
	}
}

int main( void )
{  
	first_run = eeprom_read_byte(&first_run_eep);
	_delay_ms(100);
	
	//инициализируем дисплей
	LCD_Init();
	relay1_Init();
	but_Init();
	ADC_Init();
	
	


	while(1)
	{
		if (!first_run)
		{
			eeprom_write_byte(&first_run_eep,0);
			eeprom_write_word(&pressure_max_eep,0);
			eeprom_write_word(&pressure_min_eep,0);
			pressure_max = eeprom_read_word(&pressure_max_eep);
			pressure_min = eeprom_read_word(&pressure_min_eep);
			set_pressure_const(pressure_min,pressure_max);
			pressure_max = eeprom_read_word(&pressure_max_eep);
			pressure_min = eeprom_read_word(&pressure_min_eep);
			LCD_Clear();
		}
		else
		{
			pressure_max = eeprom_read_word(&pressure_max_eep);
			pressure_min = eeprom_read_word(&pressure_min_eep);
		}
		
		pressure_mid = pressure_midle();
		float pressure_atm = ConvToAtm(pressure_mid);
		char temp[30];
		sprintf(temp, "P: %.2f aїј", pressure_atm);
		LCD_Goto(0,0);
		LCD_SendStr(temp);
		
		
		if (pressure_mid <= pressure_min)
		{
			relay1_On();
			
			
		};
		if (pressure_mid >= pressure_max)
		{
			relay1_Off();
		};
		
		if (but_ok_IsSet())
		{
			float pressure_tmp = ConvToAtm(pressure_min);
			char pres_temp[30];
			sprintf(pres_temp, "P min: %.2f aїј", pressure_tmp);
			LCD_Goto(0,0);
			LCD_SendStr(pres_temp);
			_delay_ms(1500);
			pressure_tmp = ConvToAtm(pressure_max);
			sprintf(pres_temp, "P max: %.2f aїј", pressure_tmp);
			LCD_Goto(0,0);
			LCD_SendStr(pres_temp);
			_delay_ms(1500);
			LCD_Clear();
			
		}
		
		if (but_plus_IsSet()&&but_minus_IsSet())
		{	
			_delay_ms(150);
			uint8_t stat = 1;
			LCD_Clear();
			LCD_SendStr((char*)"CІpoc Ѕacїpoeє");
			while(stat)
			{
				if (but_ok_IsSet())
				{
					_delay_ms(150);
					eeprom_write_byte(&first_run_eep,0);
					eeprom_write_word(&pressure_max_eep,0);
					eeprom_write_word(&pressure_min_eep,0);
					first_run = eeprom_read_byte(&first_run_eep);
					pressure_max = eeprom_read_word(&pressure_max_eep);
					pressure_min = eeprom_read_word(&pressure_min_eep);
					stat = 0;
					relay1_Off();
					LCD_Clear();
				}
				if (but_minus_IsSet())
				{
					_delay_ms(150);
					stat = 0;
					LCD_Clear();
				}
			}
			
		}
		_delay_ms(250);
	}; 
	return 0;
}


