/*old
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */
/*
config-pin p9_17 spi_cs
config-pin p9_18 spi
config-pin p9_21 spi
config-pin p9_22 spi_sclk
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#define PORT 8081
#define DATA_INTERVAL 250000 // Interval in nano


#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static const char *device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 100000;
static uint16_t delay;


static uint16_t transfer(int fd, int bitmap)
{
	int ret;
	uint8_t map=0x00;

	switch (bitmap) {
		case 0:
			map = 0x18;
			break;
		case 1:
			map = 0x19;
			break;
		case 2:
			map = 0x1A;
			break;
		case 3:
			map = 0x1B;
			break;
		case 4:
			map = 0x1C;
			break;
		case 5:
			map = 0x1D;
			break;
		case 6:
			map = 0x1E;
			break;
		case 7:
			map = 0x1F;
			break;
		default:
			map = 0x0C;
			break;
		}

	uint8_t tx[] = {
			map, 0x00, 0x00
	};
	uint8_t rx[ARRAY_SIZE(tx)] = {0, };
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = ARRAY_SIZE(tx),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
	uint16_t value16 = 0;
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");


	for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
		if (!(ret % 6))
			puts("");
		//printf("raw data:%.2X \n", rx[ret]);
      
	if(ret==1){
	
	       uint8_t value8 = rx[ret]; // Example 8-bit value
               value16 = (value8 << 8)|rx[ret+1]; // Shift 8-bit value into 16-bit variable
		value16=value16 << 2;
		value16=value16 >> 6;
		float fvalue16=(value16 * 3.433) / 1024; 
	printf("for loop state:%.2X \n", ret);
	printf("8-bit value: 0x%.2X\n", value8);
	printf("16-bit value: %f\n", fvalue16); 
                        puts("");
	}
               
				
        }
        puts("");


return value16;

 }

 
static void print_usage(const char *prog)
{
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev1.1)\n"
	     "  -s --speed    max speed (Hz)\n"
	     "  -d --delay    delay (usec)\n"
	     "  -b --bpw      bits per word \n"
	     "  -l --loop     loopback\n"
	     "  -H --cpha     clock phase\n"
	     "  -O --cpol     clock polarity\n"
	     "  -L --lsb      least significant bit first\n"
	     "  -C --cs-high  chip select active high\n"
	     "  -3 --3wire    SI/SO signals shared\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "speed",   1, 0, 's' },
			{ "delay",   1, 0, 'd' },
			{ "bpw",     1, 0, 'b' },
			{ "loop",    0, 0, 'l' },
			{ "cpha",    0, 0, 'H' },
			{ "cpol",    0, 0, 'O' },
			{ "lsb",     0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire",   0, 0, '3' },
			{ "no-cs",   0, 0, 'N' },
			{ "ready",   0, 0, 'R' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 's':
			speed = atoi(optarg);
			break;
		case 'd':
			delay = atoi(optarg);
			break;
		case 'b':
			bits = atoi(optarg);
			break;
		case 'l':
			mode |= SPI_LOOP;
			break;
		case 'H':
			mode |= SPI_CPHA;
			break;
		case 'O':
			mode |= SPI_CPOL;
			break;
		case 'L':
			mode |= SPI_LSB_FIRST;
			break;
		case 'C':
			mode |= SPI_CS_HIGH;
			break;
		case '3':
			mode |= SPI_3WIRE;
			break;
		case 'N':
			mode |= SPI_NO_CS;
			break;
		case 'R':
			mode |= SPI_READY;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}

int main(int argc, char *argv[])
{
system("config-pin p9_17 spi_cs");
system("config-pin p9_18 spi");
system("config-pin p9_21 spi");
system("config-pin p9_22 spi_sclk");
//new code
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 1) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    // Accept a new connection
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addr_len);
    if (clientfd == -1) {
        perror("Accepting connection failed");
        exit(EXIT_FAILURE);
    }
//new code
  	

	int ret = 0;
	int fd;

	parse_opts(argc, argv);

while(1){


   for(int arr = 0; arr < 8; arr++){
	   fd = open(device, O_RDWR);
	   	if (fd < 0)
	   		pabort("can't open device");

	   	/*
	   	 * spi mode
	   	 */
	   	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	   	if (ret == -1)
	   		pabort("can't set spi mode");

	   	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	   	if (ret == -1)
	   		pabort("can't get spi mode");

	   	/*
	   	 * bits per word
	   	 */
	   	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	   	if (ret == -1)
	   		pabort("can't set bits per word");

	   	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	   	if (ret == -1)
	   		pabort("can't get bits per word");

	   	/*
	   	 * max speed hz
	   	 */
	   	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	   	if (ret == -1)
	   		pabort("can't set max speed hz");

	   	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	   	if (ret == -1)
	   		pabort("can't get max speed hz");

	   	//printf("spi mode: %d\n", mode);
	   	//printf("bits per word: %d\n", bits);
	   	//printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	       //new code

	   	    // Generate some 12-bit data
	   	    uint16_t data = transfer(fd, arr) & 0x0FFF; // Keep only the lower 12 bits
	   	    // Extract the 0-8 number value (message number)
	   	    uint16_t messageNumber = arr & 0x0F; // Keep only the lower 4 bits
	   	    // Shift the message number to the 16th-13th bit position
	   	    messageNumber <<= 13;
	   	    // Set the 16th-13th bits of the data with the message number
	   	    data |= messageNumber;
	   	    // Send the modified data

	   	    // Wait for the specified interval before sending the next value



        // Generate some 16-bit data

        //uint16_t data = transfer(fd, arr);//(uint16_t)rand();
        // Send the data
        //if(arr==0)
			//data=0xFFFF;

        send(clientfd, &data, sizeof(data), 0);
        printf("Data sent to Python program.\n");
        printf("8-bit value: 0x%.4X\n", data);
        // Wait for the specified interval
        printf("spi mode: %d\n", arr);
        close(fd);
        usleep(DATA_INTERVAL);

	



	}
}
  
	return ret;
}
