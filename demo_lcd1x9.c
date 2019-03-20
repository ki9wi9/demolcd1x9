#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <string.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include "lcd1x9.h"

int setup() {
  // int fd = wiringPiI2CSetup(LCD1x9_SLAVE_ADDR);
  int fd = open("/dev/i2c-1", O_RDWR);  //kwc-method
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
	
	printf("%s\n", argv[1]);
	// LCD1x9_Write(fd, "987654321");
	LCD1x9_Write(fd, argv[1]);
  delay(100);  // Wait 100ms

	return -1;  // done with loop
	// return 0;  // loop again
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Usage: ./demo_lcd1x9 yourShortMessage  #limited to 9 characters\n");
		return 0;
	}
	int fd = setup();
	while ( !loop(fd, argv) );
  return 0;
}
