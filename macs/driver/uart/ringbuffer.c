/*
 * ringbuffer.h
 *
 * Created: 23.10.2018 15:50:44
 *  Author: Tim
 */ 

#include "ringbuffer.h"


/*
 *	Initializes the ring buffer
 */
inline void rb_init(volatile struct ringbuffer_t *rb)
{
	rb->head = 0;
	rb->tail = 0;
	rb->overflow = 0;
}

/*
 *	Checks if the ringbuffer is full
 */
inline uint8_t rb_isFull(volatile struct ringbuffer_t *rb)
{
	return ((rb->tail - rb->head) == RINGBUFFER_SIZE) ? 1 : 0;
}

/*
 *	Checks if the ringbuffer is empty
 */	
inline uint8_t rb_isEmpty(volatile struct ringbuffer_t *rb)
{
	return ((rb->tail - rb->head) == 0) ? 1 : 0;
}


/*
 *	Writes a byte into the ringbuffer. If the ringbuffer is full the data is not written.
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
 *	Gets the next byte from the ringbuffer
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