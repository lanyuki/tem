#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#include <json/json.h>

#define TEMP_DATA_FILE "temp_data.txt"
#define DEV_ID "hoostar01"

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

int main() {
	
	FILE *fp = fopen(TEMP_DATA_FILE, "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
	
    // 初始化随机数生成器
    srand(time(0));

    // 初始温度在-10到40之间
    int temp = (rand() % 51) - 10;
	char buf[BUFSIZ];

    while (1) {
        // 生成新温度
        temp = generate_new_temp(temp);
	
        // 打印当前温度
		bzero(buf, BUFSIZ);
        sprintf(buf, "当前温度: %d°C\n", temp);
		printf("%s", buf);
		fwrite(buf, strlen(buf),1, fp);
		fflush(fp);
		
	    //打包成JSON格式
		struct json_object *json_obj, *dev_id, *dev_temp;
		const char *json_str;
		int json_int;
		
		json_obj = json_object_new_object();
		dev_id = json_object_new_string(DEV_ID);	
		dev_temp = json_object_new_int(temp);
		json_object_object_add(json_obj,"id",dev_id);
		json_object_object_add(json_obj,"temperature",dev_temp);
		json_str = json_object_to_json_string(json_obj);
		printf("Json pack string: %s\n", json_str);
		bzero(buf, BUFSIZ);
        sprintf(buf, "%s", json_str);  //把json字符串保存到BUF，以备后用
		json_object_put(json_obj);
		
			
		//拆包演示
		printf("string to unpack: %s\n", buf);	
//    	struct json_object *json_obj, *dev_id, *dev_temp;
//		const char *json_str;		
		json_obj =json_tokener_parse(buf);
	    dev_id =json_object_object_get(json_obj,"id");
	    dev_temp =json_object_object_get(json_obj,"temperature");
	   
	   json_str =json_object_get_string(dev_id);
       printf("id:%s\t", json_str);
	   json_int =json_object_get_int(dev_temp);
	   printf("temperature:%d\t", json_int);  	   
	   printf("\n===============\n\n"); 
	   
        // 等待一秒
        sleep(1);
    }
	fclose(fp);
    return 0;
}