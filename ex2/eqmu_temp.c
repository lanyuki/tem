#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
    // 初始化随机数生成器
    srand(time(0));

    // 初始温度在-10到40之间
    int temp = (rand() % 51) - 10;

    while (1) {
        // 生成新温度
        temp = generate_new_temp(temp);

        // 打印当前温度
        printf("当前温度: %d°C\n", temp);

        // 等待一秒
        sleep(1);
    }

    return 0;
}