#include "main.h"
#include "system_clock.h"
#include "usart.h"

int main()
{
    HAL_Init();
    SystemClock_Config();

    while (true) {
    }

    return 0;
}
