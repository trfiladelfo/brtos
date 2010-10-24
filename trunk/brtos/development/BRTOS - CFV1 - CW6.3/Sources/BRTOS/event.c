/**
* \file event.c
* \brief BRTOS Event Handler Variable Declarations
*
* Declare the control block variables to semaphores, mutexes, mailboxes and queues
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
*                                 OS Event Handle Structs Declarations
*
*
*   Author:   Gustavo Weber Denardin
*   Revision: 1.1
*   Date:     11/03/2010
*
*   Authors:  Carlos Henrique Barriquelo e Gustavo Weber Denardin
*   Revision: 1.2
*   Date:     01/10/2010
*
*   Authors:  Carlos Henrique Barriquelo e Gustavo Weber Denardin
*   Revision: 1.3
*   Date:     11/10/2010
*
*   Authors:  Carlos Henrique Barriquelo e Gustavo Weber Denardin
*   Revision: 1.4
*   Date:     19/10/2010
*
*   Authors:  Carlos Henrique Barriquelo e Gustavo Weber Denardin
*   Revision: 1.41
*   Date:     20/10/2010
*
*********************************************************************************************************/


#include "event.h"
#include "queue.h"

#include <stdlib.h>





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      Semaphore Control Block Declaration         /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

#if (BRTOS_SEM_EN == 1)
  /// Semahore Control Block
  BRTOS_Sem        BRTOS_Sem_Table[BRTOS_MAX_SEM];      // Table of EVENT control blocks
#endif

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      Mutex Control Block Declaration             /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

#if (BRTOS_MUTEX_EN == 1)
  /// Mutex Control Block
  BRTOS_Mutex      BRTOS_Mutex_Table[BRTOS_MAX_MUTEX];    // Table of EVENT control blocks
#endif

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      Mbox Control Block Declaration              /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

#if (BRTOS_MBOX_EN == 1)
  /// MailBox Control Block
  BRTOS_Mbox       BRTOS_Mbox_Table[BRTOS_MAX_MBOX];     // Table of EVENT control blocks
#endif

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      Queue Control Block Declaration             /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

#if (BRTOS_QUEUE_EN == 1)
  /// Queue Control Block
  BRTOS_Queue      BRTOS_Queue_Table[BRTOS_MAX_QUEUE];    // Table of EVENT control blocks
#endif

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      Initialize Block List Control               /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void initEvents(void)
{
  INT8U i=0;
  
  #if (BRTOS_SEM_EN == 1)
    for(i=0;i<BRTOS_MAX_SEM;i++)
      BRTOS_Sem_Table[i].OSEventAllocated = 0;
  #endif
  
  #if (BRTOS_MUTEX_EN == 1)
    for(i=0;i<BRTOS_MAX_MUTEX;i++)
      BRTOS_Mutex_Table[i].OSEventAllocated = 0;
  #endif
    
  #if (BRTOS_MBOX_EN == 1)
    for(i=0;i<BRTOS_MAX_MBOX;i++)
      BRTOS_Mbox_Table[i].OSEventAllocated = 0;    
  #endif
  
  #if (BRTOS_QUEUE_EN == 1)
    for(i=0;i<BRTOS_MAX_QUEUE;i++)
      BRTOS_Queue_Table[i].OSEventAllocated = 0;    
  #endif
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////////
//                                                                                //
// Uma vari�vel global deve ser criada para controlar a entrada                   //
// em multiplas interrup��es. A troca de contexto s� deve ocorrer                 //
// quando est� vari�vel for igual a zero, ou seja, quando todas as                //
// interrup��es foram tratadas                                                    //
//                                                                                //
// Devem ser criadas fun��es para retirar e colocar dados no buffer circular      //
// Devem ser criadas fun��es                                                      //
//                                                                                //
// OBS:
// Uma interrup��o s� poder� gerar altera��o de contexto se a tarefa que acordou  //
// com o evento for de maior prioridade que a tarefa interrompida                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////
