/*
 *	Fastmap utility code for command execution
 *
 *  Maps a value in the range of [0, 200] to a value in the range of [145, 255].
 *  Usually mapping one range to another requires the use of division and/or use
 *	of the modulo operator. Both are expensive operations on devices without 
 *	hardware division support. 
 *  The mapping function is
 *		out = in * (255-145) / 200 + 145
 *  This function can be simplified, by approximating the ( (255 - 145) / 200) scaling factor
 *  by a number which satisfys 
 *		x / (2^n) ~= (255-145) / 200
 *	With x = 141 and n = 8, the maximum error introduced is 0.16.
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
 .global fastmap

 // prototype uint8_t fastmap(uint8_t)
 // avr-gcc will provide the input argument in r25:r24. (r25 = 0)
 fastmap:
	// multiply by 141
	ldi r18, 141
	mul r24, r18
	// result is in r1:r0 (high:low)
	// divide by 256 (discard lower byte) -> no instruction needed
	// add 145
	ldi r18, 145
	add r1, r0
	// output is expected in r24
	mov r24, r1
	// clear r1
	clr r1
	ret