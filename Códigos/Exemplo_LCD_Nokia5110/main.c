// Exemplo de um gerador de função onde se é possivel escolher entre onda quadrada, rampa, senoidal e triangular
// É possivel ainda configurar a amplitude e frequencia através dos botoes SW1 e SW2 da placa tivaWare

// Pinos utilizados

//    3.3V          (VCC, pin 1) power
//    Ground        (GND, pin 2) ground
//    SSI0Fss       (SCE, pin 3) connected to PA3
//    Reset         (RST, pin 4) connected to PA7
//    Data/Command  (D/C, pin 5) connected to PA6
//    SSI0Tx        (DN,  pin 6) connected to PA5
//    SSI0Clk       (SCLK, pin 7) connected to PA2
//    back light    (LED, pin 8) not connected

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/ssi.h"
#include "Nokia5110.h"
#include <math.h>

//Declaração das funções utilizadas
char* type_Wave[6] = {"", "", "Quadrada", "Rampa", "Senoidal", "Triangular"};
int cordX;
int cordY;
int cont = 0;
int aux = 0;
int plot = 0;
int type_wave = 0;
int amplitude = 0;
int frequencia= 0;
int tipo_Modo = 1;
int ampli_Tipo= 0;
int freq_Tipo = 0;


void GPIOF_Handler(){
    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    //Criando a função que vai ler os botoes SW1 e SW2
    unsigned int leitura = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    //Habiltiando o timer
    TimerEnable(TIMER0_BASE, TIMER_A);

    if(tipo_Modo){
        if(leitura == GPIO_PIN_4){
            if(cordY == 5){
                cordY = 2;
            }else{
                cordY++;
            }
        }
        if(leitura == GPIO_PIN_0){
            type_wave = cordY;
            tipo_Modo = 0;
            ampli_Tipo = 1;
            SysCtlDelay(200000);
            Nokia5110_Clear();
        }
    }else{
        if(ampli_Tipo){
            if(leitura == GPIO_PIN_4){
                if(amplitude < 100){
                    amplitude += 10;
                }else{
                    if(amplitude == 100){
                        Nokia5110_SetCursor(66, 4);
                        Nokia5110_OutString("   ");
                        amplitude = 0;
                    }
                }
            }
            if(leitura == GPIO_PIN_0){
                ampli_Tipo = 0;
                freq_Tipo = 1;
                Nokia5110_Clear();
            }
        }else{
            if(freq_Tipo){
                if(leitura == GPIO_PIN_4){
                    if(frequencia < 450){
                        frequencia += 10;
                    }else{
                        if(frequencia == 440){
                            Nokia5110_SetCursor(66,4);
                            Nokia5110_OutString("   ");
                            frequencia = 0;
                        }
                    }
                }
                if(leitura == GPIO_PIN_0){
                    freq_Tipo = 0;
                    plot = 1;
                    Nokia5110_Clear();
                }
            }
        }
    }
}


void Timer0_Handler(){
    //Limpa as fontes de interrupção do temporizador
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    TimerDisable(TIMER0_BASE, TIMER_A);
}

//  Configurando o timer display
void setup_Timer_display(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0_Handler);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_ONE_SHOT);
    TimerLoadSet(TIMER0_BASE, TIMER_A, 7000000);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

//Configuração dos GPIO utilizados no display
void setup_GPIO_display(){
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
        HWREG(GPIO_PORTF_BASE +  GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
        HWREG(GPIO_PORTF_BASE +  GPIO_O_LOCK) = 0;
        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
        GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
        GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
        GPIOIntRegister(GPIO_PORTF_BASE, GPIOF_Handler);
        GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_FALLING_EDGE);
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
        GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
}

void set_Cursor(int x, int y){
    cordX = x;
    cordY = y;
}

// Configurando o cursosr que sera mostrado no display para o usuario
void load_Cursor(){
    if(cordY == 2){
            Nokia5110_SetCursor(0, 2);
            Nokia5110_OutString("--");
            Nokia5110_SetCursor(0, 3);
            Nokia5110_OutString("  ");
            Nokia5110_SetCursor(0, 3);
            Nokia5110_OutString("  ");
            Nokia5110_SetCursor(0, 5);
            Nokia5110_OutString("  ");
    }
    if(cordY == 3){
            Nokia5110_SetCursor(0, 2);
            Nokia5110_OutString("  ");
            Nokia5110_SetCursor(0, 3);
            Nokia5110_OutString("--");
            Nokia5110_SetCursor(0, 4);
            Nokia5110_OutString("  ");
            Nokia5110_SetCursor(0, 5);
            Nokia5110_OutString("  ");
    }
    if(cordY == 4){
            Nokia5110_SetCursor(0, 2);
            Nokia5110_OutString("  ");
            Nokia5110_SetCursor(0, 3);
            Nokia5110_OutString("  ");
            Nokia5110_SetCursor(0, 4);
            Nokia5110_OutString("--");
            Nokia5110_SetCursor(0, 5);
            Nokia5110_OutString("  ");
    }
    if(cordY == 5){
            Nokia5110_SetCursor(0, 2);
            Nokia5110_OutString("  ");
            Nokia5110_SetCursor(0, 3);
            Nokia5110_OutString("  ");
            Nokia5110_SetCursor(0, 4);
            Nokia5110_OutString("  ");
            Nokia5110_SetCursor(0, 5);
            Nokia5110_OutString("--");
    }
    SysCtlDelay(80000);
}


// Configurando as ondas que podem ser selecionadas pelo usuario no display
void sele_Ondas(){
    Nokia5110_SetCursor(0, 0);
    Nokia5110_OutString("OPCAO DE ONDA:");
    Nokia5110_SetCursor(12,2);
    Nokia5110_OutString("Quadrada");
    Nokia5110_SetCursor(12,3);
    Nokia5110_OutString("Rampa");
    Nokia5110_SetCursor(12,4);
    Nokia5110_OutString("Senoidal");
    Nokia5110_SetCursor(12,5);
    Nokia5110_OutString("Triangular");
}

// Configurando a amplitudo que será mostrada ao usuário no display
void sele_Ampli(){
    Nokia5110_SetCursor(0, 0);
    Nokia5110_OutString("DEFINA A");
    Nokia5110_SetCursor(0, 1);
    Nokia5110_OutString("AMPLITUDE:");
    Nokia5110_SetCursor(46, 4);
    Nokia5110_OutDec(amplitude);
    Nokia5110_SetCursor(68, 4);
    Nokia5110_OutString("%");
}

// Configurando a frequencia que será selecionado pelo usuario no display
void sele_Freq(){
    Nokia5110_SetCursor(0, 0);
    Nokia5110_OutString("DEFINA A");
    Nokia5110_SetCursor(0, 1);
    Nokia5110_OutString("FREQUENCIA");
    Nokia5110_SetCursor(46, 4);
    Nokia5110_OutDec(frequencia);
    Nokia5110_SetCursor(68, 4);
    Nokia5110_OutString("Hz");
}

// Configurando o plot que sera mostrado ao usuario apos a seleção das caracteristicas
void plot_Coluna(int x){
    int k = 47;

    for(k = 47; k > (47 - (47 * amplitude) / 100) - 1; k--){
        Nokia5110_SetPxl(k, x);
    }
}

// Vai ser mostrado as configurações ao usuario
void info_Geral(){
    Nokia5110_SetCursor(0, 0);
    Nokia5110_OutString("CONFIGURACAO:");
    Nokia5110_SetCursor(0, 2);
    Nokia5110_OutString("Onda:");
    Nokia5110_SetCursor(30, 2);
    Nokia5110_OutString(type_Wave[type_wave]);
    Nokia5110_SetCursor(0, 3);
    Nokia5110_OutString("Amplitude:");
    Nokia5110_SetCursor(63, 4);
    Nokia5110_OutDec(amplitude);
    Nokia5110_SetCursor(0, 4);
    Nokia5110_OutString("Freq.:");
    Nokia5110_SetCursor(63, 3);
    Nokia5110_OutDec(frequencia);
}

// Calcula a função quadrada a partir da seleção dos parametro de amplitude e frequencia selecionados pelo usuario
int func_Quadrada(int x){
    int T = (1680 / frequencia);
    int amplitude_tela = ((47 * amplitude) / 100);

    x = (x - aux * (T) - aux);

    if(x == T){
        aux++;
    }

    if(x < (T / 2)){
        return 47;
    }else{
        if(x >= (T / 2)){
            return (47 - amplitude_tela);
        }
    }
    return -1;
}

// Calcula a função rampa a partir da seleção dos parametro de amplitude e frequencia selecionados pelo usuario
int func_Ramp(int x){
    int T = (1680 / frequencia);
    int amplitude_tela = ((47 * amplitude) / 100);

    x = (x - aux * (T / 2) - aux);

    if(x == (T / 2)){
        aux++;
    }
    return (47 - (2 * amplitude_tela * x) / T);
}

// Calcula a função senoidal a partir da seleção dos parametro de amplitude e frequencia selecionados pelo usuario
int func_Sin(int x){
    return (int) (23 - (sin(2 * 3.1415 * frequencia * x)) * ((23 * amplitude) / 100));
}

// Calcula a função triangular a partir da seleção dos parametro de amplitude e frequencia selecionados pelo usuario
int func_Triang(int x){
    int T = (1680 / frequencia);
    int amplitude_tela = ((47 * amplitude) / 100);

    x = (x - aux * (T / 2));

    if(x == (T / 2)){
        aux++;
    }

    if(x < (T / 4)){
        return (47 - (4 * amplitude_tela * x) / T);
    }else{
        if(x > (T / 4)){
            return (amplitude_tela + (4 * amplitude_tela * (x - T / 4) / T));
        }
        else{
            return 47 - amplitude_tela;
        }
    }
}

// Função para ajustar o plot da onda selecionada
void display(){
    if(!plot){
        if(tipo_Modo){
            Nokia5110_Clear();
            sele_Ondas();
            while(tipo_Modo){
               load_Cursor();
            }
        }
        if(ampli_Tipo){
            Nokia5110_Clear();
            while(ampli_Tipo){
                sele_Ampli();
            }
        }
        if(freq_Tipo){
            Nokia5110_Clear();
            while(freq_Tipo){
                sele_Freq();
            }
        }
    }
}


// Função para plotar a onda escolhida pelo usuário
void plot_Waves() {
    int amplitude_tela = (47 * amplitude) / 100;
    int i;

    while(cont < 10){
        Nokia5110_Clear();
        Nokia5110_ClearBuffer();
        for(i = 0; i < 84; i++){
            switch(type_wave){
            case 2:
                if(abs(func_Quadrada(i + cont) - func_Quadrada(i + cont + 1)) >= amplitude_tela){
                    plot_Coluna(i);
                }
                Nokia5110_SetPxl(func_Quadrada(i + cont), i);
                break;
            case 3:
                if(abs(func_Ramp(i + cont) - func_Ramp(i + cont + 1)) >= amplitude_tela){
                    plot_Coluna(i + 1);
                }
                Nokia5110_SetPxl(func_Ramp(i + cont), i);
                break;
            case 4:
                Nokia5110_SetPxl(func_Sin(i + cont), i);
                break;
            case 5:
                Nokia5110_SetPxl(func_Triang(i + cont), i);
                break;
            }
        }
        Nokia5110_DisplayBuffer();
        SysCtlDelay(SysCtlClockGet() / 8);
        aux = 0;
        cont++;
    }
    cont = 0;
}

int main(void)
{
    Nokia5110_Init();
    Nokia5110_Clear();
    Nokia5110_ClearBuffer();
    set_Cursor(0, 0);
    setup_GPIO_display();
    setup_Timer_display();
    IntMasterEnable();
    display();

   while(1){
       plot_Waves();
       SysCtlDelay(SysCtlClockGet() / 12);
       Nokia5110_Clear();
       info_Geral();
       SysCtlDelay(SysCtlClockGet());
       Nokia5110_Clear();
   }
}
