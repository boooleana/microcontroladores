#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"


#define ESC_REG(x)                  (*((volatile uint32_t *)(x)))


//defines novos
//Registradores
#define SYSCTL_RCGCGPIO             0x400FE608
#define GPIO_OS_DIR                 0x400
#define GPIO_OS_DEN                 0x51C
#define GPIO_OS_DR2R                0x500
#define GPIO_OS_DR4R                0x504
#define GPIO_OS_DR8R                0x508
#define GPIO_OS_SLR                 0x518
#define GPIO_OS_PULLUP              0x510
#define GPIO_OS_DATA                0x000

#define GPIOPortA_base              0x40004000
#define GPIOPortB_base              0x40005000
#define GPIOPortC_base              0x40006000
#define GPIOPortD_base              0x40007000
#define GPIOPortE_base              0x40024000
#define GPIOPortF_base              0x40025000

//defines Funcoes
//HabilitaPortal
#define HabPortalA                  0x01
#define HabPortalB                  0x02
#define HabPortalC                  0x04
#define HabPortalD                  0x08
#define HabPortalE                  0x10
#define HabPortalF                  0x20

//ConfiguraPinoSaida
#define Pino_0                      0x01
#define Pino_1                      0x02
#define Pino_2                      0x04
#define Pino_3                      0x08
#define Pino_4                      0x10
#define Pino_5                      0x20
#define Pino_6                      0x40
#define Pino_7                      0x80

//Valores Display 7seg
#define sseg_0                      0b00111111
#define sseg_1                      0b00000110
#define sseg_2                      0b01011011
#define sseg_3                      0b01001111
#define sseg_4                      0b01100110
#define sseg_5                      0b01101101
#define sseg_6                      0b01111101
#define sseg_7                      0b00000111
#define sseg_8                      0b01111111
#define sseg_9                      0b01101111
#define sseg_A                      0b01110111
#define sseg_B                      0b01111100
#define sseg_C                      0b00111001
#define sseg_D                      0b01011110
#define sseg_E                      0b01111001
#define sseg_F                      0b01110001


void HabilitaPortal (uint8_t HabPortalX)
{
    ESC_REG(SYSCTL_RCGCGPIO) |= HabPortalX;

}

void ConfiguraPinoSaida (uint32_t PortalBase, uint8_t Pino)
{
    ESC_REG(PortalBase + GPIO_OS_DIR) |= Pino;
    ESC_REG(PortalBase + GPIO_OS_DEN) |= Pino;
    ESC_REG(PortalBase + GPIO_OS_DR2R) |= Pino;
    ESC_REG(PortalBase + GPIO_OS_DR4R) &= ~(Pino);
    ESC_REG(PortalBase + GPIO_OS_DR8R) &= ~(Pino);
    ESC_REG(PortalBase + GPIO_OS_SLR) &= ~(Pino);

}

void ConfiguraPinoEntrada (uint32_t PortalBase, uint8_t Pino)
{
    ESC_REG(PortalBase + GPIO_OS_DIR) &= ~(Pino);
    ESC_REG(PortalBase + GPIO_OS_DEN) |= Pino;
    ESC_REG(PortalBase + GPIO_OS_DR2R) |= Pino;
    ESC_REG(PortalBase + GPIO_OS_DR4R) &= ~(Pino);
    ESC_REG(PortalBase + GPIO_OS_DR8R) &= ~(Pino);
    ESC_REG(PortalBase + GPIO_OS_SLR) &= ~(Pino);

}

int32_t LeituraPinosPortal (uint32_t PortalBase, uint8_t Pino)
{
    return(ESC_REG(PortalBase + (GPIO_OS_DATA + (Pino << 2))));
}

void EscritaPinosPortal (uint32_t PortalBase, uint8_t Pino, uint8_t Valor)
{
    ESC_REG(PortalBase + (GPIO_OS_DATA + (Pino << 2))) = Valor;
}

void desligaDisplay(){
    EscritaPinosPortal(GPIOPortB_base, Pino_6, 0x40);
    EscritaPinosPortal(GPIOPortB_base, Pino_7, 0x80);
    EscritaPinosPortal(GPIOPortD_base, Pino_2, 0x04);
    EscritaPinosPortal(GPIOPortD_base, Pino_3, 0x08);
}

void desligaDig(int n){
    switch (n){

        case 1:
            EscritaPinosPortal(GPIOPortB_base, Pino_6, 0x40);
            break;
        case 2:
            EscritaPinosPortal(GPIOPortB_base, Pino_7, 0x80);
            break;
        case 3:
            EscritaPinosPortal(GPIOPortD_base, Pino_2, 0x04);
            break;
        case 4:
            EscritaPinosPortal(GPIOPortD_base, Pino_3, 0x08);
            break;

    }
}

void ligaDig(int n){
    switch (n){

        case 1:
            EscritaPinosPortal(GPIOPortB_base, Pino_6, 0x00);
            break;
        case 2:
            EscritaPinosPortal(GPIOPortB_base, Pino_7, 0x00);
            break;
        case 3:
            EscritaPinosPortal(GPIOPortD_base, Pino_2, 0x00);
            break;
        case 4:
            EscritaPinosPortal(GPIOPortD_base, Pino_3, 0x00);
            break;

    }
}

void Display(int dig, uint8_t Valor){//B6,B7,D2,D3
    desligaDisplay();
    ligaDig(dig);
    EscritaPinosPortal(GPIOPortE_base, Pino_0|Pino_1|Pino_2|Pino_3, Valor);
    EscritaPinosPortal(GPIOPortC_base, Pino_4|Pino_5|Pino_6|Pino_7, Valor);
}


// Variaveis globais
uint8_t Valores[16] = {sseg_0, sseg_1, sseg_2, sseg_3, sseg_4, sseg_5, sseg_6, sseg_7, sseg_8, sseg_9, sseg_A, sseg_B, sseg_C, sseg_D, sseg_E,  sseg_F};
int i=0;
int counter = 0, aux_counter = 0, uni, dez, cen, mil, delay;

int main(void)
{
    volatile uint32_t ui32Loop;


    HabilitaPortal(HabPortalB);
    HabilitaPortal(HabPortalC);
    HabilitaPortal(HabPortalD);
    HabilitaPortal(HabPortalE);
    HabilitaPortal(HabPortalF);

    // Faz leitura dummy para efeito de atraso
    for(ui32Loop = 0; ui32Loop < 200; ui32Loop++) { }

    // Habilita o pino 3 do portal F, configura como saída digital
    ConfiguraPinoSaida (GPIOPortF_base, Pino_3|Pino_0);
    ConfiguraPinoSaida (GPIOPortE_base, Pino_0|Pino_1|Pino_2|Pino_3);
    ConfiguraPinoSaida (GPIOPortD_base, Pino_2|Pino_3);
    ConfiguraPinoSaida (GPIOPortC_base, Pino_4|Pino_5|Pino_6|Pino_7);
    ConfiguraPinoSaida (GPIOPortB_base, Pino_6|Pino_7);

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Habilita o pino 4 do portal, configura como entrada com weak pull up
    ConfiguraPinoEntrada (GPIOPortF_base, Pino_4);
    ESC_REG(GPIOPortF_base + GPIO_OS_PULLUP)|= Pino_4;


    // Loop principal
    while(1)
        {
            mil = counter/1000;
            cen = (counter%1000)/100;
            dez = (counter%100)/10;
            uni = counter%10;
            delay = 1000;


            aux_counter++;
            if (aux_counter > 50){
                aux_counter = 0;
                counter++;
            }
            if (counter > 9999) counter = 0;



            for(ui32Loop = 0; ui32Loop < delay; ui32Loop++) {  } // Atraso
            Display(1,Valores[mil]);

            for(ui32Loop = 0; ui32Loop < delay; ui32Loop++) {  } // Atraso
            Display(2,Valores[cen]);

            for(ui32Loop = 0; ui32Loop < delay; ui32Loop++) {  } // Atraso
            Display(3,Valores[dez]);

            for(ui32Loop = 0; ui32Loop < delay; ui32Loop++) {  } // Atraso
            Display(4,Valores[uni]);


        }
}
