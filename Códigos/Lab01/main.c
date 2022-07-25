#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"

#define ESC_REG(x)					(*((volatile uint32_t *)(x)))

#define SYSCTL_RCGC2_R          	0x400FE108
#define SYSCTL_RCGC2_GPIOF      	0x00000020
#define GPIO_PORTF_DIR_R       		0x40025400
#define GPIO_PORTF_DEN_R        	0x4002551C
#define GPIO_PORTF_DATA_R       	0x400253FC

int main(void)
{
    volatile uint32_t ui32Loop;

    // Habilita o portal de GPIO F
    ESC_REG(SYSCTL_RCGC2_R) = SYSCTL_RCGC2_GPIOF;

    // Faz leitura dummy para efeito de atraso
    ui32Loop = ESC_REG(SYSCTL_RCGC2_R);

    // Habilita o pino 3 do portal F, configura como saída digital
    ESC_REG(GPIO_PORTF_DIR_R) = 0x08;
    ESC_REG(GPIO_PORTF_DEN_R) = 0x08;

    // Loop principal
    while(1)
    {
        // Acende o LED
    	ESC_REG(GPIO_PORTF_DATA_R)|= 0x08;

        // Atraso
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }

        // Apaga o LED
        ESC_REG(GPIO_PORTF_DATA_R) &= ~(0x08);

        // Atraso
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }
    }
}
