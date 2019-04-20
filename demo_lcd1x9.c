#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <string.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include "lcd1x9.h"

int setup(int bus) {
  // int fd = wiringPiI2CSetup(LCD1x9_SLAVE_ADDR);
  int fd;
  switch (bus) {
    case 0: fd = open("/dev/i2c-0", O_RDWR); break;
    case 1: fd = open("/dev/i2c-1", O_RDWR); break;
    case 2: fd = open("/dev/i2c-2", O_RDWR); break;
    default:
      fprintf(stderr, "supported bus 0,1,2\n");
      exit(-1);
  }
  if (0 > fd) {
    fprintf(stderr, "ERROR: Unable to access module: %s\n", strerror (errno));
    exit(-1);
  }
  // if (0 > begin(fd)) {
	if (ioctl(fd, I2C_SLAVE, LCD1x9_SLAVE_ADDR) < 0) {  //kwc-method
    fprintf(stderr, "ERROR: module not found\n");
    exit(-1);
  }

	LCD1x9_Initialize(fd);  //kwc-method

  delay(100);  // Wait 100ms  //fixme

	return fd;
}

int loop(int fd, char* argv[]) {
	
	printf("%s\n", argv[2]);
	// LCD1x9_Write(fd, "987654321");
	LCD1x9_Write(fd, argv[2]);
  delay(100);  // Wait 100ms

	return -1;  // done with loop
	// return 0;  // loop again
}

int main(int argc, char* argv[]) {
  char *p;

	if (argc != 3) {
		printf("Usage: ./demo_lcd1x9 bus yourShortMessage  # bus=/dev/i2c-? , max 9 chars\n");
    printf(" e.g.: ./demo_lcd1x9 0 123456789  # 0 refers to /dev/i2c-0 \n");
    printf(" e.g.: ./demo_lcd1x9 2 987654321  # 2 refers to /dev/i2c-2 \n");
		return 0;
	}

  errno = 0;
  long conv = strtol(argv[1], &p, 10);
  if (errno != 0 || *p != '\0' || conv > INT_MAX) {
    printf("supported bus 0,1,2\n");
    return 0;
  }

	int fd = setup((int)conv);
	while ( !loop(fd, argv) );
  return 0;
}
