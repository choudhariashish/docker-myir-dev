/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "fsl_mu.h"

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

int main(void)
{
    // char ch;

    /* Init board hardware. */
    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();

    MU_Init(MUB);

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
