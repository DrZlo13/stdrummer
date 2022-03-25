#pragma once
#include <core.h>
#include <log.h>
#include <FreeRTOS.h>
#include "event_groups.h"

#define ever \
    ;        \
    ;

#define taskdef(task_body) #task_body, task_body

enum TaskPriority {
    Idle = tskIDLE_PRIORITY,
    Low,
    Mid,
    Normal,
    High,
};

template <size_t task_stack_byte_size> class Task {
public:
    typedef void (*TaskBody)(void* args);

private:
    StaticTask_t task_buffer;
    const char* const task_name = "";
    static const uint16_t task_stack_size = task_stack_byte_size / 4;
    StackType_t task_stack[task_stack_size];
    TaskBody task_body = NULL;
    TaskPriority task_priority = Idle;
    TaskHandle_t task_handle = NULL;
    void* task_argument;

    StaticEventGroup_t task_event_group;
    EventGroupHandle_t task_event_group_handle;

    static void task_inner_body(void* argument) {
        Task<task_stack_byte_size>* _this = static_cast<Task<task_stack_byte_size>*>(argument);

        if(_this->task_body != NULL) {
            _this->task_body(_this->task_argument);
        } else {
            Core::crash("Task launched without body");
        }
        vTaskDelete(NULL);
    }

public:
    Task(const char* name, TaskBody body, TaskPriority priority = Normal)
        : task_name(name)
        , task_body(body)
        , task_priority(priority) {
    }

    ~Task() {
    }

    void start(void* argument = NULL) {
        task_argument = argument;

        task_event_group_handle = xEventGroupCreateStatic(&task_event_group);

        if(!task_event_group_handle) {
            Core::crash("Cannot create task event group");
        }

        task_handle = xTaskCreateStatic(
            task_inner_body,
            task_name,
            task_stack_size,
            this,
            task_priority,
            task_stack,
            &task_buffer);

        if(task_handle != NULL) {
            Log::info("{} started at {:#x}", task_name, (uint32_t)task_handle);
        } else {
            Core::crash("Cannot start task");
        }
    }

    void notify_from_isr() {
        BaseType_t task_woken = pdFALSE;

        // vTaskNotifyGiveFromISR(task_handle, &task_woken);

        uint32_t notify_value = 0x01L;
        xEventGroupSetBitsFromISR(task_event_group_handle, notify_value, &task_woken);

        portYIELD_FROM_ISR(task_woken);
    }

    void notify_from_isr(uint16_t bits) {
        BaseType_t task_woken = pdFALSE;

        // xTaskNotifyFromISR(task_handle, bits, eSetBits, &task_woken);

        uint32_t notify_value = bits;
        notify_value = (notify_value << 8) | 0x01L;
        xEventGroupSetBitsFromISR(task_event_group_handle, notify_value, &task_woken);

        portYIELD_FROM_ISR(task_woken);
    }

    void notify() {
        // xTaskNotify(task_handle, 0, eNoAction);

        uint32_t notify_value = 0x0001L;
        xEventGroupSetBits(task_event_group_handle, notify_value);
    }

    void notify(uint16_t bits) {
        // xTaskNotify(task_handle, bits, eSetBits);

        uint32_t notify_value = bits;
        notify_value = (notify_value << 8) | 0x01L;
        xEventGroupSetBits(task_event_group_handle, notify_value);
    }

    uint16_t notify_wait(uint32_t ms_to_wait = portMAX_DELAY) {
        uint32_t notify_value;
        uint32_t ticks_to_wait;
        if(ms_to_wait == portMAX_DELAY) {
            ticks_to_wait = portMAX_DELAY;
        } else {
            ticks_to_wait = pdMS_TO_TICKS(ms_to_wait);
        }

        notify_value = xEventGroupWaitBits(
            task_event_group_handle, 0x00FFFFFF, pdTRUE, pdFALSE, ticks_to_wait);
        notify_value = (notify_value & 0x00FFFF00L) >> 8;

        // notify_value = ulTaskNotifyTake(pdTRUE, ticks_to_wait);
        return notify_value;
    }
};