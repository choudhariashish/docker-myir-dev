/*
 * devmem2.c: Simple program to read/write from/to any location in memory.
 *
 *  Copyright (C) 2000, Jan-Derk Bakker (J.D.Bakker@its.tudelft.nl)
 *
 *
 * This software has been developed for the LART computing board
 * (http://www.lart.tudelft.nl/). The development has been sponsored by
 * the Mobile MultiMedia Communications (http://www.mmc.tudelft.nl/)
 * and Ubiquitous Communications (http://www.ubicom.tudelft.nl/)
 * projects.
 *
 * The author can be reached at:
 *
 *  Jan-Derk Bakker
 *  Information and Communication Theory Group
 *  Faculty of Information Technology and Systems
 *  Delft University of Technology
 *  P.O. Box 5031
 *  2600 GA Delft
 *  The Netherlands
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
  
#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
 
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#define IPC_START                  0x80100000
#define IPC_A_TO_B_BUFFER_READY    0x80100000
#define IPC_A_TO_B_BUFFER          (0x80100000 + 0x00000004)

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

int main(int argc, char **argv) {
    int fd;
    void *map_base, *virt_addr; 
	off_t target;
	int access_type = 'w';

	target = 0x80100000;

    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
    printf("/dev/mem opened.\n"); 
    fflush(stdout);
    
    /* Map one page */
    map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *) -1) FATAL;
    printf("Memory mapped at address %p.\n", map_base); 
    fflush(stdout);

    virt_addr = map_base + (target & MAP_MASK);

    IPC_t ipc = {0};

    ipc.aTobMsg.ready = 0x00;
    ipc.aTobMsg.payload[0] = 0x01;
    ipc.aTobMsg.payload[1] = 0x02;
    ipc.aTobMsg.payload[2] = 0x03;
    ipc.aTobMsg.payload[3] = 0x04;

    *((struct IPCMsg *) virt_addr) = ipc.aTobMsg;
    *((char *) virt_addr) = 0x01;

    ipc.bToaMsg.ready = 0x00;
    ipc.bToaMsg.payload[0] = 0x05;
    ipc.bToaMsg.payload[1] = 0x06;
    ipc.bToaMsg.payload[2] = 0x07;
    ipc.bToaMsg.payload[3] = 0x08;

    *((struct IPCMsg *) (virt_addr + sizeof(ipc.aTobMsg))) = ipc.bToaMsg;
    *((char *) (virt_addr + sizeof(ipc.aTobMsg))) = 0x01;

    fflush(stdout);

	if(munmap(map_base, MAP_SIZE) == -1) FATAL;
    close(fd);
    return 0;
}
