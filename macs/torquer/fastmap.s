
 /*
  * fastmap.s
  *
  *	usage: uint8_t fastmap(uint8_t original)
  * maps [0-200] -> [0 255]
  * results will be rounded to zero decimal places
  *
  *	registers used: r0, r1, r18, r24, r25
  * cycles: 13 + ret = 17
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