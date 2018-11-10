/*
 *	Circular buffer for UART receive and transmit operations.
 *  
 *  The ringbuffer can be used outside of its intended context by creating a 
 *  new ringbuffer_t variable. The buffer size for all buffers is fixed (see
 *  RING_BUFFER_SIZE).
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

 
 #ifndef RINGBUFFER_H_
 #define RINGBUFFER_H_

 #include <avr/io.h>
 
 #define RINGBUFFER_SIZE	64
 #define RINGBUFFER_SIZE_MSK	(RINGBUFFER_SIZE - 1)

/*
 *	RingBuffer structure
 *
 *	Every instance of this structure is equivalent to a new ringbuffer
 */
struct ringbuffer_t
{
	uint8_t head;
	uint8_t tail;
	uint8_t overflow;
	uint8_t buffer[RINGBUFFER_SIZE];
};

/*
 *	Initializes a ringbuffer
 *	After initialization the ringbuffer is empty
 */
void rb_init(volatile struct ringbuffer_t *rb);

/*
 *	Checks if the ringbuffer is full
 *	Returns: 1, if the buffer is full. 0 otherwise.
 */
uint8_t rb_isFull(volatile struct ringbuffer_t *rb);

/*
 *	Checks if the ringbuffer is empty
 *	Returns: 1, if the buffer is full. 0 otherwise.
 */
uint8_t rb_isEmpty(volatile struct ringbuffer_t *rb);

/*
 *	Writes data to the ringbuffer
 *	NOTE: If the ringbuffer is full, data will be dropped. 
 *
 *	data: The data to be written.
 */
void rb_write(volatile struct ringbuffer_t *rb, uint8_t data);

/*
 *	Reads the next data from the ringbuffer
 *	Returns: 0, if the buffer is empty. Next data otherwise.
 */
uint8_t rb_read(volatile struct ringbuffer_t *rb);
 
 #endif