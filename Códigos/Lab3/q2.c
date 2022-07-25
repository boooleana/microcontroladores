#include <stdint.h>

#define ESC_REG(x)                  (*((volatile uint32_t *)(x)))

#define SYSCTL_RCGC2_R              0x400FE108
#define SYSCTL_RCGC2_GPIOF          0x00000020
#define GPIO_PORTF_DIR_R            0x40025400
#define GPIO_PORTF_DEN_R            0x4002551C
#define GPIO_PORTF_DATA_R           0x400253FC
#define GPIO_PORTF_DR2R             0x40025500
#define GPIO_PORTF_PUR              0x40025510
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

void Display(uint8_t Valor){//B6,B7,D2,D3
    EscritaPinosPortal(GPIOPortB_base, Pino_6, 0x00);
    EscritaPinosPortal(GPIOPortE_base, Pino_0|Pino_1|Pino_2|Pino_3, Valor);
    EscritaPinosPortal(GPIOPortC_base, Pino_4|Pino_5|Pino_6|Pino_7, Valor);
}





// Variaveis globais
uint8_t Valores[16] = {sseg_0, sseg_1, sseg_2, sseg_3, sseg_4, sseg_5, sseg_6, sseg_7, sseg_8, sseg_9, sseg_A, sseg_B, sseg_C, sseg_D, sseg_E,  sseg_F};
int i=0;

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
    ConfiguraPinoSaida (GPIOPortF_base, Pino_3);
    ConfiguraPinoSaida (GPIOPortE_base, Pino_0|Pino_1|Pino_2|Pino_3);
    ConfiguraPinoSaida (GPIOPortD_base, Pino_2|Pino_3);
    ConfiguraPinoSaida (GPIOPortC_base, Pino_4|Pino_5|Pino_6|Pino_7);
    ConfiguraPinoSaida (GPIOPortB_base, Pino_6|Pino_7);


    // Habilita o pino 4 do portal, configura como entrada com weak pull up
    ConfiguraPinoEntrada (GPIOPortF_base, Pino_4);
    ESC_REG(GPIOPortF_base + GPIO_OS_PULLUP)|= Pino_4;

    // Loop principal
    while(1)
        {

            // Atraso
            for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++) {  }
            Display(Valores[i]);
            i++;
            if(i == 16) i = 0;


        }
}
