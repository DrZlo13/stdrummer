#include <hal/hal.h>
#include <cmsis_os.h>
#include "log.h"

extern "C" void Error_Handler(void);

namespace Core {

//-----------------------------cmsis_os2.c-------------------------------
// helpers to get isr context
// get arch
#ifndef __ARM_ARCH_6M__
#define __ARM_ARCH_6M__ 0
#endif
#ifndef __ARM_ARCH_7M__
#define __ARM_ARCH_7M__ 0
#endif
#ifndef __ARM_ARCH_7EM__
#define __ARM_ARCH_7EM__ 0
#endif
#ifndef __ARM_ARCH_8M_MAIN__
#define __ARM_ARCH_8M_MAIN__ 0
#endif
#ifndef __ARM_ARCH_7A__
#define __ARM_ARCH_7A__ 0
#endif

// get masks
#if((__ARM_ARCH_7M__ == 1U) || (__ARM_ARCH_7EM__ == 1U) || (__ARM_ARCH_8M_MAIN__ == 1U))
#define IS_IRQ_MASKED() ((__get_PRIMASK() != 0U) || (__get_BASEPRI() != 0U))
#elif(__ARM_ARCH_6M__ == 1U)
#define IS_IRQ_MASKED() (__get_PRIMASK() != 0U)
#elif(__ARM_ARCH_7A__ == 1U)
/* CPSR mask bits */
#define CPSR_MASKBIT_I 0x80U

#define IS_IRQ_MASKED() ((__get_CPSR() & CPSR_MASKBIT_I) != 0U)
#else
#define IS_IRQ_MASKED() (__get_PRIMASK() != 0U)
#endif

// get is irq mode
#if(__ARM_ARCH_7A__ == 1U)
/* CPSR mode bitmasks */
#define CPSR_MODE_USER 0x10U
#define CPSR_MODE_SYSTEM 0x1FU

#define IS_IRQ_MODE() ((__get_mode() != CPSR_MODE_USER) && (__get_mode() != CPSR_MODE_SYSTEM))
#else
#define IS_IRQ_MODE() (__get_IPSR() != 0U)
#endif

// added osKernelGetState(), because KernelState is a static var
#define IS_IRQ() (IS_IRQ_MODE() || (IS_IRQ_MASKED() && (osKernelGetState() == osKernelRunning)))
//-------------------------end of cmsis_os2.c----------------------------

static const char* irq_get_name(IRQn_Type irq) {
    switch(irq) {
    case NonMaskableInt_IRQn:
        return "NonMaskableInt";
    case MemoryManagement_IRQn:
        return "MemoryManagement";
    case BusFault_IRQn:
        return "BusFault";
    case UsageFault_IRQn:
        return "UsageFault";
    case SVCall_IRQn:
        return "SVCall";
    case DebugMonitor_IRQn:
        return "DebugMonitor";
    case PendSV_IRQn:
        return "PendSV";
    case SysTick_IRQn:
        return "SysTick";
    case WWDG_IRQn:
        return "WWDG";
    case PVD_IRQn:
        return "PVD";
    case TAMP_STAMP_IRQn:
        return "TAMP_STAMP";
    case RTC_WKUP_IRQn:
        return "RTC_WKUP";
    case FLASH_IRQn:
        return "FLASH";
    case RCC_IRQn:
        return "RCC";
    case EXTI0_IRQn:
        return "EXTI0";
    case EXTI1_IRQn:
        return "EXTI1";
    case EXTI2_IRQn:
        return "EXTI2";
    case EXTI3_IRQn:
        return "EXTI3";
    case EXTI4_IRQn:
        return "EXTI4";
    case DMA1_Stream0_IRQn:
        return "DMA1_Stream0";
    case DMA1_Stream1_IRQn:
        return "DMA1_Stream1";
    case DMA1_Stream2_IRQn:
        return "DMA1_Stream2";
    case DMA1_Stream3_IRQn:
        return "DMA1_Stream3";
    case DMA1_Stream4_IRQn:
        return "DMA1_Stream4";
    case DMA1_Stream5_IRQn:
        return "DMA1_Stream5";
    case DMA1_Stream6_IRQn:
        return "DMA1_Stream6";
    case ADC_IRQn:
        return "ADC";
    case EXTI9_5_IRQn:
        return "EXTI9_5";
    case TIM1_BRK_TIM9_IRQn:
        return "TIM1_BRK_TIM9";
    case TIM1_UP_TIM10_IRQn:
        return "TIM1_UP_TIM10";
    case TIM1_TRG_COM_TIM11_IRQn:
        return "TIM1_TRG_COM_TIM11";
    case TIM1_CC_IRQn:
        return "TIM1_CC";
    case TIM2_IRQn:
        return "TIM2";
    case TIM3_IRQn:
        return "TIM3";
    case TIM4_IRQn:
        return "TIM4";
    case I2C1_EV_IRQn:
        return "I2C1_EV";
    case I2C1_ER_IRQn:
        return "I2C1_ER";
    case I2C2_EV_IRQn:
        return "I2C2_EV";
    case I2C2_ER_IRQn:
        return "I2C2_ER";
    case SPI1_IRQn:
        return "SPI1";
    case SPI2_IRQn:
        return "SPI2";
    case USART1_IRQn:
        return "USART1";
    case USART2_IRQn:
        return "USART2";
    case EXTI15_10_IRQn:
        return "EXTI15_10";
    case RTC_Alarm_IRQn:
        return "RTC_Alarm";
    case OTG_FS_WKUP_IRQn:
        return "OTG_FS_WKUP";
    case DMA1_Stream7_IRQn:
        return "DMA1_Stream7";
    case SDIO_IRQn:
        return "SDIO";
    case TIM5_IRQn:
        return "TIM5";
    case SPI3_IRQn:
        return "SPI3";
    case DMA2_Stream0_IRQn:
        return "DMA2_Stream0";
    case DMA2_Stream1_IRQn:
        return "DMA2_Stream1";
    case DMA2_Stream2_IRQn:
        return "DMA2_Stream2";
    case DMA2_Stream3_IRQn:
        return "DMA2_Stream3";
    case DMA2_Stream4_IRQn:
        return "DMA2_Stream4";
    case OTG_FS_IRQn:
        return "OTG_FS";
    case DMA2_Stream5_IRQn:
        return "DMA2_Stream5";
    case DMA2_Stream6_IRQn:
        return "DMA2_Stream6";
    case DMA2_Stream7_IRQn:
        return "DMA2_Stream7";
    case USART6_IRQn:
        return "USART6";
    case I2C3_EV_IRQn:
        return "I2C3_EV";
    case I2C3_ER_IRQn:
        return "I2C3_ER";
    case FPU_IRQn:
        return "FPU";
    case SPI4_IRQn:
        return "SPI4";
    case SPI5_IRQn:
        return "SPI5";
    default:
        return "UNKNOWN";
    }
}

static bool task_is_isr_context(void) {
    return IS_IRQ();
}

static void transmit_current_tick() {
    char tick[] = "-2147483648";
    itoa(HalTime::tick(), tick, 10);
    uart_debug.transmit(tick);
}

static void transmit_crash_message() {
    transmit_current_tick();
    uart_debug.transmit(" " LOG_COLOR_E);
    uart_debug.transmit("[Owo] Big bad CRASH happened");
    uart_debug.transmit(LOG_COLOR_RESET "\r\n");
}

static void transmit_interrupt_info() {
    uint32_t current_interrupt = __get_IPSR();
    char tmp_str[] = "-2147483648";
    itoa(current_interrupt, tmp_str, 10);

    transmit_current_tick();
    uart_debug.transmit(" " LOG_COLOR_E);
    uart_debug.transmit("[ISR]");
    uart_debug.transmit("[");
    uart_debug.transmit(tmp_str);
    uart_debug.transmit(":");
    uart_debug.transmit(
        irq_get_name(static_cast<IRQn_Type>(current_interrupt - NVIC_USER_IRQ_OFFSET)));
    uart_debug.transmit("] ");
    uart_debug.transmit(LOG_COLOR_RESET);
}

static void transmit_heap_info(const char* log_color = LOG_COLOR_E) {
    char tmp_str[] = "-2147483648";

    transmit_current_tick();
    uart_debug.transmit(log_color);
    uart_debug.transmit(" [heap] ");
    uart_debug.transmit(LOG_COLOR_RESET);

    uart_debug.transmit("total ");
    itoa(configTOTAL_HEAP_SIZE, tmp_str, 10);
    uart_debug.transmit(tmp_str);

    uart_debug.transmit(", free ");
    itoa(xPortGetFreeHeapSize(), tmp_str, 10);
    uart_debug.transmit(tmp_str);

    uart_debug.transmit(", watermark ");
    itoa(xPortGetMinimumEverFreeHeapSize(), tmp_str, 10);
    uart_debug.transmit(tmp_str);

    uart_debug.transmit("\r\n");
}

static void transmit_stack_info(const char* task_name, const char* log_color = LOG_COLOR_E) {
    // TaskStatus_t task_status;
    // vTaskGetInfo(NULL, &task_status, pdTRUE, eInvalid);
    char tmp_str[] = "-2147483648";

    transmit_current_tick();
    uart_debug.transmit(log_color);

    uart_debug.transmit(" [");
    if(task_name != NULL) {
        uart_debug.transmit(task_name);
    } else {
        uart_debug.transmit("???");
    }
    uart_debug.transmit("][stack] ");
    uart_debug.transmit(LOG_COLOR_RESET);

    uart_debug.transmit("watermark ");
    itoa(uxTaskGetStackHighWaterMark(NULL) * 4, tmp_str, 10);
    uart_debug.transmit(tmp_str);

    uart_debug.transmit("\r\n");
}

void crash(const char* message) {
    bool is_isr = task_is_isr_context();
    __disable_irq();

    transmit_crash_message();
    transmit_heap_info();

    if(is_isr) {
        transmit_interrupt_info();
    } else {
        const char* name = osThreadGetName(osThreadGetId());
        transmit_stack_info(name);

        transmit_current_tick();
        uart_debug.transmit(" " LOG_COLOR_E);
        if(name == NULL) {
            uart_debug.transmit("[???] ");
        } else {
            uart_debug.transmit("[");
            uart_debug.transmit(name);
            uart_debug.transmit("] ");
        }
        uart_debug.transmit(LOG_COLOR_RESET);
    }

    if(message != NULL) {
        uart_debug.transmit(message);
        uart_debug.transmit("\r\n");
    } else {
        uart_debug.transmit("¯\\(ツ)/¯\r\n");
    }

    Error_Handler();
}

void log_stack() {
    const char* name = osThreadGetName(osThreadGetId());
    transmit_stack_info(name, LOG_COLOR_I);
}

void log_heap() {
    transmit_heap_info(LOG_COLOR_I);
}

}

extern "C" void core_crash(const char* message) {
    Core::crash(message);
}
