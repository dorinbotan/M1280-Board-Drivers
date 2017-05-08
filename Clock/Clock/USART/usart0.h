#include "usart.h"
#include "ringbuffer.h"

#ifndef USART0_H
#define USART0_H

void usart0_init(void);

ring_buffer_size_t usart0_recv_queue_size(void);
ring_buffer_size_t usart0_recv_dequeue(char *data);
ring_buffer_size_t usart0_recv_peek(char *data, ring_buffer_size_t index);
void usart0_send(char data);
void usart0_send_arr(const char *data, ring_buffer_size_t size);
usart_desc_t *get_usart0_descriptor(void);

#endif /* USART0_H */
