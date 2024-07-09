#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main() {
    srand(time(0));
    while (1) {
        int temp = (rand() % 51) - 10;
        printf("Temperature: %d°C\n", temp);
        sleep(1);
    }
    return 0;
}
