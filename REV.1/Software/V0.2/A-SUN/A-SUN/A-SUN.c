/*
 * A_SUN.c
 *
 * Created: 03.09.2015 23:06:42
 *  Author: tonik_000
 */ 


#include "compilers_4.h"
#include "lcd_lib_2.h"
#include "Relay.h"
#include "ADC.h"
#include "pressure.h"
#include "buttons.h"
#include "LCD_Menu.h"

volatile uint16_t menu = 2;
volatile uint16_t old_menu = 0;
volatile uint16_t pressure_mid;
volatile uint16_t pressure_max_din;
volatile uint16_t pressure_max_stat;
volatile uint16_t pressure_min_stat;


int main(void)
{
	LCD_Init();
	relay1_Init();
	but_Init();
	ADC_Init();
    while(1)
    {
		menu = Menu(menu);
// 		char temp[30];
// 		sprintf(temp, "%d", menu);
// 		LCD_Clear();
// 		LCD_SendStr(temp);
// 		_delay_ms(250);
// 		if (menu != old_menu)
// 		{
// 		
		switch(menu)
		{
			case 1:
			{
				if (menu!=old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr((char*)"Hac¿po¹º¸");//Íàñòğîéêè
				break;
			}
			case 2:
			{
				uint16_t pressure_mid;
				pressure_mid = pressure_midle();
				float pressure_atm = ConvToAtm(pressure_mid);
				char temp[30];
				sprintf(temp, "P: %.2f a¿¼", pressure_atm);
				if (menu!=old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr(temp);
				break;
			}
			case 3:
			{
				if (menu!=old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr((char*)"Tec¿");//Òåñò
				break;
			}			
			case 11:
			{
				if (menu!=old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr((char*)"P Max Statics");
				break;
			}
			case 21:
			{
				if (menu!=old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr((char*)"P Max Dynamics");
				break;
			}
			case 31:
			{
				if (menu!=old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr((char*)"P Min Statics");
				break;
			}
			case 41:
			{
				if (menu!=old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr((char*)"T ½a¾o»½e½¸Ç");//Âğåìÿ íàïîëíåíèÿ
				break;
			}
			case 51:
			{
				if (menu!=old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr((char*)"T ½acoca");//Âğåìÿ íàñîñà
				break;
			}
			case 61:
			{
				if (menu!=old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr((char*)"T ¸½¿e´pa¿opa");//Âğåìÿ èíòåãğàòîğà
				break;
			}
			case 71:
			{
				if (menu!=old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr((char*)"T Aá¨");//Âğåìÿ ÀÖÏ
				break;
			}
			case 13:
			{
				if (menu!=old_menu)
				{
					LCD_Clear();
				}
				else
				{
					LCD_ReturnHome();
				}
				LCD_SendStr((char*)"¨epeãaÀa ãa½½Ãx");
				break;
			}
			case 110:
			{
				Set_Pressure(pressure_max_stat,menu,old_menu);
				break;
			}
			case 210:
			{
				Set_Pressure(pressure_max_din,menu,old_menu);
				break;
			}
			case 310:
			{
				Set_Pressure(pressure_min_stat,menu,old_menu);
				break;
			}
// 			case 410:
// 			{
// 				break;
// 			}
			default:
			{
				menu = 2;
			}
// 		}
		
// 		}
// 		_delay_ms(250);
		
        //TODO:: Please write your application code 
    }
	old_menu = menu;
	}
	}