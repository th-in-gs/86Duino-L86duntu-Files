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

#define getch getchar

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
    char *ifname = "can0";
    int s, flag, so_rcvbuf, nbytes, ifindex;
    socklen_t socklen;
    struct ifreq ifr;
    struct sockaddr_can addr;
    struct can_frame frame;
    int i, j, c, packlen, type, datalen;

	if (argc < 4) {
		printf("test [packlen] [type] [datalen]\n");
		printf("\n");
		printf("    [packlen] - If press \'T\', it will transmit 'packlen' number packages\n");
		printf("                (0 ~ ...)\n\n");
		printf("    [type]    - If press \'T\', it will transmit 'type' CAN-TYPE packages\n");
		printf("                0 for Standard Data Frame\n");
		printf("                1 for Extended Data Frame\n");
		printf("                2 for Stadnard Remote Frame\n");
		printf("                3 for Extended Remote Frame\n\n");
		printf("    [datalen] - If press \'T\', it will transmit 'datalen' number data in\n");
		printf("                one package\n");
		printf("                (0 ~ 8)\n\n");
		return 0;
	}
	packlen = atoi(argv[1]);
	type    = atoi(argv[2]);
	datalen = atoi(argv[3]);
	
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("socket");
        return 1;
    }
    
    // enable non block operation
    flag = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, flag | O_NONBLOCK);
    
    so_rcvbuf = 0;
    socklen = sizeof(so_rcvbuf);
    nbytes = getsockopt(s, SOL_SOCKET, SO_RCVBUF, &so_rcvbuf, &socklen);
    printf("nbytes:%i errno:%i so_rcvbuf:%i \n", nbytes, errno, so_rcvbuf);

    so_rcvbuf = 1024 * 1000;
    nbytes = setsockopt(s, SOL_SOCKET, SO_RCVBUF, &so_rcvbuf, sizeof(so_rcvbuf));
    printf("nbytes:%i errno:%i so_rcvbuf:%i \n", nbytes, errno, so_rcvbuf);

    nbytes = getsockopt(s, SOL_SOCKET, SO_RCVBUF, &so_rcvbuf, &socklen);
    printf("nbytes:%i errno:%i so_rcvbuf:%i \n", nbytes, errno, so_rcvbuf);
    
    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr);
    ifindex = ifr.ifr_ifindex;

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(s);
        return 1;
    }
	
	printf("CAN bus init ok!!\n");
	
	while (1) {
		if (kbhit()) {
			c = getch();
			
			if (c == 27) break;
			if (c == 't' || c == 'T') {
                frame.can_dlc = datalen;
				
				for (i = 0; i < packlen; i++) {
                    frame.can_id  = i + 1;
                    if (type == 1) {
                        frame.can_id |= CAN_EFF_FLAG;
                    } else if (type == 2) {
                        frame.can_id |= CAN_RTR_FLAG;
                    } else if (type == 3) {
                        frame.can_id |= (CAN_EFF_FLAG | CAN_RTR_FLAG);
                    }
					for (j = 0; j < datalen; j++)
						frame.data[j] = i*16 + j + 1;
					nbytes = write(s, &frame, sizeof(struct can_frame));
                    if (nbytes <= 0) {
                        printf(" [XMIT]: Write packet fail.\n");
                    }
				}
			}
		}
		
		while ((nbytes = read(s, &frame, sizeof(struct can_frame))) > 0) {
			printf("*[RCVD]: ");
			if (nbytes >= sizeof(struct can_frame)) {
                if ((frame.can_id & CAN_EFF_FLAG) && (frame.can_id & CAN_RTR_FLAG)) {
                    printf("3:");
                } else if (!(frame.can_id & CAN_EFF_FLAG) && (frame.can_id & CAN_RTR_FLAG)) {
                    printf("2:");
                } else if ((frame.can_id & CAN_EFF_FLAG) && !(frame.can_id & CAN_RTR_FLAG)) {
                    printf("1:");
                } else {
                    printf("0:");
                }
				printf("%08lx:%d -> ", frame.can_id & ~(CAN_EFF_FLAG | CAN_RTR_FLAG), frame.can_dlc);
				for (i = 0; i < frame.can_dlc; i++)
					printf("%02x ", frame.data[i]);
            } else {
				printf("Read fail.");
            }
			printf("\n");
		}
	}

	close(s);
	
	return 0;
}
