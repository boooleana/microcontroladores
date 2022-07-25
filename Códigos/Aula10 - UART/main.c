
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"

void Handler_UART0(void){
    UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE)); //echo - Função bloqueante
    UARTIntClear(UART0_BASE,UART_INT_RX | UART_INT_RT);

}
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); // 80 MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0 | GPIO_PIN_1); //Configura pin (s) para uso do periférico UART.
    GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(),115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTEnable(UART0_BASE);
    UARTIntEnable(UART0_BASE,UART_INT_RX | UART_INT_RT);// Podemos habilitar a interrupção por Timeout
    IntEnable(INT_UART0);
    IntMasterEnable();
    //UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    //UARTFIFOEnable(UART0_BASE);

    while(1)
    {

    }
}
