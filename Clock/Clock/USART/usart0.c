#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include "usart0.h"

ring_buffer_t usart0_recv_ring_buf;
ring_buffer_t usart0_send_ring_buf;

void usart0_init(void) {
  cli();

  ring_buffer_init(&usart0_recv_ring_buf);
  ring_buffer_init(&usart0_send_ring_buf);
  
  UCSR0A = 0;
  // Enable USART0 TX and RX
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  // Async USART, 8bit, no parity and 1 stop bit
  UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
  // 9600 Baud Rate at 7.37 MHz
  UBRR0L = 47;
  UBRR0H = 0;

  sei();
}

ring_buffer_size_t usart0_recv_queue_size(void) {
  ring_buffer_size_t result;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    result = ring_buffer_num_items(&usart0_recv_ring_buf);
  }
  return result;
}

ring_buffer_size_t usart0_recv_dequeue(char *data) {
  ring_buffer_size_t result;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    result = ring_buffer_dequeue(&usart0_recv_ring_buf, data);
  }
  return result;
}

ring_buffer_size_t usart0_recv_peek(char *data, ring_buffer_size_t index) {
  ring_buffer_size_t result;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    result = ring_buffer_peek(&usart0_recv_ring_buf, data, index);
  }
  return result;
}

void usart0_send(char data) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    ring_buffer_queue(&usart0_send_ring_buf, data);
    UCSR0B |= (1 << UDRIE0);
  }
}

void usart0_send_arr(const char *data, ring_buffer_size_t size) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    ring_buffer_queue_arr(&usart0_send_ring_buf, data, size);
    UCSR0B |= (1 << UDRIE0);
  }
}

usart_desc_t *get_usart0_descriptor(void) {
  static usart_desc_t descriptor =
    {
      .usart_recv_queue_size = usart0_recv_queue_size,
      .usart_recv_dequeue = usart0_recv_dequeue,
      .usart_recv_peek = usart0_recv_peek,
      .usart_send = usart0_send,
      .usart_send_arr = usart0_send_arr
    };
  return &descriptor;
}

ISR(USART0_RX_vect) {
  char received_data = UDR0;
  ring_buffer_queue(&usart0_recv_ring_buf, received_data);
}

ISR(USART0_UDRE_vect) {
  char data;
  if(ring_buffer_dequeue(&usart0_send_ring_buf, &data) > 0)
    UDR0 = data;
  else
    UCSR0B &= ~(1 << UDRIE0);
}
