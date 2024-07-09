#ifndef __HOOSTAR_NET_H__
#define __HOOSTAR_NET_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>       
#include <sys/socket.h>
/* man 7 ip得到的头文件 */
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

//#define SERV_DEF_IP "192.168.20.4"
#define SERV_DEF_PORT 5002

#define QUIT_STR "quit"

//the following macro is just for server.c
#define BACKLOG 5
#define IPV4_STR_IP_LEN 16

  struct cli_info {
	int cli_fd;
	struct sockaddr_in cin;	
	//.... more 
  };
#endif
