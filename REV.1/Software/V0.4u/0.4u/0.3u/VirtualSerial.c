#include "VirtualSerial.h"

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = INTERFACE_ID_CDC_CCI,
				.DataINEndpoint           =
					{
						.Address          = CDC_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs.
 */
static FILE USBSerialStream;


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */

uint8_t first_run_eep EEMEM = 0;
uint16_t pressure_min_eep EEMEM = 0;
uint16_t pressure_max_eep EEMEM = 0;
uint16_t time_of_regulator_eep EEMEM = 0;

volatile uint8_t first_run = 0;
volatile uint8_t first_run_local = 0;
volatile uint8_t pump_stat = 0;
volatile uint16_t pressure_mid = 41;
volatile uint16_t pressure_max = 0;
volatile uint16_t pressure_min = 0;
volatile uint16_t time_of_regulator = 0;


volatile bool relay_On_first = true;
volatile bool relay_Off_first = true;


bool testHandle_FirstStart = true;
uint16_t testHandle_P_Max = 0;
uint16_t testHandle_P_Min = 0;
uint16_t testHandle_T_Reg = 0;
uint8_t testHandle_RelayOld = 0;

typedef enum {normal,settings,test} menu;
	menu mode = normal;
	menu oldmode = test;
	menu Show = normal;
	menu oldShow = normal;
	
void set_const()
{
	uint8_t stat = 1;
	uint8_t P_max_ok = 0;
	uint8_t P_min_ok = 0;
	uint8_t P_ok = 0;
	uint8_t T_reg_ok = 0;
	pressure_max = eeprom_read_word(&pressure_max_eep);
	pressure_min = eeprom_read_word(&pressure_min_eep);
	time_of_regulator = eeprom_read_word(&time_of_regulator_eep);
	
	while(stat)
	{
		while(!P_ok)
		{
			if (pressure_min == 0)
			{
				pressure_mid = 41;
			}
			else pressure_mid = pressure_min;
			while(!P_min_ok)
			{
				
				float pressure_atm = ConvToAtm(pressure_mid);
				char temp[30];
				sprintf(temp, "P min: %.2f a¿¼", pressure_atm);
				LCD_Goto(0,0);
				LCD_SendStr(temp);
				if (but_ok_IsSet())
				{
					_delay_ms(70);
					if (but_ok_IsSet())
					{
						eeprom_update_word(&pressure_min_eep,pressure_mid);
						pressure_min = eeprom_read_word(&pressure_min_eep);
						P_min_ok = 1;
						if (pressure_max == 0)
						{
							pressure_mid = 41;
						}
						else pressure_mid = pressure_max;
					}
					
				}
				if (but_plus_IsSet())
				{
					_delay_ms(70);
					if (but_plus_IsSet())
					{
						pressure_mid++;
					}
				}
				if (but_minus_IsSet())
				{
					_delay_ms(70);
					if (but_minus_IsSet())
					{
						if (pressure_mid>41)
						pressure_mid--;
						else pressure_mid = 41;
					}
				}
				
				_delay_ms(80);
			}
			while(!P_max_ok)
			{
				
				float pressure_atm = ConvToAtm(pressure_mid);
				char temp[30];
				sprintf(temp, "P max: %.2f a¿¼", pressure_atm);
				LCD_Goto(0,0);
				LCD_SendStr(temp);
				
				if (but_ok_IsSet())
				{
					_delay_ms(70);
					if (but_ok_IsSet())
					{ 
						pressure_max = pressure_mid;
						if (pressure_min > pressure_max)
						{
							
							LCD_Clear();
							char Error[] = {0x4F,0xC1,0xB8,0xB2,0xBA,0x61,0x21,0x00};
							LCD_SendStr(Error);
							_delay_ms(1500);
							LCD_ReturnHome();
							LCD_SendStr((char*)"P max < P min");
							_delay_ms(1500);
// 							eeprom_update_byte(&first_run_eep,0);
// // 							eeprom_update_word(&pressure_max_eep,0);
// // 							eeprom_update_word(&pressure_min_eep,0);
// 							first_run = eeprom_read_byte(&first_run_eep);
// 							pressure_max = eeprom_read_word(&pressure_max_eep);
// 							pressure_min = eeprom_read_word(&pressure_min_eep);
							LCD_Clear();
						}
						else
						{
							eeprom_update_word(&pressure_max_eep,pressure_mid);
							pressure_max = eeprom_read_word(&pressure_max_eep);
							pressure_mid = 0;
							P_max_ok = 1;
							LCD_Clear();
							LCD_SendStr((char*)"P ok!");
							_delay_ms(1000);
							P_ok = 1;
						}
						
					}
				}
				if (but_plus_IsSet())
				{
					_delay_ms(70);
					if (but_plus_IsSet())
					{
						pressure_mid++;
					}
				}
				if (but_minus_IsSet())
				{
					_delay_ms(70);
					if (but_minus_IsSet())
					{
						if (pressure_mid>41)
						pressure_mid--;
						else pressure_mid = 41;
					}
				}
			}
		}
		while (!T_reg_ok)
		{
			char temp[30];
			sprintf(temp, "T Reg: %3.0d c", time_of_regulator);
			LCD_Goto(0,0);
			LCD_SendStr(temp);
			if (but_ok_IsSet())
			{
				_delay_ms(70);
				if (but_ok_IsSet())
				{
					eeprom_update_word(&time_of_regulator_eep,time_of_regulator);
					time_of_regulator = eeprom_read_word(&time_of_regulator_eep);
					T_reg_ok = 1;
					eeprom_write_byte(&first_run_eep,1);
					first_run = eeprom_read_byte(&first_run_eep);
					stat = 0;
					LCD_Clear();
				}
				
			}
			if (but_plus_IsSet())
			{
				_delay_ms(70);
				if (but_plus_IsSet())
				{
					time_of_regulator++;
				}
			}
			if (but_minus_IsSet())
			{
				_delay_ms(70);
				if (but_minus_IsSet())
				{
					if (time_of_regulator>1)
					time_of_regulator--;
					else time_of_regulator = 1;
				}
			}
		}
		
	}
}

int main(void)
{
	SetupHardware();

	/* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

	GlobalInterruptEnable();

	first_run = eeprom_read_byte(&first_run_eep);

	for (;;)
	{
		if (!first_run_local)
		{
			if (!first_run)
			{
				set_const();
				LCD_Clear();
				first_run_local = 1;
			}
			else
			{
				pressure_max = eeprom_read_word(&pressure_max_eep);
				pressure_min = eeprom_read_word(&pressure_min_eep);
				time_of_regulator = eeprom_read_word(&time_of_regulator_eep);
				float pressure_atm = ConvToAtm(pressure_min);
				char temp[30];
				sprintf(temp, "P min: %.2f a¿¼", pressure_atm);
				LCD_Goto(0,0);
				LCD_SendStr(temp);
				_delay_ms(1000);
				pressure_atm = ConvToAtm(pressure_max);
				sprintf(temp, "P max: %.2f a¿¼", pressure_atm);
				LCD_Goto(0,0);
				LCD_SendStr(temp);
				_delay_ms(1000);
				LCD_Clear();
				sprintf(temp, "T Reg: %3.0d c", time_of_regulator);
				LCD_SendStr(temp);
				_delay_ms(1000);
				LCD_Clear();
				first_run_local = 1;
			}		
		}
		
		loop();
		_delay_ms(250);
		
		/* Must throw away unused bytes from the host, or it will lock up while waiting for the device */
// 		CDC_Device_SendString(&VirtualSerial_CDC_Interface, ReportString);
	}
}

void loop ()
{
	switch (mode)
	{
		case normal:
		normalHandle();
		break;
		case settings:
		settingsHandle();
		break;
		case test:
		testHandle();
		break;
	}
}

void normalHandle()
{
	
	if(oldmode != mode){
		oldmode = mode;
		Show = mode;
	}

	if (but_plus_IsSet())
	{
		_delay_ms(70);
		if (but_plus_IsSet())
		{
			if (Show < 2)
				 Show++;
			else Show = 0;
		}
	}
	if (but_ok_IsSet())
	{
		_delay_ms(70);
		if (but_ok_IsSet())
		{
			switch (Show)
			{
				case settings:	mode = settings; break;
				case test:{ mode = test; testHandle_FirstStart=true; break;}
				default: break;
			}
		}
	}
		if (oldShow != Show)
		{
			oldShow = Show;
			LCD_Clear();
			
		}
	switch (Show)
	{
		case normal:
		{
			pressure_mid = cur_pressure_SMA(pressure_midle());
			float pressure_atm = ConvToAtm(pressure_mid);
			char temp[30];
			sprintf(temp, "P: %.2f a¿¼", pressure_atm);
			LCD_Goto(0,0);
			LCD_SendStr(temp);
			break;
			
		}
		case settings:
		{
			LCD_Goto(0,0);
			LCD_SendStr((char*)"Hac¿po¹º¸");//Íàñòðîéêè
			break;
		}
		case test:
		{
			LCD_Goto(0,0);
			LCD_SendStr((char*)"Tec¿");//Òåñò
			break;
		}
	}
	regulation();
	
}
void settingsHandle()
{
	set_const();
	mode = normal;
}
void testHandle()
{
	if (testHandle_FirstStart)
	{
		LCD_Goto(0,0);
		LCD_SendStr((char*)"Test running!");
		CDC_Device_SendString(&VirtualSerial_CDC_Interface, (char*)"Max\tMin\tT Reg\tRelay\tOriginal\tSMA\r\n");
		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
		testHandle_FirstStart = false;
		_delay_ms(750);
		LCD_Clear();
	}
	Send_Data_USB();
	if (pressure_mid <= pressure_min)
	{
		relay1_On();
		relay_Off_first = true;
		if (relay_On_first)
		{
			for (uint16_t i = 0; i<(time_of_regulator*4UL); i++)
			{
				Send_Data_USB();
				_delay_ms(250);
			}
			relay_On_first = false;
		}
		
	};
	if (pressure_mid >= pressure_max)
	{
		relay1_Off();
		relay_On_first = true;
		if (relay_Off_first)
		{
			for (uint16_t i = 0; i<(time_of_regulator*4UL); i++)
			{
				Send_Data_USB();
				_delay_ms(250);
			}
			relay_Off_first = false;
		}
	};
	
	float pressure_atm = ConvToAtm(pressure_mid);
	char temp[30];
	sprintf(temp, "P test: %.2f a¿¼", pressure_atm);
	LCD_Goto(0,0);
	LCD_SendStr(temp);
	if (but_minus_IsSet())
	{
		_delay_ms(70);
		if (but_minus_IsSet())
		{
			mode = normal;
			LCD_Clear();
		}
	}
	_delay_ms(250);
}

void Send_Data_USB(void)
{
	char temp[30];
	sprintf(temp, "%d\t", pressure_max);
	CDC_Device_SendString(&VirtualSerial_CDC_Interface, temp);
	sprintf(temp, "%d\t", pressure_min);
	CDC_Device_SendString(&VirtualSerial_CDC_Interface, temp);
	sprintf(temp, "%d\t", time_of_regulator);
	CDC_Device_SendString(&VirtualSerial_CDC_Interface, temp);
	sprintf(temp, "%d\t", relay1_IsOn());
	CDC_Device_SendString(&VirtualSerial_CDC_Interface, temp);
	sprintf(temp, "%d\t", pressure_midle());
	CDC_Device_SendString(&VirtualSerial_CDC_Interface, temp);
	pressure_mid = cur_pressure_SMA(pressure_midle());
	sprintf(temp, "%d\r\n", pressure_mid);
	CDC_Device_SendString(&VirtualSerial_CDC_Interface, temp);
	CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
	USB_USBTask();
}

void regulation(void)
{
	if (pressure_mid <= pressure_min)
	{
		relay1_On();
		relay_Off_first = true;
		if (relay_On_first)
		{
			for (uint16_t i = 0; i<time_of_regulator; i++)
			{
				_delay_ms(1000);
			}
			relay_On_first = false;
		}
		
	};
	if (pressure_mid >= pressure_max)
	{
		relay1_Off();
		relay_On_first = true;
		if (relay_Off_first)
		{
			for (uint16_t i = 0; i<time_of_regulator; i++)
			{
				_delay_ms(1000);
			}
			relay_Off_first = false;
		}
		
	};
};



/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
	XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
	XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

	/* Hardware Initialization */
	LCD_Init();
	relay1_Init();
	but_Init();
	ADC_Init();
	pressure_Init();
	USB_Init();
}

/** Checks for changes in the position of the board joystick, sending strings to the host upon each change. */
		// CDC_Device_SendString(&VirtualSerial_CDC_Interface, ReportString);

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
		LCD_Clear();
		LCD_SendStr((char*)"USB connect");
		_delay_ms(500);
		LCD_Clear();
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	LCD_Clear();
	LCD_SendStr((char*)"USB disconnect");
	_delay_ms(500);
	LCD_Clear();
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

