/*
 *  $Id$
 */

/*
 * tst-raw.c
 *
 * Copyright (c) 2002-2007 Volkswagen Group Electronic Research
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Volkswagen nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * Alternatively, provided that this notice is retained in full, this
 * software may be distributed under the terms of the GNU General
 * Public License ("GPL") version 2, in which case the provisions of the
 * GPL apply INSTEAD OF those given above.
 *
 * The provided data structures and external interfaces from this code
 * are not restricted to be used by modules with a GPL compatible license.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * Send feedback to <linux-can@vger.kernel.org>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h> 

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#define _kbhit kbhit
#define _getch getchar

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    int s;
    struct sockaddr_can addr;
    struct can_filter rfilter[4];
    struct can_frame frame;
    int nbytes, i;
    struct ifreq ifr;
    char *ifname = "vcan2";
    int ifindex;
    int opt;
    int after_read;
    int flag;
    struct timeval tv;

    /* sockopt test */
    int loopback = 0;
    int set_loopback = 0;
    int recv_own_msgs = 0;
    int set_recv_own_msgs = 0;
    int send_one_frame = 0;
    int ignore_errors = 0;
    int show_frame = 0;
    int enable_block = 0;
    int enable_delay = 0;

    unsigned int can_sid = 0;
    unsigned int can_eid = 0;
    int send_stop_frame = 0;
    int so_rcvbuf;
    socklen_t socklen;
    
    int bstop = 0;

    while ((opt = getopt(argc, argv, "i:l:r:semb")) != -1) {
        switch (opt) {

        case 'i':
            ifname = optarg;
            break;

        case 'l':
            loopback = atoi(optarg);
            set_loopback = 1;
            break;

        case 'r':
            recv_own_msgs = atoi(optarg);
            set_recv_own_msgs = 1;
            break;

        case 's':
            send_one_frame = 1;
            break;

        case 'e':
            ignore_errors = 1;
            break;
        
        case 'm':
            show_frame = 1;
            break;
        
        case 'b':
            enable_block = 1;
            break;
        
        case 'd':
            enable_delay = 1;
            break;

        default:
            fprintf(stderr, "Unknown option %c\n", opt);
            break;
        }
    }


    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("socket");
        return 1;
    }

    // enable non block operation
    if (enable_block == 0)
    {
        printf("enable non block operation \n");
        flag = fcntl(s, F_GETFL, 0);
        fcntl(s, F_SETFL, flag | O_NONBLOCK);
    }
    else
    {
        printf("enable block operation \n");
        // if not enable non block operation, you can setting timeout value for read...etc operations.
        // setting timeout value
        tv.tv_sec = 0;
        tv.tv_usec = 1;
        setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    }

    rfilter[0].can_id   = 0x123;
    rfilter[0].can_mask = CAN_SFF_MASK;
    rfilter[1].can_id   = 0x200;
    rfilter[1].can_mask = 0x700;
    rfilter[2].can_id   = 0x80123456;
    rfilter[2].can_mask = 0x1FFFF000;
    rfilter[3].can_id   = 0x80333333;
    rfilter[3].can_mask = CAN_EFF_MASK;

    //setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    so_rcvbuf = 0;
    socklen = sizeof(so_rcvbuf);
    nbytes = getsockopt(s, SOL_SOCKET, SO_RCVBUF, &so_rcvbuf, &socklen);
    printf("get so_rcvbuf:%i \n", so_rcvbuf);

    so_rcvbuf = 1024 * 1000;
    nbytes = setsockopt(s, SOL_SOCKET, SO_RCVBUF, &so_rcvbuf, sizeof(so_rcvbuf));
    printf("set so_rcvbuf:%i \n", so_rcvbuf);

    nbytes = getsockopt(s, SOL_SOCKET, SO_RCVBUF, &so_rcvbuf, &socklen);
    printf("get so_rcvbuf:%i \n", so_rcvbuf);

    if(set_loopback)
        setsockopt(s, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));

    if(set_recv_own_msgs)
        setsockopt(s, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &recv_own_msgs, sizeof(recv_own_msgs));

    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr);
    ifindex = ifr.ifr_ifindex;

    

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    srand( (unsigned)time( NULL ) );
    
    can_sid = 0;
    can_eid = 0;

    after_read = 0;
    while (1) {
        if(send_stop_frame)
        {
            // to send stop frame
            while(1)
            {
                //usleep(50);
                nbytes = write(s, &frame, sizeof(struct can_frame));
                if (nbytes > 0)
                   break;
            }
            // to receive the following frame after sending a stop frame
            while(1)
            {
                if ((nbytes = read(s, &frame, sizeof(struct can_frame))) < 0)
                {
                    if (errno == EAGAIN)
                    {
                        continue;
                    }
                    printf("read errno: %i \n", errno);
                        return 1;
                }
                /*
                printf("after stop farme, ");
                if (frame.can_id & CAN_EFF_FLAG)
                    printf("EFF %8X ", frame.can_id & CAN_EFF_MASK);
                else
                    printf("SFF %3X ", frame.can_id & CAN_SFF_MASK);
            
                printf("[%d] ", frame.can_dlc);
                if (frame.can_id & CAN_RTR_FLAG)
                {
                    printf("remote request ");
                }
                else
                {
                    for (i = 0; i < frame.can_dlc; i++)
                    {
                        printf("%02X ", frame.data[i]);
                    }
                }
                printf("\n");
                */
            }
        }
        
        if (enable_delay)
        {
            usleep(50);
        }
        
        if(send_one_frame && after_read)
        {
            memset(&frame, 0, sizeof(struct can_frame));
            
            frame.can_id  = can_eid | CAN_EFF_FLAG;

            frame.can_dlc = rand()%9;
            for (i = 0; i < 8; i++)
            {
                frame.data[i] = can_eid;
            }
            
            if (show_frame)
            {
                printf("send    ");
                if (frame.can_id & CAN_EFF_FLAG)
                    printf("EFF %8X ", frame.can_id & CAN_EFF_MASK);
                else
                    printf("SFF %3X ", frame.can_id & CAN_SFF_MASK);
            
                printf("[%d] ", frame.can_dlc);
                if (frame.can_id & CAN_RTR_FLAG)
                {
                    printf("remote request ");
                }
                else
                {
                    for (i = 0; i < frame.can_dlc; i++)
                    {
                        printf("%02X ", frame.data[i]);
                    }
                }
            }
            
            while(1)
            {
                //usleep(50);
                nbytes = write(s, &frame, sizeof(struct can_frame));
                if (nbytes > 0)
                {
                    can_eid += 1;
                    if (show_frame)
                    {
                        printf("OK \n");
                    }
                    break;
                }
            }
        }
        
        bstop = 0;
        while(1)
        {
            nbytes = read(s, &frame, sizeof(struct can_frame));
            if (nbytes < 0)
            {
                if (errno == EAGAIN)
                {
                    break;
                }
                printf("read errno: %i \n", errno);
                //perror("read");
                if (!ignore_errors)
                    return 1;
            }
            else if (nbytes < sizeof(struct can_frame))
            {
                fprintf(stderr, "read: incomplete CAN frame\n");
                return 1;
            }
            else
            {
                // tst-raw-1 expect to recevie standard frame from tst-raw-0.
                // and can_sid is in increasing order.
                // if the value of can_sid is not expected, tst-raw-1 will send a stop frame to tst-raw-0 and dump the following frame.
                if (frame.can_id & CAN_EFF_FLAG)
                {
                    printf("this is a stop frame EFF %8X \n", frame.can_id & CAN_EFF_MASK);
                    bstop = 1;
                    break;
                }
                else
                {
                    //printf("%3X  ", frame.can_id & CAN_SFF_MASK);
                    if ((can_sid & CAN_SFF_MASK) != (frame.can_id & CAN_SFF_MASK))
                    {
                        printf("can_sid(%3X) != frame.can_id(%3X) \n", can_sid, frame.can_id);
                        send_stop_frame = 1;
                        break;
                    }
                    else
                    {
                        if (show_frame)
                        {
                            printf("receive ");
                            if (frame.can_id & CAN_EFF_FLAG)
                                printf("EFF %8X ", frame.can_id & CAN_EFF_MASK);
                            else
                                printf("SFF %3X ", frame.can_id & CAN_SFF_MASK);
                        
                            printf("[%d] ", frame.can_dlc);
                            if (frame.can_id & CAN_RTR_FLAG)
                            {
                                printf("remote request ");
                            }
                            else
                            {
                                for (i = 0; i < frame.can_dlc; i++)
                                {
                                    printf("%02X ", frame.data[i]);
                                }
                            }
                            printf("\n");
                        }
                    }
                }
                
                //fflush(stdout);
                after_read = 1;
                can_sid += 1;
            }
            if (bstop == 1)
            {
                break;
            }
        }
    }

    close(s);
    printf("can_sid=%u can_eid=%u \n", can_sid, can_eid);

    return 0;
}
