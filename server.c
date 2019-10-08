/*
*  server.c
*
*  Created on: Feb 18, 2017
*      Author: zach, from SCUT
*/

#include "server.h"

//实例化一个服务器
s_server   server;

//客户端结构体的大小
socklen_t  cli_len = sizeof(struct sockaddr_in);

//全局通信信号
char control_signal[COM_LEN];

/*------------------------------------------------------*/

//服务器配置的函数
void server_init()
{
	//创建一个套接字
	server.socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (server.socket_descriptor < 0)
	{
		printf("[ ERROR ] Socket_descriptor.\n");
		//return FALSE;
		//perror("socket");
		exit(1);
	}

	//初始化服务器端地址结构
	memset(&server.serv_addr, 0, sizeof(server.serv_addr));
	server.serv_addr.sin_family = AF_INET;
	server.serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server.serv_addr.sin_port = htons(SERV_PORT);

	//将套接字绑定到本地端口
	int bind_status = -1;
	for (int i = 0; bind_status == -1; i++)
	{
		bind_status = bind(server.socket_descriptor, (struct sockaddr*)&server.serv_addr, sizeof(server.serv_addr));
		if (bind_status == 0)
		{
			break;
		}
	}
	if (bind_status == 0)
	{
		printf("[ OK ] Bind host port.\n");
	}
	else
	{
		printf("[ ERROR ] Bind host port.\n");
		//return FALSE;
	//	exit(1);
	}

	//开始监听
	int listen_id = listen(server.socket_descriptor, LIST_MAX);
	if (listen_id == -1)
	{
		printf("[ ERROR ] Listen host connection.\n");
		//return FALSE;
	//	exit(1);
	}
}


//服务器等待监测的函数
void server_check()
{
	while (1)
	{
		server.connection_status = accept(server.socket_descriptor, (struct sockaddr*) NULL, &cli_len);
		if (server.connection_status < 0)
		{
			printf("[ ERROR ] Accept new connection.\n");
		}
		else
		{
			printf("[ OK ] Accept new connection.\n");
		}
	}
}

//接受客户机命令
void serv_recv()
{
while(1)
{
	if (recv(server.connection_status, server.recv_command, sizeof(server.connection_status), 0) <= 0)
	{
		//若接收信息失败，命令为 EMPTY
		//printf("[ WAIT ] No new command received.\n");
		for (int i = 0; i < sizeof(control_signal); i++)
		{
			server.recv_command[i] = EMPTY[i];
			control_signal[i] = EMPTY[i];
		}
		usleep(500000);

	}
	else
	{
		//信息接收成功
		printf("[ OK ] New command received.\n");
		if (server.recv_command[0] == 'C')  //校验位为 'C'
		{
			for (int i = 0; i < sizeof(control_signal); i++)
			{
				control_signal[i] = server.recv_command[i];
			}
			printf("Successfully received message: %s\n", control_signal);
		}
	}
	usleep(50000);
	//printf("test:\n  server.recv_command : %s control_signal: %s",server.recv_command, control_signal);
}
}


//启动服务器
void server_start()
{
	int ret;
	pthread_t server_check_id;
	//pthread_t server_receiver_id;

	//配置服务器
	server_init();
	//创建accept专用线程，用于等待连接
	if((ret = pthread_create (&server_check_id, NULL, (void *)server_check, NULL)) == 0)
	{
		printf("[ OK ] Server connection thread created.\n");
	}
	else
	{
		printf("[ ERROR ] Server connection thread created.\n");
	//	exit(1);
	}
	serv_recv();
	//创建receiver专用线程，用于接收数据
	//if((ret = pthread_create (&server_receiver_id, NULL, (void *)serv_recv, NULL)) == 0)
	//{
	//	printf("[ OK ] Server receiver thread created.\n");
	//}
	//else
	//{
	//	printf("[ ERROR ] Server receiver thread created.\n");
	//	exit(1);
	//}
}
