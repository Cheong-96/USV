/*
*  motor.c
*
*  Created on: Feb 18, 2017
*      Author: zach, from SCUT
*/

#include "motor.h"
#include <time.h>

motor_pwm motor_left;   //左电机
motor_pwm motor_right;  //右电机
extern  char control_signal[COM_LEN];
//char temp;  //全局控制信号

//引脚配置，电调初始化
void motor_setup()
{
    wiringPiSetup();    //初始化树莓派引脚，使用wiringPi引脚编号

    pinMode(LFETMOTOR_PIN, OUTPUT);
    pinMode(RIGHTMOTOR_PIN, OUTPUT);   //设置左右电机的哦0，1口为输出模式

    pinMode(SWITCH, OUTPUT);
    digitalWrite(SWITCH, HIGH);    //让switch口输出为高电平
    motor_init_3_s();
    control_signal[0] = 'C';
    control_signal[1] = '0';
}

//对发送过来的命令进行判断，修改电调控制信号
void * judge_state_thread()
{
    while(1)
    {
        char temp;
        temp = control_signal[1];
        if(temp == '0')//命令0 EMPTY
        {
            motor_left.time_h = TIME_DEP;
            motor_left.time_l = TIME_PWM_PERIOD - motor_left.time_h;
            motor_right.time_h = TIME_DEP;
            motor_right.time_l = TIME_PWM_PERIOD - motor_right.time_h;
//            printf("[ OK ] Boat stops.\n");
        }
        if(temp == '1')//命令1 向前
        {
            motor_left.time_h = SPEED_HIGH2;
            motor_left.time_l = TIME_PWM_PERIOD - motor_left.time_h;
            motor_right.time_h = SPEED_HIGH2;
            motor_right.time_l = TIME_PWM_PERIOD - motor_right.time_h;
//            printf("[ OK ] Boat goes forward.\n");
        }
        if(temp == '2')//命令2 向后
        {
            motor_left.time_h = SPEED_BACK;
            motor_left.time_l = TIME_PWM_PERIOD - motor_left.time_h;
            motor_right.time_h = SPEED_BACK;
            motor_right.time_l = TIME_PWM_PERIOD - motor_right.time_h;
//            printf("[ OK ] Boat goes backward.\n");
        }
        if(temp == '3')//命令3 向左
        {
            motor_left.time_h = TIME_DEP;
            motor_left.time_l = TIME_PWM_PERIOD - motor_left.time_h;
            motor_right.time_h = SPEED_HIGH1;
            motor_right.time_l = TIME_PWM_PERIOD - motor_right.time_h;
//            printf("[ OK ] Boat turns left.\n");
        }
        if(temp == '4')//命令4 向右
        {
            motor_left.time_h = SPEED_HIGH1;
            motor_left.time_l = TIME_PWM_PERIOD - motor_left.time_h;
            motor_right.time_h = TIME_DEP;
            motor_right.time_l = TIME_PWM_PERIOD - motor_right.time_h;
//            printf("[ OK ] Boat turn right.\n");
        }
        if(temp == '5')//命令5 向左前
        {
            motor_left.time_h = SPEED_LOW2;
            motor_left.time_l = TIME_PWM_PERIOD - motor_left.time_h;
            motor_right.time_h = SPEED_HIGH2;
            motor_right.time_l = TIME_PWM_PERIOD - motor_right.time_h;
//            printf("[ OK ] Boat goes left and forward.\n");
        }
        if(temp == '6')//命令6 向右前
        {
            motor_left.time_h = SPEED_HIGH2;
            motor_left.time_l = TIME_PWM_PERIOD - motor_left.time_h;
            motor_right.time_h = SPEED_LOW2;
            motor_right.time_l = TIME_PWM_PERIOD - motor_right.time_h;
//            printf("[ OK ] Boat goes right and forward.\n");
        }
	if(temp == 'S')//命令S 切换控制主机为遥控器
        {
            digitalWrite(SWITCH, LOW);
            printf("[ OFF ] Boat_by_rpi swith is off.\n");
        }
	if(temp == 'R')//命令S 切换控制主机为rpi
        {
            digitalWrite(SWITCH, HIGH);
            printf("[ ON ] Boat_by_rpi swith is on.\n");
        }
    }
}

//左电机控制线程
void * motor_left_thread()
{
    while(1)
    {
//        printf("TEST:L__MOTOR RUN!!!!!!!!!!!!!!!!!!!!!!!\n");
        digitalWrite(LFETMOTOR_PIN, HIGH);
        usleep(motor_left.time_h);
        digitalWrite(LFETMOTOR_PIN, LOW);
        usleep(motor_left.time_l);
    }
}

//右电机控制线程
void * motor_right_thread()
{
    while(1)
    {
//	printf("TEST:R__MOTOR RUN!!!!!!!!!!!!!!!!!!!!!!!\n");
        digitalWrite(RIGHTMOTOR_PIN, HIGH);
        usleep(motor_right.time_h);
        digitalWrite(RIGHTMOTOR_PIN, LOW);
        usleep(motor_right.time_l);
    }
}

//电调最初3s初始化
void motor_init_3_s()
{
    motor_right.time_h = TIME_DEP;
    motor_right.time_l = TIME_PWM_PERIOD - motor_right.time_h;
    int time;
    for(time=0; time < TIME_3_S; time++)
    {
        digitalWrite(LFETMOTOR_PIN, HIGH);  //此处仅利用右电机的参数是为了避免线程
        digitalWrite(RIGHTMOTOR_PIN, HIGH);
        usleep(motor_right.time_h);
        digitalWrite(LFETMOTOR_PIN, LOW);
        digitalWrite(RIGHTMOTOR_PIN, LOW);
        usleep(motor_right.time_l);
    }
}

void motor_run()
{
    motor_setup();
    pthread_t judge_state_thread_id;
    pthread_t motor_left_thread_id;
    pthread_t motor_right_thread_id;
    if((pthread_create (&judge_state_thread_id, NULL, (void *)judge_state_thread, NULL)) == 0)
	{
		printf("[ OK ] Judging state thread created.\n");
	}
	else
	{
		printf("[ ERROR ] Judging state thread created.\n");
	}
    if((pthread_create (&motor_left_thread_id, NULL, (void *)motor_left_thread, NULL)) == 0)
	{
		printf("[ OK ] Left motor thread created.\n");
	}
	else
	{
		printf("[ ERROR ] Left motor thread created.\n");
	}
    if((pthread_create (&motor_right_thread_id, NULL, (void *)motor_right_thread, NULL)) == 0)
	{
		printf("[ OK ] Right motor thread created.\n");
	}
	else
	{
		printf("[ ERROR ] Right motor thread created.\n");
	}
}
