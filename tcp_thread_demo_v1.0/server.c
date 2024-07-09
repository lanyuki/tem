/*
 * Usage: 
     ./server [serv_port]
 *	e.g.,  ./server  5003    
            argv[0]   argv[1]
 */
#include "net.h"
#include <pthread.h>
#include "debug.h"
#include <assert.h>
int cli_data_handle(void *arg);
/*程序使用方法的提示函数*/
void usage(char *s)
{
	printf("Usage:\n\t%s [serv_port]\n", s);
	printf("\tserv_port: the port value for server(serv_port >= 5000). \n\t\tdefault is 5002.\n\n");
}
int main(int argc, char *argv[])
{
   int sock_fd = -1;
   struct sockaddr_in sin;
   
   /*-----优化2：把端口改为可作为参数进行传递------ */
   int port = SERV_DEF_PORT;
   if(argc != 1 && argc != 2) {
	usage(argv[0]);
	exit(1);
   }
   if(argc == 2 ) {  
	   int itmp = -1;
	   itmp = atoi(argv[1]);  /*把类似‘5003’的字符串转成数字*/
	   if(itmp < 5000) {
		  usage(argv[0]);
		  exit(1);
	   }
	   port = itmp;
   }
    /*-----------end of 优化2----------*/
	
   /*1.创建基于internet的TCP套接字 */
   sock_fd = socket(AF_INET, SOCK_STREAM, 0);
   if(sock_fd < 0) {
   	 perror("socket");
  	 exit(1);
   }
   /* 优化3： 允许本地的地址和端口快速重用[设置要在socket()之后，bind()之前]*/
   int reuse_on = 1;
   setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_on, sizeof(int));
   
   /*2.把socket和特定的IP地址和端口绑定 */
   /*2.1 填充sockaddr_in结构体*/
   bzero(&sin, sizeof(sin)); /*把填充字节sin_zero[8]清零*/
   sin.sin_family = AF_INET;
   sin.sin_port = htons(port);  /*网络字节序端口*/
   //优化1： 绑定的本地任意IP地址
   //   sin.sin_addr.s_addr =  inet_addr(SERV_IP); /*32位网络字节序的IP地址*/
   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   /*2.2绑定*/
   if( bind(sock_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind");
		exit(1);
   }
   
   /*3. 把主动套接字变成被动套节字 */
   if(listen(sock_fd, BACKLOG) < 0) {
   		perror("listen");
		exit(1);
   }
   printf("server(port=%d) starting....OK!\n", port);
   /*4.阻塞等待客户端的连接*/
   int new_fd = -1;

  /*4.取得客户端的信息*/
   struct sockaddr_in cin;

   socklen_t addrlen = sizeof(cin);  
   while(1) {   
	   new_fd  = accept(sock_fd, (struct sockaddr *)&cin, &addrlen); 
	   if(new_fd < 0) {
		  printf("Client connect is errror"); 
		  continue;
	   }
	  /*思路： 
       1.有一个新的客户端连接过来之后，创建一个新的线程来和新的客户端端交互
       2.要从主线程带给子线程信息： new_fd, 连接上来的客户信息
     */ 
	  //填写要带给子线程的数据
	  pthread_t tid;
	  struct cli_info cinfo;
	  cinfo.cli_fd = new_fd;
	  memcpy(&cinfo.cin,&cin, sizeof(cin));
	  pthread_create(&tid, NULL, (void *)cli_data_handle, (void *)&cinfo);	
	  //pthread_create(&tid, NULL, (void *)cli_data_handle, NULL);		
  } 
 
   close(sock_fd);
   return 0;
}

int cli_data_handle(void *arg)
{
	 int ret = -1;
	int new_fd; 
	struct sockaddr_in cin;
	struct cli_info *pInfo = (struct cli_info *)arg;
	
	assert(pInfo != NULL);
	if(!arg) return ret;  /*防止主线程传递空的参数*/

	//取出主线程带过来的数据
    new_fd = pInfo->cli_fd;
	memcpy(&cin, &pInfo->cin, sizeof(cin));
    
	/*把子线程设置成分离属性*/
	pthread_detach(pthread_self());
	
   char cli_ip[16];	
   bzero(cli_ip, IPV4_STR_IP_LEN);
   inet_ntop(AF_INET, (void *)&cin.sin_addr,cli_ip, sizeof(cin));
   printf("Client(%s:%d) is conneted!\n",cli_ip, ntohs(cin.sin_port));  //打印连接过来的客户端的IP地址和端口号
   
  
   char buf[BUFSIZ];
   while(1) {
   		bzero(buf, BUFSIZ);
 		/*和新连接的客户端进行交互*/
		ret = read(new_fd, buf, BUFSIZ);
		if(!ret) {  //  0==ret: 表示客户端已经关闭 		
			break;
		}			
        /*分析读到的客户端数据*/
        //...FIXME!!!

       /*简单打印一下客户端的信息:只用于调试用，正式版不需要此打印信息*/
       hoostar_dbg("Client(%s:%d) said:%s\n", cli_ip, ntohs(cin.sin_port), buf);
	   
    } 
   	close(new_fd);
	return 0;	
}
 

   
  