#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include "esp_log.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "portmacro.h"

#include <deque>
#include <functional>
#include <mutex>

template<typename T> class TaskQueue
{
    using Handler = std::function<void(T)>;
    public:
        TaskQueue(int stack_allocation_size, Handler handler)
        : handler_(handler)
        {
            queue_ = xQueueGenericCreate(1, sizeof(T), queueQUEUE_TYPE_BASE);
            xTaskCreate(
                job_handler,
                "TaskQueue_Task",
                stack_allocation_size,
                (void *)this,
                3,
                &task_
            );
            esp_task_wdt_add(task_);
        }
        ~TaskQueue()
        {
        }

        void send(T data)
        {
            xQueueGenericSend(queue_, (void*)&data, portMAX_DELAY, queueOVERWRITE);
        }

        void job()
        {
            T data;
            while(true) {
                if (xQueueReceive(queue_, (void*)&data, pdMS_TO_TICKS(1000)) == pdTRUE) {
                    (handler_) (data);
                }
                esp_task_wdt_reset();
            }
        }
    private:
        TaskHandle_t task_;
        QueueHandle_t queue_;
        Handler handler_;

        static void job_handler(void* data)
        {
            auto task_queue = static_cast<TaskQueue<T>*>(data);
            task_queue->job();
        }
};

#endif
