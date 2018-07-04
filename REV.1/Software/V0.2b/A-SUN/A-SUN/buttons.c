/*
 * buttons.c
 *
 * Created: 05.09.2015 4:59:20
 *  Author: tonik_000
 */ 

#include "buttons.h"

static bool Plus_shortPress;
static bool Plus_askedLongPress;
static bool Plus_buttonIsPressed;
static int32_t Plus_buttonTime;

static bool Minus_shortPress;
static bool Minus_askedLongPress;
static bool Minus_buttonIsPressed;
static int32_t Minus_buttonTime;

static bool Ok_shortPress;
static bool Ok_askedLongPress;
static bool Ok_buttonIsPressed;
static int32_t Ok_buttonTime;

void buttonHandle()
{
	 if(Plus_buttonIsPressed){
		 if(!but_plus_IsSet()){
			 Plus_buttonIsPressed = false;
			 int32_t delta = millis() - Plus_buttonTime;
			 if((delta > SHORT_PRESS_TIME) && (delta < LONG_PRESS_TIME)){
				 Plus_shortPress = true;
			 }
			 Plus_askedLongPress = false;
		 }
		 }else{
		 if(but_plus_IsSet()) {
			 Plus_buttonIsPressed = true;
			 Plus_buttonTime = millis();
		 }
	 }
	  if(Minus_buttonIsPressed){
		  if(!but_minus_IsSet()){
			  Minus_buttonIsPressed = false;
			  int32_t delta = millis() - Minus_buttonTime;
			  if((delta > SHORT_PRESS_TIME) && (delta < LONG_PRESS_TIME)){
				  Minus_shortPress = true;
			  }
			  Minus_askedLongPress = false;
		  }
		  }else{
		  if(but_minus_IsSet()){
			  Minus_buttonIsPressed = true;
			  Minus_buttonTime = millis();
		  }
	  }
	   if(Ok_buttonIsPressed){
		   if(!but_ok_IsSet()){
			   Ok_buttonIsPressed = false;
			   int32_t delta = millis() - Ok_buttonTime;
			   if((delta > SHORT_PRESS_TIME) && (delta < LONG_PRESS_TIME)){
				   Ok_shortPress = true;
			   }
			   Ok_askedLongPress = false;
		   }
		   }else{
		   if(but_ok_IsSet()){
			   Ok_buttonIsPressed = true;
			   Ok_buttonTime = millis();
		   }
	   }
}

bool Plus_isLongPress(){
	if(!Plus_askedLongPress){
		if(Plus_buttonIsPressed && but_plus_IsSet() && ((millis() - Plus_buttonTime) > LONG_PRESS_TIME)){
			Plus_shortPress = false;
			Plus_askedLongPress = true;
			return true;
		}
	}
	return false;
}
bool Plus_isShortPress(){
	if(Plus_shortPress){
		Plus_shortPress = false;
		return true;
	}
	return false;
}

bool Minus_isLongPress(){
	if(!Minus_askedLongPress){
		if(Minus_buttonIsPressed && but_minus_IsSet() && ((millis() - Minus_buttonTime) > LONG_PRESS_TIME)){
			Minus_shortPress = false;
			Minus_askedLongPress = true;
			return true;
		}
	}
	return false;
}
bool Minus_isShortPress(){
	if(Minus_shortPress){
		Minus_shortPress = false;
		return true;
	}
	return false;
}

bool Ok_isLongPress(){
	if(!Ok_askedLongPress){
		if(Ok_buttonIsPressed && but_ok_IsSet() && ((millis() - Ok_buttonTime) > LONG_PRESS_TIME)){
			Ok_shortPress = false;
			Ok_askedLongPress = true;
			return true;
		}
	}
	return false;
}
bool Ok_isShortPress(){
	if(Ok_shortPress){
		Ok_shortPress = false;
		return true;
	}
	return false;
}

void Plus_clearPress(){
	Plus_shortPress = false;
}

void Minus_clearPress(){
	Minus_shortPress = false;
}

void Ok_clearPress(){
	Ok_shortPress = false;
}