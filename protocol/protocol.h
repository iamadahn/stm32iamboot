#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "usart.h"
#include "stdint.h"

#define HANDSHAKE_LENGTH            10U
#define ACK_LENGTH                  4U
#define FIRMWARE_BYTES_LENGTH       56U
#define TOTAL_MSG_LENGTH            64U

uint8_t handshake_send_serial(usart_t* usart);
uint8_t handshake_receive_serial(usart_t* usart, uint32_t* number_of_packets);
uint8_t ack_send_serial(usart_t* usart);

#endif