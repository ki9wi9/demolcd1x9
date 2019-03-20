#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>
#include <unistd.h>

//kwc
#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include "lcd1x9.h"
#include "lcd1x9_mapping.h"

/* DECLARE EXTERNAL VARIABLES HERE */

/* DEFINE LOCAL MACROS HERE */

/* DEFINE LOCAL VARIABLES HERE */
static BYTE lcdBitmap[20]; // 40segments * 4 = 160px, 160 / 8 = 20bytes

// int begin(int fd)
// {
// 	LCD1x9_Initialize(fd);
//   // Wait at least 4.5ms
//   delay(5);
//
//   return 0;
// }

uint8_t m_idx_i2cBuf = 0;  //kwc-method
uint8_t m_i2cBuf[32];  //kwc-method

char local_I2C_WriteByte(int fd, unsigned char ucData)
{
	m_i2cBuf[m_idx_i2cBuf++] = ucData;  //stores it for later writing
  return 0;
}

void i2cWriteBlock(int fd) {

	// printf("m_idx_i2cBuf = %d\n", m_idx_i2cBuf);
	// printf("[0x%02X 0x%02X 0x%02X 0x%02X 0x%02X .. 0x%02X 0x%02X]\n",
	// 	m_i2cBuf[0], m_i2cBuf[1], m_i2cBuf[2], m_i2cBuf[3], m_i2cBuf[4], m_i2cBuf[m_idx_i2cBuf-2], m_i2cBuf[m_idx_i2cBuf-1]);

  if (write(fd, &m_i2cBuf, m_idx_i2cBuf) < 0) {
    printf("Error writing block: %s\n", strerror(errno));
  }
	m_idx_i2cBuf = 0;  //reset index
  return;
}

/******************************************************************************
* Description: LCD1x9_Initialize(..) - initializes pins and registers of the LCD1x9
*				Also lights up all segments
* Input: 	none
* Output: 	none
* Return:	0 if sucessfully initialized, -1 if error occured 
*******************************************************************************/
char LCD1x9_Initialize(int fd)
{
	char result = 0, i;
	
	// the idea is to toggle pins as inputs/outputs to emulate I2C open drain mode of operation!	
	// LCD1x9_SDA_TRIS = 1;
	// LCD1x9_SCL_TRIS = 1;
	//
	// LCD1x9_SDA_LAT = 0;
	// LCD1x9_SCL_LAT = 0;
	
	// local_I2C_Delay();
	// if( !(LCD1x9_SCL_PORT && LCD1x9_SDA_PORT) )
	// 	return -1;

	// local_I2C_Start();
	// result |= local_I2C_WriteByte(fd, LCD1x9_SLAVE_ADDR | 0x00);  //kwc-method IMPT!
	result |= local_I2C_WriteByte(fd, 0b11001000); // mode register
	result |= local_I2C_WriteByte(fd, 0b11110000); // blink register
	result |= local_I2C_WriteByte(fd, 0b11100000); // device select register
	result |= local_I2C_WriteByte(fd, 0b00000000); // pointer register
	
	// light up all the segments, initialize the local display buffer as well
	for(i = 0; i < 20; i++) {
		result |= local_I2C_WriteByte(fd, 0xFF);
		lcdBitmap[i] = 0xFF;
	}	
	
	// local_I2C_Stop();
	i2cWriteBlock(fd);  //kwc-method
	
	return (result ? -1 : 0);
}

/******************************************************************************
* Description: LCD1x9_enableSegment(..) - enables a segment in the display buffer
*		Note: Does not actually light up the segment, have to call the 'LCD1x9_Update(..)'
* Input: 	comIndex - backplate index
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
void LCD1x9_enableSegment(BYTE comIndex, BYTE bitIndex)
{
	if(bitIndex >= 40)
		return;
		
	comIndex &= 0x3;
	
	if(bitIndex & 0x1)
		comIndex |= 0x4;
		
	bitIndex >>= 1;
	
	lcdBitmap[bitIndex] |= 0x80 >> comIndex;
}

/******************************************************************************
* Description: LCD1x9_disableSegment(..) - disables a segment in the display buffer
*		Note: Does not actually lights off the segment, have to call the 'LCD1x9_Update(..)'
* Input: 	comIndex - backplate index
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
void LCD1x9_disableSegment(BYTE comIndex, BYTE bitIndex)
{
	if(bitIndex >= 40)
		return;
		
	comIndex &= 0x3;
	
	if(bitIndex & 0x1)
		comIndex |= 0x4;
		
	bitIndex >>= 1;
	
	lcdBitmap[bitIndex] &= ~(0x80 >> comIndex);
}

/******************************************************************************
* Description: LCD1x9_Update(..) - disables a segment in the display buffer
*		Note: Does not actually lights off the segment, have to call the 'LCD1x9_Update(..)'
* Input: 	comIndex - backplate index
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
void LCD1x9_Update(int fd)
{
	BYTE i;
	
	// local_I2C_Start();
	// local_I2C_WriteByte(fd, LCD1x9_SLAVE_ADDR | 0x00);  //kwc-method IMPT!
	local_I2C_WriteByte(fd, 0b11100000); // device select register
	local_I2C_WriteByte(fd, 0b00000000); // pointer register
	
	// send the local buffer to the device
	for(i = 0; i < 20; i++) {
		local_I2C_WriteByte(fd, lcdBitmap[i]);
	}
	// local_I2C_Stop();
	i2cWriteBlock(fd);  //kwc-method
}

/******************************************************************************
* Description: LCD1x9_Write(..) - writes a string to the display
* Input: 	string - the string to write, no more than 9 characters
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
void LCD1x9_Write(int fd, char *string)
{
	BYTE data, length, index, i;
	WORD bitfield;
	BYTE com, bit;
	
	length = strlen(string);
	if(length > 9) {
    printf("Error: no more than 9 characters.\n");  //kwc-method
		return;
	}
	
	index  = 0;
	/* fill out all characters on display */
	for (index = 0; index < 9; index++) {
		if (index < length) {
			data = (BYTE)string[index];
		} else {
			data = 0x20; // fill with spaces if string is shorter than display
		}

		data -= 0x20;
		bitfield = LCDAlphabet[data];
	
		for (i = 0; i < 16; i++) {
			bit = LCD1x9.Text[index].bit[i];
			com = LCD1x9.Text[index].com[i];
		
			if (bitfield & ((WORD)1 << i)) {
				LCD1x9_enableSegment(com, bit);
			} else {
				LCD1x9_disableSegment(com, bit);
			}
		}
	}

	LCD1x9_Update(fd);
}
