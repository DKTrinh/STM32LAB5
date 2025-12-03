/*
 * fsm.c
 *
 *  Created on: Nov 21, 2025
 *      Author: ADMIN
 */
#include "fsm.h"
//ADC_HandleTypeDef hadc1;
//UART_HandleTypeDef huart2;

uint8_t buffer_byte = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

int status = INIT;
uint8_t cmd_flag = INIT;
uint8_t cmd_data[MAX_CMD_SIZE];
uint8_t cmd_data_index = 0;
int ADC_value = 0;


int isCmdEqualToRST(uint8_t str[]){
	if (str[0] == 'R' && str[1] == 'S' && str[2] == 'T') {
		return 1;
	}

	return 0;
}

int isCmdEqualToOK(uint8_t str[]){
	if (str[0] == 'O' && str[1] == 'K')
		return 1;
	return 0;
}

void command_parser_fsm(ADC_HandleTypeDef* hadc1, UART_HandleTypeDef* huart2) {
	char str[50];
	switch(status) {
		case INIT:
			if(buffer_byte == '!') {
				status = READING;
				cmd_data_index = 0;
			}
			break;
		case READING:
			if (buffer_byte == '!') {
				cmd_data_index = 0;
			}
			else if (buffer_byte == '#') {
				if (isCmdEqualToRST(cmd_data) == 1){
					cmd_flag = RST;
					ADC_value = HAL_ADC_GetValue(hadc1);
					HAL_UART_Transmit(huart2, (void *)str, sprintf(str, "!ADC=%d#\r\n", ADC_value), 1000);
					setTimer1(3000);
				}
				else if (isCmdEqualToOK(cmd_data) == 1){
					cmd_flag = OK;
				}
				status = INIT;
				cmd_data_index = 0;
			}
			else {
				cmd_data[cmd_data_index] = buffer_byte;
				cmd_data_index++;
				if(cmd_data_index >= MAX_CMD_SIZE) {
					cmd_data_index = 0;
				}
			}
			break;
		default:
			break;
	}
}

void uart_communication_fsm(ADC_HandleTypeDef* hadc1, UART_HandleTypeDef* huart2) {
	char str[50];
	switch(cmd_flag){
		case INIT:
			break;
		case RST:
			if(timer1_flag == 1){
				HAL_UART_Transmit(huart2, (void *)str, sprintf(str, "!ADC=%d#\r\n", ADC_value), 1000);
				setTimer1(3000);
			}
			break;
		case OK:
			ADC_value = 0;
			timer1_flag = 0;
			cmd_flag = INIT;
			break;
		default:
			break;
	}
}
