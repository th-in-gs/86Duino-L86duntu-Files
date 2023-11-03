#include <stdlib.h>
#include <stdio.h>
//#include <conio.h>
//#include <process.h>
#include <string.h>
#include <unistd.h>
#include "io.h"
#include "com.h"

#define CROSSBAR_REG_NUM 44
#define SB_CROSSBASE  (0x64)
#define CROSSBARBASE  (0x0A00)

COMPort *handle;

bool com_init(int port, unsigned long baud, unsigned char config) 
{
	if (io_Init() == false) 
	{
		printf("ERROR: IO init fail.\n");
		return false;
	}

	// UART Clock Selection.
	// When South Bridge Function 0 C0h[31] (SBCLK)=0,
	// When South Bridge Function 0 C0h[31] (SBCLK)=1,
	sb_Write(0xc0, sb_Read(0xc0) & 0x7fffffffL | ((unsigned long)1L << 31));
	io_Close();

	if ((handle = com_Init(port)) == NULL) 
	{
		printf("COM init fail!!\n");
		return false;
	}

	switch (baud) 
	{
		case 6000000L: baud = COM_UARTBAUD_6000000BPS; break;
		case 3000000L: baud = COM_UARTBAUD_3000000BPS; break;
		case 2000000L: baud = COM_UARTBAUD_2000000BPS; break;
		case 1500000L: baud = COM_UARTBAUD_1500000BPS; break;
		case 1000000L: baud = COM_UARTBAUD_1000000BPS; break;
		case 750000L:  baud = COM_UARTBAUD_750000BPS;  break;
		case 500000L:  baud = COM_UARTBAUD_500000BPS;  break;
		case 461538L:  baud = COM_UARTBAUD_461538BPS;  break;
		case 333333L:  baud = COM_UARTBAUD_333333BPS;  break;
		case 300000L:  baud = COM_UARTBAUD_300000BPS;  break;
		case 250000L:  baud = COM_UARTBAUD_250000BPS;  break;
		case 200000L:  baud = COM_UARTBAUD_200000BPS;  break;
		case 150000L:  baud = COM_UARTBAUD_150000BPS;  break;
		case 125000L:  baud = COM_UARTBAUD_125000BPS;  break;
		case 115200L:  baud = COM_UARTBAUD_115200BPS;  break;
		case 57600L:   baud = COM_UARTBAUD_57600BPS;   break;
		case 38400L:   baud = COM_UARTBAUD_38400BPS;   break;
		case 28800L:   baud = COM_UARTBAUD_28800BPS;   break;
		case 19200L:   baud = COM_UARTBAUD_19200BPS;   break;
		case 14400L:   baud = COM_UARTBAUD_14400BPS;   break;
		case 9600L:    baud = COM_UARTBAUD_9600BPS;    break;
		case 4800L:    baud = COM_UARTBAUD_4800BPS;    break;
		case 2400L:    baud = COM_UARTBAUD_2400BPS;    break;
		case 1200L:    baud = COM_UARTBAUD_1200BPS;    break;
		case 600L:     baud = COM_UARTBAUD_600BPS;     break;
		case 300L:     baud = COM_UARTBAUD_300BPS;     break;
		case 50L:      baud = COM_UARTBAUD_50BPS;      break;
		default:       baud = COM_UARTBAUD_9600BPS;    break;
	}

	com_SetBPS(handle, baud);
	com_SetFormat(handle, config);  

	return true;
}

int main(int argc, char* argv[])
{
	int i;
	int crossbarBase, gpioBase;
	int port;
	char port_name[8];
	unsigned long baud;

	if(argc != 3)
	{
		printf("Parameters input error!\n");
		printf("Example: use_com 1 115200\n");
		return EXIT_FAILURE;
	}

	switch(argv[1][0])
	{
		case '1': 
			port = COM1; 
			strcpy(port_name, "COM1");
			break;

		case '2': 
			port = COM2; 
			strcpy(port_name, "COM2");
			break;

		case '3': 
			port = COM3; 
			strcpy(port_name, "COM3");
			break;

		case '4': 
			port = COM4; 
			strcpy(port_name, "COM4");
			break;

		default :
			printf("Assign comport error!\n");
			break;
	}

	baud = atol(argv[2]);


	if(io_Init() == false)
		return false;

	//set corssbar Base Address
	crossbarBase = sb_Read16(SB_CROSSBASE) & 0xfffe;
	if(crossbarBase == 0 || crossbarBase == 0xfffe)
		sb_Write16(SB_CROSSBASE, CROSSBARBASE | 0x01);

	if(port == COM1)
		io_outpw(CROSSBARBASE + 0x9a, 0x0808);
	else if(port == COM2)
		io_outpw(CROSSBARBASE + 0x9e, 0x0808);
	else if(port == COM3)
		io_outpw(CROSSBARBASE + 0x9c, 0x0808);

	io_Close();

	if(com_init(port, baud, BYTESIZE8|NOPARITY|STOPBIT1) == false)
	{
		printf("Com port init fail\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

