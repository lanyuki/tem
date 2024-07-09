/*
 * Usage: 
     ./client serv_ip [serv_port]
 *	e.g.,  ./client 192.168.20.4 [5003]    
            argv[0]   argv[1]    argv[2]
 */
#include <time.h>
#include <json/json.h>
#include "net.h"
int generate_new_temp(int current_temp);


/*程序使用方法的提示函数*/
void usage(char *s)
{
	printf("Usage:\n\t%s serv_ip [serv_port]\n", s);
	printf("\tserv_ip: the IP address of server.");
	printf("\tserv_port: the port value for server(serv_port >= 5000). \n\t\tdefault is 5002.\n\n");
}
int main(int argc, char *argv[])
{
   int fd = -1;
   struct sockaddr_in sin;
  
    /*-----把服务器的IP地址和端口改为可作为参数进行传递------ */
    if(argc !=2 && argc !=3 ){
		usage(argv[0]);
		exit(1);
	}
	int port = SERV_DEF_PORT;
    if(argc ==3 ){
		int itmp = -1;
	   itmp = atoi(argv[2]);  /*把类似‘5003’的字符串转成数字*/
	   if(itmp < 5000) {
		  usage(argv[0]);
		  exit(1);
	   }
	   port = itmp;
	}		
   /*1.创建基于internet的TCP套接字 */
   fd = socket(AF_INET, SOCK_STREAM, 0);
   if(fd < 0) {
   	 perror("socket");
  	 exit(1);
   }
   /*2. 连接服务器*/
   /*2.1 填充sockaddr_in结构体*/
   bzero(&sin, sizeof(sin)); /*把填充字节sin_zero[8]清零*/
   sin.sin_family = AF_INET;
   sin.sin_port = htons(port);  /*网络字节序端口*/
   sin.sin_addr.s_addr =  inet_addr(argv[1]); /*32位网络字节序的IP地址*/
   /*2.2连接服务器*/
   if( connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind");
		exit(1);
   }
   printf("Client connect server(%s:%d) is successful!\n", argv[1], port);
   
   /*3.从客户端程序所在的机器上的键盘上获取数据，并通过网络发送到服务器*/
   char buf[BUFSIZ];
   int ret  = -1, len;
   
    // 初始化随机数生成器
    srand(time(0));

    // 初始温度在-10到40之间
    int temp = (rand() % 51) - 10;
   //int temp = 30;
   while(1) {
	
    	 // 模拟生成新温度
        temp = generate_new_temp(temp);
		 
		 //打包成JSON格式
		struct json_object *json_obj = json_object_new_object();
		json_object_object_add(json_obj,"temperature",json_object_new_int(temp));
		const char *json_str = json_object_to_json_string(json_obj);
		len = strlen(json_str);
		ret = write(fd,json_str,len);
		json_object_put(json_obj);
				
		if(!ret)  {  //写数据为表示： 对方已经关闭连接
			printf("Server has shutdown!\n");
			break;
		}
		if(ret != len) {  //数据没发送完，当一般的普通错误处理，不退出程序
			printf("Send data to server error\n");
			continue;
		}
		
		// 等待一秒
        sleep(1);
   }
   close(fd);
   return 0;
}


// 生成新的温度值
int generate_new_temp(int current_temp) {
    int change = (rand() % 3) - 1; // -1到1的变化幅度
    int new_temp = current_temp + change;

    // 保证新温度在-10到40之间
    if (new_temp < -10) {
        new_temp = -10;
    } else if (new_temp > 40) {
        new_temp = 40;
    }

    return new_temp;
}

