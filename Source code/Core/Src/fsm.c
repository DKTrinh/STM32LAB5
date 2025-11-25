/*
 * fsm.c
 *
 *  Created on: Nov 23, 2025
 *      Author: ADMIN
 */
#include "fsm.h"

uint8_t buffer_byte = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

static int parser_state = init;
static uint8_t protocol_state = init;
static uint8_t cmd_store[MAX_CMD_SIZE];
static uint8_t cmd_idx = 0;
static int adc_cache = 0;

static int matchRST(uint8_t *p) {
    return (p[0] == 'R' && p[1] == 'S' && p[2] == 'T');
}

static int matchOK(uint8_t *p) {
    return (p[0] == 'O' && p[1] == 'K');
}

void command_parser_fsm(ADC_HandleTypeDef* hadc, UART_HandleTypeDef* huart) {
    char outbuff[50];

    switch (parser_state) {
        case init:
            if (buffer_byte == '!') parser_state = read;
            break;

        case read:
            if (buffer_byte != '!' && buffer_byte != '#') {
                cmd_store[cmd_idx++] = buffer_byte;
                if (cmd_idx > 3) {
                    cmd_idx = 0;
                    parser_state = stop;
                }
            }
            if (buffer_byte == '#') {
                parser_state = stop;
                cmd_idx = 0;
            }
            break;

        case stop:
            if (matchRST(cmd_store)) {
                protocol_state = RST;
                adc_cache = HAL_ADC_GetValue(hadc);
                HAL_UART_Transmit(huart, (uint8_t*)outbuff, sprintf(outbuff, "!ADC=%d#\r\n", adc_cache), 300);
                setTimer1(3000);
            }
            else if (matchOK(cmd_store)) {
                protocol_state = OK;
            }
            parser_state = init;
            break;

        default:
            parser_state = init;
            break;
    }
}

void uart_communication_fsm(ADC_HandleTypeDef* hadc, UART_HandleTypeDef* huart) {
    char txbuff[50];

    switch (protocol_state) {
        case init:
            break;

        case RST:
            if (timer1_flag) {
                adc_cache = HAL_ADC_GetValue(hadc);
                HAL_UART_Transmit(huart, (uint8_t*)txbuff, sprintf(txbuff, "!ADC=%d#\r\n", adc_cache), 300);
                setTimer1(1000);
            }
            break;

        case OK:
            adc_cache = -1;
            protocol_state = init;
            break;

        default:
            protocol_state = init;
            break;
    }
}
