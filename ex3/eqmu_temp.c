#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#define TEMP_DATA_FILE "temp_data.txt"

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
        // 等待一秒
        sleep(1);
    }
	fclose(fp);
    return 0;
}