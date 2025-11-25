/*
 * fsm.h
 *
 *  Created on: Nov 23, 2025
 *      Author: ADMIN
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"
#include "software_timer.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define init 0
#define read 1
#define stop 2

#define RST 10
#define OK 11

#define MAX_BUFFER_SIZE  30
#define MAX_CMD_SIZE 5

extern uint8_t buffer_byte;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

void command_parser_fsm();
void uart_communication_fsm();

#endif /* INC_FSM_H_ */
