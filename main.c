/*
*  main.c
*
*  Created on: Feb 18, 2017
*      Author: zach, from SCUT
*/

#include "server.h"
#include "motor.h"

int main()
{
    printf("main funtion test successfully");
    motor_run();
    server_start();
}
