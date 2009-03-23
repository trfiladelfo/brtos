
/*********************************************************************************************************
*                                               BRTOS
*                                Brazilian Real-Time Operating System
*                            Acronymous of Basic Real-Time Operating System
*
*                              
*                                  Open Source RTOS under MIT License
*
*
*
*                                      OS Time managment functions
*
*
*   Author: Gustavo Weber Denardin
*   Revision: 1.0
*   Date:     20/03/2009
*
*********************************************************************************************************/


#include "OS_RTC.h"
#include "BRTOS.h"
#include <hidef.h> /* for EnableInterrupts macro */

// estrutura - Hora
  OSTime Hora;
  OSDate Data;





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      OS Update Time Function                     /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void OSUpdateTime(OSTime *Ptr_Hora)
{
  #if (Coldfire == 1)
  if (!iNesting)
  #endif
     OSEnterCritical;
     
  Ptr_Hora -> RTC_Second++;

 	if (Ptr_Hora -> RTC_Second == 60){

 		Ptr_Hora -> RTC_Second = 0;
 		Ptr_Hora -> RTC_Minute++;

 	if (Ptr_Hora -> RTC_Minute == 60){

 		Ptr_Hora -> RTC_Minute = 0;
 		Ptr_Hora -> RTC_Hour++;

 	if (Ptr_Hora -> RTC_Hour == 24){

 		Ptr_Hora -> RTC_Hour = 0;
  		
 	}}}
  	
  #if (Coldfire == 1)
  if (!iNesting)
  #endif
     OSExitCritical;
	
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      OS Update Time Function                     /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void OSUpdateUptime(OSTime *Ptr_Hora,OSDate *Ptr_Dia)
{

  #if (Coldfire == 1)
  if (!iNesting)
  #endif
     OSEnterCritical;
     
  Ptr_Hora -> RTC_Second++;

  if (Ptr_Hora -> RTC_Second == 60){

  	Ptr_Hora -> RTC_Second = 0;
  	Ptr_Hora -> RTC_Minute++;

  if (Ptr_Hora -> RTC_Minute == 60){

  	Ptr_Hora -> RTC_Minute = 0;
  	Ptr_Hora -> RTC_Hour++;

  if (Ptr_Hora -> RTC_Hour == 24){

  	Ptr_Hora -> RTC_Hour = 0;
  	Ptr_Dia -> RTC_Day++;
  		
  }}}
  
  #if (Coldfire == 1)
  if (!iNesting)
  #endif
     OSExitCritical;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      OS Update Date Function                     /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void OSUpdateDate(OSDate *Ptr_Dia)
{   

  #if (Coldfire == 1)
  if (!iNesting)
  #endif
     OSEnterCritical;
     
	Ptr_Dia -> RTC_Day++;	
		
	if (Ptr_Dia -> RTC_Day == 30){ 
	// deve-se adaptar para os dias exatos de cada m�s

		Ptr_Dia -> RTC_Day = 0;
		Ptr_Dia -> RTC_Month++;

	if (Ptr_Dia -> RTC_Month == 12){

		Ptr_Dia -> RTC_Month = 0;
		Ptr_Dia -> RTC_Year++;

	if (Ptr_Dia -> RTC_Year == 9999){    // ano m�ximo 9999	

		Ptr_Dia -> RTC_Year = 0;
	}}}
	
  #if (Coldfire == 1)
  if (!iNesting)
  #endif
     OSExitCritical;
	
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      OS Reset Time Function                      /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void OSResetTime(OSTime *Ptr_Hora)
{
      
  #if (Coldfire == 1)
  if (!iNesting)
  #endif
     OSEnterCritical;
     
   Ptr_Hora->RTC_Second = 0;
   Ptr_Hora->RTC_Minute = 0;
   Ptr_Hora->RTC_Hour = 0;
   
  #if (Coldfire == 1)
  if (!iNesting)
  #endif
     OSExitCritical;
      
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////    





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      OS Reset Date Function                      /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
 
void OSResetDate(OSDate *Ptr_Dia)
{
   
  #if (Coldfire == 1)
  if (!iNesting)
  #endif
     OSEnterCritical;
     
   Ptr_Dia->RTC_Day = 0;
   Ptr_Dia->RTC_Month = 0;
   Ptr_Dia->RTC_Year = 0;
   
  #if (Coldfire == 1)
  if (!iNesting)
  #endif
     OSExitCritical;
      
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////




 
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      Return Time Function                        /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
 
OSTime OSUptime(void)
{
  return Hora;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      Return Date Function                        /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

OSDate OSUpDate(void)
{
  return Data;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////