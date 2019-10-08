*/server.h
*
*  Created on: Feb 18, 2017
*      Author: zach, from SCUT
*/

#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#define    EMPTY             "C0"
#define    SERV_PORT       1234    //服务器的端口号
#define    IP_NUM          "192.168.2.233"   //服务器的IP地址
#define    LIST_MAX        30  //连接请求队列的最大长度，这里指网络断开重接的次数
#define    COM_LEN         2   //接收命令的长度

//服务器结构体
typedef struct server
{
	//网络套接字
	int socket_descriptor;
	//套接字接收标志
	int connection_status;
	//服务器接收到的命令
	char recv_command[COM_LEN];
	//套接字IP
	struct sockaddr_in serv_addr;
}s_server;

void server_init();     //服务器配置的函数
void server_check();    //服务器等待监测的函数
void serv_recv();       //接受客户机命令
void server_start();    //启动服务器