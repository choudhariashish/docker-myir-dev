#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "fsl_pwm.h"
#include "fsl_mu.h"

#define DEMO_PWM_BASEADDR   PWM1
#define DEMO_PWM_IRQn       PWM1_IRQn
#define DEMO_PWM_IRQHandler PWM1_IRQHandler

// #define DEMO_PWM_BASEADDR   PWM4
// #define DEMO_PWM_IRQn       PWM4_IRQn
// #define DEMO_PWM_IRQHandler PWM4_IRQHandler

#define PWM_PERIOD_VALUE    80

volatile uint32_t pwmDutycycle = 0U;
volatile bool pwmDutyUp        = true; /* Indicate PWM Duty cycle is increase or decrease */

struct IPCMsg
{
    char ready;
    char payload[511];
};

struct IPC
{
    struct IPCMsg aTobMsg;
    struct IPCMsg bToaMsg;
};
typedef struct IPC IPC_t;

IPC_t *ipcObj = (IPC_t *)0x80100000;


void DEMO_PWM_IRQHandler(void)
{
    /* Gets interrupt kPWM_FIFOEmptyFlag */
    if (PWM_GetStatusFlags(DEMO_PWM_BASEADDR) & kPWM_FIFOEmptyFlag)
    {
        if (pwmDutyUp)
        {
            /* Increase duty cycle until it reach limited value. */
            if (++pwmDutycycle > PWM_PERIOD_VALUE)
            {
                pwmDutycycle = PWM_PERIOD_VALUE;
                pwmDutyUp    = false;
            }
        }
        else
        {
            /* Decrease duty cycle until it reach limited value. */
            if (--pwmDutycycle == 0U)
            {
                pwmDutyUp = true;
            }
        }
        /* Write duty cycle to PWM sample register.  */
        PWM_SetSampleValue(DEMO_PWM_BASEADDR, pwmDutycycle);
        /* Clear kPWM_FIFOEmptyFlag */
        PWM_clearStatusFlags(DEMO_PWM_BASEADDR, kPWM_FIFOEmptyFlag);
    }
    SDK_ISR_EXIT_BARRIER;
}

int main(void)
{
    pwm_config_t pwmConfig;

    BOARD_RdcInit();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();
    MU_Init(MUB);

    PWM_GetDefaultConfig(&pwmConfig);
    pwmConfig.enableWaitMode = true;
    PWM_Init(DEMO_PWM_BASEADDR, &pwmConfig);
    PWM_EnableInterrupts(DEMO_PWM_BASEADDR, kPWM_FIFOEmptyInterruptEnable);
    for (pwmDutycycle = 0u; pwmDutycycle < 3; pwmDutycycle++)
    {
        PWM_SetSampleValue(DEMO_PWM_BASEADDR, pwmDutycycle);
    }
    if (PWM_GetStatusFlags(DEMO_PWM_BASEADDR))
    {
        PWM_clearStatusFlags(DEMO_PWM_BASEADDR, kPWM_FIFOEmptyFlag | kPWM_RolloverFlag | kPWM_CompareFlag | kPWM_FIFOWriteErrorFlag);
    }
    PWM_SetPeriodValue(DEMO_PWM_BASEADDR, PWM_PERIOD_VALUE);
    EnableIRQ(DEMO_PWM_IRQn);
    PWM_StartTimer(DEMO_PWM_BASEADDR);

    PRINTF("hello world.\r\n");

    while (1)
    {
        if (0x01 == ipcObj->aTobMsg.ready)
        {
            PRINTF("rx:%x %x %x %x\r\n", ipcObj->aTobMsg.payload[0],
                                         ipcObj->aTobMsg.payload[1],
                                         ipcObj->aTobMsg.payload[2],
                                         ipcObj->aTobMsg.payload[3]);
            ipcObj->aTobMsg.ready = 0x00;
        }

        if (0x01 == ipcObj->bToaMsg.ready)
        {
            PRINTF("rx:%x %x %x %x\r\n", ipcObj->bToaMsg.payload[0],
                                         ipcObj->bToaMsg.payload[1],
                                         ipcObj->bToaMsg.payload[2],
                                         ipcObj->bToaMsg.payload[3]);
            ipcObj->bToaMsg.ready = 0x00;
        }
    }
}
