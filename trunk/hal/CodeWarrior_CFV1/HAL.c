/**
* \file HAL.c
* \brief BRTOS Hardware Abstraction Layer Functions.
*
* This file contain the functions that are processor dependant.
*
*
**/

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
*                                   OS HAL Functions to Coldfire V1
*
*
*   Author:   Gustavo Weber Denardin
*   Revision: 1.0
*   Date:     20/03/2009
*
*********************************************************************************************************/

#include "BRTOS.h"

#pragma warn_implicitconv off
#pragma warn_unusedarg off


#if (SP_SIZE == 32)
  INT32U SPvalue;                             ///< Used to save and restore a task stack pointer
#endif

#if (SP_SIZE == 16)
  INT16U SPvalue;                             ///< Used to save and restore a task stack pointer
#endif




////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      OS Tick Timer Setup                         /////
/////                                                  /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void TickTimerSetup(void)
{
  
  /* ### Init_TPM init code */
  /* TPM1SC: TOF=0,TOIE=0,CPWMS=0,CLKSB=0,CLKSA=0,PS2=0,PS1=0,PS0=0 */
  TPM1SC = 0x00;                       /* Stop and reset counter */
  TPM1MOD = (configCPU_CLOCK_HZ / configTICK_RATE_HZ) >> configTIMER_PRE_SCALER; /* Period value setting */
  (void)(TPM1SC == 0);                 /* Overflow int. flag clearing (first part) */
  /* TPM1SC: TOF=0,TOIE=1,CPWMS=0,CLKSB=0,CLKSA=1,PS2=0,PS1=0,PS0=0 */
  TPM1SC = 0x48 | configTIMER_PRE_SCALER;                       /* Int. flag clearing (2nd part) and timer control register setting */
  
  /* ### */
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      OS RTC Setup                                /////
/////                                                  /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void OSRTCSetup(void)
{  
  /* ### Init_RTC init code */
  /* RTCMOD: RTCMOD=0x63 */
  // Cristal de Refer�ncia = 1Khz
  RTCMOD = (configRTC_CRISTAL_HZ / configRTC_PRE_SCALER);
  /* RTCSC: RTIF=1,RTCLKS=0,RTIE=1,RTCPS=0x0B */
  RTCSC = 0x1B;                                      
  /* ### */

  //OSResetTime(&Hora);
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
#if (NESTING_INT == 1)
#pragma TRAP_PROC
void TickTimer(void)
#else
interrupt void TickTimer(void)
#endif
{
  // ************************
  // Entrada de interrup��o
  // ************************
  OS_INT_ENTER();
  
  // Interrupt handling
  TICKTIMER_INT_HANDLER;

  counter++;
  if (counter == TickCountOverFlow) counter = 0;
  
  // BRTOS TRACE SUPPORT
  #if (OSTRACE == 1) 
      #if(OS_TICK_SHOW == 1) 
          #if(OS_TRACE_BY_TASK == 1)
          Update_OSTrace(0, ISR_TICK);
          #else
          Update_OSTrace(configMAX_TASK_INSTALL - 1, ISR_TICK);
          #endif         
      #endif       
  #endif  

  #if (NESTING_INT == 1)
  OS_ENABLE_NESTING();
  #endif   
    
  // ************************
  // Handler code for the tick
  // ************************
  OS_TICK_HANDLER();
  
  // ************************
  // Interrupt Exit
  // ************************
  OS_INT_EXIT();  
  // ************************  
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////   Software Interrupt to provide Switch Context   /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
 
/************************************************************//**
* \fn interrupt void SwitchContext(void)
* \brief Software interrupt handler routine (Internal kernel function).
*  Used to switch the tasks context.
****************************************************************/
#if (NESTING_INT == 1)
#pragma TRAP_PROC
void SwitchContext(void)
#else
interrupt void SwitchContext(void)
#endif
{
  // ************************
  // Entrada de interrup��o
  // ************************
  OS_INT_ENTER();

  // Interrupt Handling
  
  // ************************
  // Interrupt Exit
  // ************************
  OS_INT_EXIT();  
  // ************************
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////  Task Installation Function                      /////
/////                                                  /////
/////  Parameters:                                     /////
/////  Function pointer, task priority and task name   /////
/////                                                  /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


void CreateVirtualStack(void(*FctPtr)(void), INT16U NUMBER_OF_STACKED_BYTES)
{  
   // First 4 bytes defined to Coldfire Only
   // Format: First 4 bits = processor indicating a two-longword frame, always 0x04 in MCF51QE
   //         Other 4 bits = fault status field, always 0x00 if no error occurred
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 8] = 0x40;
   // Vector
   // The 8-bit vector number, vector[7:0], defines the exception type and is
   // calculated by the processor for all internal faults and represents the
   // value supplied by the interrupt controller in the case of an interrupt
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 7] = 0x80;
   
   // Initial SR Register
   // Interrupts Enabled
   // CCR = 0x00
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 6] = 0x20;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 5] = 0x00;

   // Pointer to Task Entry
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 1] = ((unsigned long) (FctPtr)) & 0x00FF;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 2] = ((unsigned long) (FctPtr)) >> 8;  
   
   #if (NESTING_INT == 1)  
   
   // Initialize registers
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 13] = 0xA1;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 17] = 0xA0;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 21] = 0xD2;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 25] = 0xD1;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 29] = 0xD0;
   
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 33] = 0xA6;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 37] = 0xA5;   
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 41] = 0xA4;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 45] = 0xA3;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 49] = 0xA2;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 53] = 0xD7;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 57] = 0xD6;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 61] = 0xD5;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 65] = 0xD4;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 69] = 0xD3;
   
   #else
   
   // Initialize registers
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 9] = 0xA1;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 13] = 0xA0;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 17] = 0xD2;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 21] = 0xD1;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 25] = 0xD0;
   
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 29] = 0xA6;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 33] = 0xA5;   
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 37] = 0xA4;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 41] = 0xA3;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 45] = 0xA2;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 49] = 0xD7;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 53] = 0xD6;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 57] = 0xD5;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 61] = 0xD4;
   STACK[iStackAddress + NUMBER_OF_STACKED_BYTES - 65] = 0xD3;
   #endif
}





#if (NESTING_INT == 1)

INT16U OS_CPU_SR_Save(void)
{  
  asm
  {
    
        MOVE.W   SR,D0            // Copy SR into D0 
        MOVE.L   D0,-(A7)         // Save D0
        ORI.L    #0x0700,D0       // Disable interrupts
        MOVE.W   D0,SR
        MOVE.L   (A7)+,D0         // Restore D0
  }
}


void OS_CPU_SR_Restore(INT16U)
{  
  asm
  {
    
        MOVE.L   D0,-(A7)         // Save D0
        MOVE.W   2(A7),D0
        MOVE.W    D0,SR
        MOVE.L   (A7)+,D0         // Restore D0
  }
}

  

#endif

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////