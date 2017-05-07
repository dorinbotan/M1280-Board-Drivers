#include <avr/io.h>
#include "ringbuffer.h"

#ifndef USART_H
#define USART_H

typedef struct {
  ring_buffer_size_t (*usart_recv_queue_size)(void);
  ring_buffer_size_t (*usart_recv_dequeue)(char *data);
  ring_buffer_size_t (*usart_recv_peek)(char *data, ring_buffer_size_t index);
  void (*usart_send)(char data);
  void (*usart_send_arr)(const char *data, ring_buffer_size_t size);
} usart_desc_t;

#endif /* USART_H */
