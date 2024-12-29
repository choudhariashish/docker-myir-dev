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
//        ch = GETCHAR();
//        PUTCHAR(ch);

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

/*
#define IPC_NODE_A     1
#define IPC_NODE_B     2
#define IPC_NODE_ID    IPC_NODE_B

#define IPC_MSG_PAYLOAD_SIZE    1024
#define IPC_MSG_QSIZE           10

struct Msg
{
    uint32_t id;
    char payload[IPC_MSG_PAYLOAD_SIZE];
    uint32_t crc;
};
typedef struct Msg Msg_t;

struct IPCMsgQueue
{
    int count;
    int head;
    int tail;
    Msg_t msgs[IPC_MSG_QSIZE];
};
typedef struct IPCMsgQueue IPCMsgQueue_t;

struct IPCMsg
{
    IPCMsgQueue_t aTobQ;
    IPCMsgQueue_t bToaQ;
};
typedef struct IPCMsg IPCMsg_t;

void IPCMsgInitQ(IPCMsgQueue_t *q)
{
    q->count = 0;
    q->head = 0;
    q->tail = 0;
}

int IPCMsgSend(IPCMsgQueue_t *q, Msg_t *m)
{
    // Q full case.
    if (q->head == q->tail && q->count != 0)
    {
        return 1;
    }
    q->head = (q->head + 1) % IPC_MSG_QSIZE;
    memcpy(&q->msgs[q->head], m, sizeof(Msg_t));
    q->count++;
    return 0;
}

int IPCMsgGet(IPCMsgQueue_t *q, Msg_t *m)
{
    if (NULL == m)
    {
        return 1;
    }

    // Q empty case.
    if (q->tail == q->head && q->count == 0)
    {
        return 1;
    }

    q->tail = (q->tail + 1) % IPC_MSG_QSIZE;
    memcpy(m, &q->msgs[q->tail], sizeof(Msg_t));
    q->count--;
    return 0;
}

IPCMsg_t *ipcMsg = (IPCMsg_t *)0x80100000;
*/