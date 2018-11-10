/*
 * ringbuffer.c1
 *
 * Created: 28.10.2018 11:52:00
 *  Author: Tim
 */ 
 
 #ifndef RINGBUFFER_H_
 #define RINGBUFFER_H_

 #include <avr/io.h>
 
 #define RINGBUFFER_SIZE	64
 #define RINGBUFFER_SIZE_MSK	(RINGBUFFER_SIZE - 1)

 struct ringbuffer_t
 {
	 uint8_t head;
	 uint8_t tail;
	 uint8_t overflow;
	 uint8_t buffer[RINGBUFFER_SIZE];
 };

void rb_init(volatile struct ringbuffer_t *rb);
uint8_t rb_isFull(volatile struct ringbuffer_t *rb);
uint8_t rb_isEmpty(volatile struct ringbuffer_t *rb);
void rb_write(volatile struct ringbuffer_t *rb, uint8_t data);
uint8_t rb_read(volatile struct ringbuffer_t *rb);
 
 #endif