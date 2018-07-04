/*
 * LCD_Menu.c
 *
 * Created: 03.09.2015 23:27:49
 *  Author: tonik_000
 */ 

#include "LCD_Menu.h"

uint16_t Menu (uint16_t _menu)
{
	if ((_menu == 1)|(_menu==2)|(_menu==3))
	{
		
		if (but_plus_IsSet())
		{
			_menu++;
		}
		if (but_minus_IsSet())
		{
			_menu = 2;
		}
		if (but_ok_IsSet())
		{
			_menu+=10;
		}
		_delay_ms(250);
	}
	if (_menu==4)
	{
		_menu=1;
	}
	
	if ((_menu > 10)&(_menu < 99))
	{
		
		if (but_plus_IsSet())
		{
			if (!(_menu==13))
			{
				if ((_menu+10)<80)
				{
					_menu+=10;
				}
				else
				{
					_menu-=60;
				}
			}
			_delay_ms(250);
		}
		if (but_minus_IsSet())
		{
			_menu = 2;
			
		}
		if (but_ok_IsSet())
		{
			if (!(_menu==13))
			{
				_menu*=10;
			}
			else{_menu-=10;}
			_delay_ms(250);
		}
	}
	if ((_menu > 109)&(_menu < 1000))
	{
		
		if (but_minus_IsSet())
		{
			if ((_menu/10)>10)
			{
				_menu/=10;
			}
			
		}
		_delay_ms(250);		
	}
	return _menu;
	
}
