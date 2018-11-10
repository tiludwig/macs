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

#include "ringbuffer.h"

/*
 *	Initializes a ringbuffer
 *	After initialization the ringbuffer is empty
 */
inline void rb_init(volatile struct ringbuffer_t *rb)
{
	rb->head = 0;
	rb->tail = 0;
	rb->overflow = 0;
}

/*
 *	Checks if the ringbuffer is full
 *	Returns: 1, if the buffer is full. 0 otherwise.
 */
inline uint8_t rb_isFull(volatile struct ringbuffer_t *rb)
{
	return ((rb->tail - rb->head) == RINGBUFFER_SIZE) ? 1 : 0;
}

/*
 *	Checks if the ringbuffer is empty
 *	Returns: 1, if the buffer is full. 0 otherwise.
 */
inline uint8_t rb_isEmpty(volatile struct ringbuffer_t *rb)
{
	return ((rb->tail - rb->head) == 0) ? 1 : 0;
}


/*
 *	Writes data to the ringbuffer
 *	NOTE: If the ringbuffer is full, data will not be written. 
 *
 *	data: The data to be written.
 */
inline void rb_write(volatile struct ringbuffer_t *rb, uint8_t data) {
	/* check if full */
	if(rb_isFull(rb) == 1) {
		rb->overflow = 1;
		return;
	}
	/* resolve index */
	uint8_t tail = rb->tail++;
	uint8_t insert_index = tail & RINGBUFFER_SIZE_MSK;
	
	/* insert data */
	rb->buffer[insert_index] = data;
}


/*
 *	Reads the next data from the ringbuffer
 *	Returns: 0, if the buffer is empty. Next data otherwise.
 */
inline uint8_t rb_read(volatile struct ringbuffer_t *rb)
{
	/* check if empty */
	if(rb_isEmpty(rb)) {
		return 0;
	} 
	/* resolve index */
	uint8_t head = rb->head++;
	uint8_t read_index = head & RINGBUFFER_SIZE_MSK;
		
	/* read data */
	return rb->buffer[read_index];
}