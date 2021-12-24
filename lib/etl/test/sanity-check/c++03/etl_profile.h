/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 Bo Rydberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef __ETL_PROFILE_H__
#define __ETL_PROFILE_H__

#define ETL_TARGET_DEVICE_GENERIC
#define ETL_TARGET_OS_NONE
#define ETL_CPP11_SUPPORTED 0
#define ETL_CPP14_SUPPORTED 0
#define ETL_CPP17_SUPPORTED 0
#define ETL_IN_UNIT_TEST
#define ETL_CALLBACK_TIMER_USE_INTERRUPT_LOCK
#define ETL_CALLBACK_TIMER_DISABLE_INTERRUPTS
#define ETL_CALLBACK_TIMER_ENABLE_INTERRUPTS
#define ETL_MESSAGE_TIMER_USE_INTERRUPT_LOCK
#define ETL_MESSAGE_TIMER_DISABLE_INTERRUPTS
#define ETL_MESSAGE_TIMER_ENABLE_INTERRUPTS

#if defined(ETL_FORCE_TEST_CPP03)
  #define ETL_FUNCTION_FORCE_CPP03
  #define ETL_PRIORITY_QUEUE_FORCE_CPP03
  #define ETL_QUEUE_ATOMIC_FORCE_CPP03
  #define ETL_VARIANT_FORCE_CPP03
  #define ETL_VECTOR_FORCE_CPP03
  #define ETL_QUEUE_FORCE_CPP03
  #define ETL_QUEUE_MPMC_MUTEX_FORCE_CPP03
  #define ETL_QUEUE_ISR_FORCE_CPP03
  #define ETL_QUEUE_LOCKED_FORCE_CPP03
  #define ETL_OPTIONAL_FORCE_CPP03
  #define ETL_LARGEST_TYPE_FORCE_CPP03
  #define ETL_TYPE_SELECT_FORCE_CPP03
  #define ETL_UNINITIALIZED_BUFFER_FORCE_CPP03
  #define ETL_CRC_FORCE_CPP03
  #define ETL_MEM_CAST_FORCE_CPP03
  #define ETL_OBSERVER_FORCE_CPP03
  #define ETL_MESSAGE_PACKET_FORCE_CPP03
  #define ETL_OBSERVER_FORCE_CPP03
  #define ETL_MESSAGE_ROUTER_FORCE_CPP03
  #define ETL_FSM_FORCE_CPP03
#endif

#endif
