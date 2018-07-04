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
 *  Header file for VirtualSerial.c.
 */

#ifndef _VIRTUALSERIAL_H_
#define _VIRTUALSERIAL_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <avr/pgmspace.h>
		#include <avr/eeprom.h>
		#include <stdio.h>
		#include <stdlib.h>
		#include <string.h>

		#include "Descriptors.h"
		#include "lcd_lib_2.h"
		#include "Relay.h"
		#include "ADC.h"
		#include "pressure.h"
		#include "buttons.h"

		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/Board/Joystick.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Platform/Platform.h>
		
		
		#define __save_interrupt()       SREG
		#define __restore_interrupt(var) SREG = (var)
		#define __disable_interrupt()    cli()
		#define __enable_interrupt()     sei()
		#define __delay_cycles(var)      _delay_us((uint16_t)(var)/(F_CPU/1000000))
		#define delay_us(us) 	         _delay_us(us)
		#define delay_ms(ms)             _delay_ms(ms)
		#define __swap_nibbles(x)        (((x<<4)&0xf0)|((x>>4)&0x0f))

		#define INLINE inline

		#define __flash const PROGMEM
		#define __flash_p
		#define read_byte_flash(x) pgm_read_byte(&(x))
		#define read_word_flash(x) pgm_read_word(&(x))

	/* Function Prototypes: */
		void SetupHardware(void);
		void set_const();
		void loop ();
		void normalHandle();
		void settingsHandle();
		void testHandle();
		
		
		
		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);

#endif

