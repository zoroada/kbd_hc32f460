/* USB 板级实现：配置系统/USB 时钟、引脚和中断，提供 USB 底层所需延时。 */
#include "hc32_ll.h"
#include "usb_bsp.h"
#include "usb_dev_int.h"
extern usb_core_instance usb_dev;
/** @brief USB 中断入口：把硬件中断交给 HC32 USB 核心处理，由核心调用类回调。 */
static void keyboard_usb_interrupt(void) { usb_isr_handler(&usb_dev); }
/** @brief 初始化板级时钟，并根据 12 MHz 晶振配置 USB 的 48 MHz PLL 时钟。 */
void bsp_init(void)
{
    LL_PERIPH_WE(LL_PERIPH_GPIO|LL_PERIPH_EFM|LL_PERIPH_FCG|LL_PERIPH_PWC_CLK_RMU|LL_PERIPH_SRAM);
    BSP_CLK_Init();
    stc_clock_pllx_init_t pll;
    CLK_PLLxStructInit(&pll);
    pll.u8PLLState=CLK_PLLX_ON;
    pll.PLLCFGR=0;
    pll.PLLCFGR_f.PLLM=3-1;
    pll.PLLCFGR_f.PLLN=84-1;
    pll.PLLCFGR_f.PLLP=7-1;
    pll.PLLCFGR_f.PLLQ=7-1;
    pll.PLLCFGR_f.PLLR=7-1;
    CLK_PLLxInit(&pll);
    CLK_SetUSBClockSrc(CLK_USBCLK_PLLXP);
}
/** @brief 配置 PA11/PA12 数据线、PA09 VBUS 功能和 USBFS 外设时钟。 */
void usb_pin_init(void)
{
    stc_gpio_init_t cfg;
    GPIO_StructInit(&cfg); cfg.u16PinAttr=PIN_ATTR_ANALOG;
    GPIO_Init(GPIO_PORT_A,GPIO_PIN_11|GPIO_PIN_12,&cfg);
    GPIO_SetFunc(GPIO_PORT_A,GPIO_PIN_09,GPIO_FUNC_10);
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USBFS,ENABLE);
}
/** @brief 将 USBFS 全局中断绑定到 INT030，并设置优先级、清除挂起标志后启用。 */
void usb_bsp_nvicconfig(usb_core_instance *pdev)
{
    (void)pdev;
    stc_irq_signin_config_t irq={INT_SRC_USBFS_GLB,INT030_IRQn,keyboard_usb_interrupt};
    INTC_IrqSignIn(&irq);
    NVIC_ClearPendingIRQ(INT030_IRQn);
    NVIC_SetPriority(INT030_IRQn,DDL_IRQ_PRIO_15);
    NVIC_EnableIRQ(INT030_IRQn);
}
/** @brief 向 USB 底层提供微秒延时，单位为 us。 */
void usb_udelay(uint32_t us) { DDL_DelayUS(us); }
/** @brief 向 USB 底层提供毫秒延时，单位为 ms。 */
void usb_mdelay(uint32_t ms) { DDL_DelayMS(ms); }
