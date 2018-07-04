/*
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the VirtualSerial demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

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

typedef enum {normal,settings,test} Mode;
Mode opMode = normal;
Mode oldOpMode = settings;
	
typedef enum {P_max_s,P_max_d,P_min_s,T_filling,T_pump,T_adc,T,Log_On_Set} setMode;
setMode settingMode = P_max_s;
setMode oldSettingMode = P_max_d;

bool Log_On=false;

volatile int32_t lastDisplayTime = 0;
// volatile int32_t lastDistanceTime = 0;
volatile int32_t lastSwitchTime = 0;
volatile int32_t lastOffResetTime = 0;
// volatile int32_t LEDtime = 0;
volatile int32_t mss = 0;

int8_t valueShown = 0;

uint16_t pressure_mid;
uint16_t pressure_max_dyn;
uint16_t pressure_max_stat;
uint16_t pressure_min_stat;




int main(void)
{
	SetupHardware();
	
	/* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

// 	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	GlobalInterruptEnable();
	lastDisplayTime = lastSwitchTime = millis();
	for (;;)
	{
		mss = millis();
		buttonHandle();
		loop();
		/* Must throw away unused bytes from the host, or it will lock up while waiting for the device */
//		CDC_Device_SendString(&VirtualSerial_CDC_Interface, ReportString);
// 		CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}

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
	clock_init();
	USB_Init();
}

void loop(){
	//какой режим выполнения?
	switch(opMode)
	{
		case normal:
		//нормальный режим вызывает функцию нормального режима
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
	bool first = false;
	if(oldOpMode != opMode){
		oldOpMode = opMode;
		Ok_clearPress();
		Plus_clearPress();
		Minus_clearPress();
		first = true;
		lastSwitchTime = mss;
		relay1_Off();
	}
	if(Plus_isShortPress()){
		valueShown++;
		if(valueShown > 3){
			valueShown = 0;
		}
	}
	if(Ok_isShortPress() && valueShown==1){
		opMode = settings;
	}
	if(((mss-lastDisplayTime>500) && (mss-lastSwitchTime > 1000)) || first){
		lastDisplayTime = mss;
		LCD_Clear();
		switch(valueShown){
			case 0:{
				pressure_mid = pressure_midle();
				float pressure_atm = ConvToAtm(pressure_mid);
				char temp[30];
				sprintf(temp, "P min: %.2f aїј", pressure_atm);
				LCD_Goto(0,0);
				LCD_SendStr(temp);
				if (Log_On==true)
				{
					CDC_Device_SendString(&VirtualSerial_CDC_Interface, temp);
				}
			}
			break;
			case 1:
			LCD_Goto(0,0);
			LCD_SendStr((char*)"Hacїpo№єё");//Настройки
			break;
			case 2:
			LCD_Goto(0,0);
			LCD_SendStr((char*)"Tecї");//Тест
			break;
		}
	}
			
}
void settingsHandle()
{
	
}
void testHandle()
{
	
}


// CDC_Device_SendString(&VirtualSerial_CDC_Interface, ReportString);


/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	LCD_Clear();
	LCD_SendStr((char*)"USB connect");
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	LCD_Clear();
	LCD_SendStr((char*)"USB disconnect");
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
	LCD_Clear();
	LCD_SendStr((char*)"USB config ok");
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

