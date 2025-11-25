/*
 * software_timer.h
 *
 *  Created on: Nov 23, 2025
 *      Author: ADMIN
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define TICK 10

extern int timer1_flag;
extern int timer2_flag;
extern int timer3_flag;

void setTimer1(int duration);
void setTimer2(int duration);
void timerRun();
void setTimer3 (int duration);

#endif /* INC_SOFTWARE_TIMER_H_ */
