/*
*  motor.h
*
*  Created on: Feb 18, 2017
*      Author: zach, from SCUT
*/
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <wiringPi.h>
#include "server.h"


#define    EMPTY           "C0" //命令0   空命令
#define    FORWARD         "C1" //命令1   向前
#define    BACKWARD        "C2" //命令2   向后
#define    RIGHT           "C3" //命令3   向左
#define    LEFT            "C4" //命令4   向右
#define    FORLEFT         "C5" //命令5   向左前
#define    FORRIGHT        "C6" //命令6   向右前
#define    SPEED_LOW1      1550  //速度为一档
#define    SPEED_LOW2      1650  //速度为二档
#define    SPEED_MID       1750  //速度为三档
#define    SPEED_HIGH1     1850 //速度为四挡
#define    SPEED_HIGH2     1920 //速度为五档
#define    SPEED_BACK      1300 //退档
#define    TIME_PWM_PERIOD 2000 //PWM周期为2ms
#define    TIME_DEP        1500 //电调起始时长    1.5ms
#define    TIME_3_S        1500 //1500*2ms = 3s
#define    LFETMOTOR_PIN   (0)  //左电机
#define    RIGHTMOTOR_PIN  (1)  //右电机
#define    SWITCH          (5)  //继电器开关，17_04_16加入，用于切换<树莓派/遥控器>的控制

typedef struct
{
    //高电平时间
    int time_h;
    //低电平时间
    int time_l;

}motor_pwm;

void motor_setup();         //引脚配置，电调初始化
void *  motor_left_thread();   //左电机控制线程
void *  motor_right_thread();  //右电机控制线程
void motor_init_3_s();      //电调最初3s初始化
void judge_state();         //命令判断函数
void motor_run();           //启动电机


#endif  /* _MOTOR_H_ */
