#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "driverlib/systick.h"
#include "inc/hw_gpio.h"

uint8_t ui8PinData=2;

void SystickIntHandler (void){
   uint8_t ui8IntSource;

   ui8IntSource = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0);

   if ((ui8IntSource & 0x08) ==  0x08)
   {
       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
   }
   else
       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xFF);


   if ((ui8IntSource & 0x01) ==  0x00) //SW PF0
    {
        if ((ui8IntSource & 0x02) == 0x02)
                       {
                           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
                       }
                   else
                       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    }
    if ((ui8IntSource & 0x10) ==  0x00)//SW PF4
        {
            if ((ui8IntSource & 0x04) == 0x04)
                           {
                               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
                           }
                       else
                           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
         }

}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    SysTickPeriodSet(0x00FFFFFF);
    SysTickIntEnable();
    SysTickEnable();


    while(1)
        {


        }

}
