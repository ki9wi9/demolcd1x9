#ifndef _LCD1x9_DRIVER_H
#define _LCD1x9_DRIVER_H

#include <stdint.h>

typedef uint8_t BYTE;
typedef uint16_t WORD;

#define LCD1x9_SLAVE_ADDR        0x38

/******************************************************************************
* Description: LCD1x9_Initialize(..) - initializes pins and registers of the LCD1x9
*				Also lights up all segments
* Input: 	none
* Output: 	none
* Return:	0 if sucessfully initialized, -1 if error occured 
*******************************************************************************/
// char LCD1x9_Initialize(void);  //fixme
char LCD1x9_Initialize(int fd);  //fixme

/******************************************************************************
* Description: LCD1x9_enableSegment(..) - enables a segment in the display buffer
*		Note: Does not actually light up the segment, have to call the 'LCD1x9_Update(..)'
* Input: 	comIndex - backplate index
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
void LCD1x9_enableSegment(BYTE comIndex, BYTE bitIndex);

/******************************************************************************
* Description: LCD1x9_disableSegment(..) - disables a segment in the display buffer
*		Note: Does not actually lights off the segment, have to call the 'LCD1x9_Update(..)'
* Input: 	comIndex - backplate index
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
void LCD1x9_disableSegment(BYTE comIndex, BYTE bitIndex);

/******************************************************************************
* Description: LCD1x9_Update(..) - disables a segment in the display buffer
*		Note: Does not actually lights off the segment, have to call the 'LCD1x9_Update(..)'
* Input: 	comIndex - backplate index
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
// void LCD1x9_Update(void);
void LCD1x9_Update(int fd);  //fixme

/******************************************************************************
* Description: LCD1x9_Write(..) - writes a string to the display
* Input: 	string - the string to write, no more than 9 characters
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
void LCD1x9_Write(int fd, char *string);  //fixme
// void LCD1x9_WriteROM(ROM char* string); // same thing but with ROM string
void LCD1x9_WriteROM(char* string); // same thing but with ROM string  //fixme

#endif // _LCD1x9_DRIVER_H
