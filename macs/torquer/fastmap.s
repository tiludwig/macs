/*
 *	Fastmap utility code for command execution
 *
 *  Maps a value in the range of [0, 200] to a value in the range of [0, 255].
 *  Usually mapping one range to another requires the use of division and/or use
 *	of the modulo operator. Both are expensive operations on devices without 
 *	hardware division support. 
 *  The mapping function is
 *		out = in * 255 / 200
 *  This function can be simplified, by approximating the (255 / 200) scaling factor
 *  by a number which satisfys 
 *		x / (2^n) ~= 255 / 200
 *	With x = 5 and n = 2, the mapping is accurate at lower numbers, but gets 
 *	proportionaly worse at higher numbers. A correction factor (7 / 256) is multiplied
 *	to the input value and added to the mapping result. 
 *	NOTE: The result will be round to zero decimal places.
 *
 *	Function prototype: uint8_t fastmap(uint8_t input)
 *  Registers used:		r0, r1, r18, r24, r25
 *  Cycles:				13 + ret
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
 /*
  * fastmap.s
  *
  *	usage: uint8_t fastmap(uint8_t original)
  * maps [0-200] -> [0 255]
  * results will be rounded to zero decimal places
  *

  *
  * approximates original * 255 / 200 by original * 5/4 + original * 7/256
  * to round the result corrective factor has to be adjusted to *14/256 >> 1
  * the asr (>> 1) is necessary to obtain the carry flag for the following add with carry
  */

 .global fastmap

 fastmap:
	; save input in other register for later use
	mov r25, r24
	; load multiplier and multiply input
	ldi r18, 5
	mul r25, r18
	; result is in r0:r1 (low:high)
	; divide by 4
	asr r1
	ror r0
	asr r1
	ror r0
	; save lower byte (after division by 4 result is guaranted to be < 255)
	mov r24, r0
	; now add correction factor
	; load multiplier and multiply to get corrective factor
	ldi r18, 14
	mul r25, r18
	asr r1
	; divide by 256 (discard lower byte)
	; add correction (with carry to round)
	adc r24, r1
	; r1 = 0 for gcc
	clr r1
	ret