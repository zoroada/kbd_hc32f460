/* FreeRTOS 软件定时器：5 ms 键盘任务、单次矩阵消抖。 */
#include "creat_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "quantum.h"
#include "keyboard_matrix.h"

static TimerHandle_t debounce_timer;

/** @brief 5 ms 自动重载定时器回调：执行键盘扫描和 Quantum 后台维护，不周期重发 USB 报告。 */
static void keyboard_timer_callback(TimerHandle_t timer)
{
    keyboard_task();
}

/** @brief 单次消抖定时器超时后提交稳定的矩阵状态。 */
static void debounce_timer_callback(TimerHandle_t timer)
{
    keyboard_task();
}

/** @brief 启动或重置单次消抖定时器。 */
void keyboard_debounce(void)
{
    if (debounce_timer != NULL) {
        (void)xTimerReset(debounce_timer, 0);
    }
}

/** @brief 创建键盘和消抖定时器；调度器由 main 启动。 */
void Creat_app_task(void)
{
    TimerHandle_t keyboard_timer=xTimerCreate("Keyboard",pdMS_TO_TICKS(5),pdTRUE,NULL,keyboard_timer_callback);
    debounce_timer=xTimerCreate("KeyDebounce",pdMS_TO_TICKS(KEYBOARD_DEBOUNCE_MS),pdFALSE,NULL,debounce_timer_callback);
    configASSERT(keyboard_timer!=NULL);
    configASSERT(debounce_timer!=NULL);
    BaseType_t keyboard_started=xTimerStart(keyboard_timer,0);
    configASSERT(keyboard_started==pdPASS);
}
