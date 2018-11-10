/*
 *	NXP MAG3110 register defines
 *
 *	This file contains the register definitions for the NXP MAG3110
 *
 * Copyright 2018 Tim Ludwig
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef MAG3110_DEFINES_H_
#define MAG3110_DEFINES_H_


#define MAG31110_ADDRESS		0x0E
#define MAG3110_READ_ADDRESS	((MAG3110_WRITE_ADDRESS << 1) | 0x0)
#define MAG3110_WRITE_ADDRESS	((MAG3110_WRITE_ADDRESS << 1) | 0x1)

#define DR_STATUS				0x00
#define OUT_X_MSB				0x01
#define OUT_X_LSB				0x02
#define OUT_Y_MSB				0x03
#define OUT_Y_LSB				0x04
#define OUT_Z_MSB				0x05
#define OUT_Z_LSB				0x06
#define WHO_AM_I				0x07
#define SYSMOD					0x08
#define OFF_X_MSB				0x09
#define OFF_X_LSB				0x0A
#define OFF_Y_MSB				0x0B
#define OFF_Y_LSB				0x0C
#define OFF_Z_MSB				0x0D
#define OFF_Z_LSB				0x0E
#define DIE_TEMP				0x0F
#define CTRL_REG1				0x10
#define CTRL_REG2				0x11

#define MAG3110_DEVICE_ID		0xC4

#define STATUS_ZYXOW			7
#define STATUS_ZOW				6
#define STATUS_YOW				5
#define STATUS_XOW				4
#define STATUS_ZYXDR			3
#define STATUS_ZDR				2
#define STATUS_YDR				1
#define STATUS_XDR				0

#define SYSMOD_SYSMOD1			1
#define SYSMOD_SYSMOD0			0

#define CTRL1_DR2				7
#define CTRL1_DR1				6
#define CTRL1_DR0				5
#define CTRL1_OS1				4
#define CTRL1_OS0				3
#define CTRL1_FR				2
#define CTRL1_TM				1
#define CTRL1_AC				0

#define CTRL2_AUTO_MRST_EN		7
#define CTRL2_RAW				5
#define CTRL2_MAG_RST			4

#endif /* MAG3110_DEFINES_H_ */