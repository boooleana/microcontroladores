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
#include "bitmap.h"

//Declaração das funções utilizadas
char* selecao_Menu[6] = {"", "", "Play", "Guide"};
int cordX;
int cordY;
int cont = 0;
int aux = 0;
int in_game = 0;
int selecao_menu = 0;
int amplitude = 0;
int frequencia= 0;
int ins_1 = 1;
int menu_Tipo= 0;
int ins_jogo = 0;
uint8_t x = 3;
uint8_t y = 2;
int direita = 1;
int esquerda = 0;
int cima = 0;
int baixo = 0;


void move(){
    if(direita){
        x+=3;
        if(x>81)  x=81;
    }
    if(esquerda){
        x-=3;
        if(x<3){
            x=3;
        }
    }
    if(cima){
        y-=3;
        if(y<3){
            y=3;
        }
    }
    if(baixo){
        y+=3;
        if(y>81){
            y=81;
        }
    }
}

void GPIOF_Handler(){
    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    //Criando a função que vai ler os botoes SW1 e SW2
    unsigned int leitura = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    //Habiltiando o timer
    TimerEnable(TIMER0_BASE, TIMER_A);

    if(!in_game){
        if(ins_1){
            if(leitura == GPIO_PIN_0){
                menu_Tipo = 1;
                ins_1 = 0;
            }
        }else{
            if(menu_Tipo){
                if(leitura == GPIO_PIN_0){ //select
                    selecao_menu = cordY;
                    menu_Tipo = 0;

                    if(selecao_menu == 2){
                        in_game = 1;
                        selecao_menu = 0;
                    }
                    if(selecao_menu == 3){
                        ins_jogo = 1;
                        selecao_menu = 0;
                    }
                    SysCtlDelay(200000);
                    Nokia5110_Clear();
                }
                if(leitura == GPIO_PIN_4){ //next
                    if(cordY == 3){
                        cordY = 2;
                    }else{
                        cordY++;
                    }
                }
            }else{
                if(ins_jogo){
                    if(leitura == GPIO_PIN_0){ //select
                        ins_jogo = 0;
                        menu_Tipo = 1;
                        SysCtlDelay(200000);
                        Nokia5110_Clear();
                    }
                }
            }
        }
    }else{
        if(leitura == GPIO_PIN_0){ //move
            move();
        }
        if(leitura == GPIO_PIN_4){ //turn
            if(direita){
                direita = 0;
                baixo = 1;
            }else{
                if(esquerda){
                    esquerda = 0;
                    cima = 1;
                }else{
                    if(cima){
                        cima = 0;
                        direita = 1;
                    }else{
                        if(baixo){
                            baixo = 0;
                            esquerda = 1;
                        }
                    }
                }
            }
        }
    }
}


void Timer0IntHandler(){
    //Limpa as fontes de interrupção do temporizador
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    TimerDisable(TIMER0_BASE, TIMER_A);
}

//  Configurando o timer display
void setup_Timer_display(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
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
    SysCtlDelay(80000);
}


// Configurando instrucoes iniciais de comando
void instrucao_inicial(){
    Nokia5110_SetCursor(14, 0);
    Nokia5110_OutString("COMANDOS");
    Nokia5110_SetCursor(4,2);
    Nokia5110_OutString("SW1 - Select");
    Nokia5110_SetCursor(4,3);
    Nokia5110_OutString("SW2 - Next");
    Nokia5110_SetCursor(13,5);
    Nokia5110_OutString("press SW1");
}

// Configurando o menu do jogo
void menu(){
    Nokia5110_SetCursor(30, 0);
    Nokia5110_OutString("MAZE");
    Nokia5110_SetCursor(20,2);
    Nokia5110_OutString("Play");
    Nokia5110_SetCursor(20,3);
    Nokia5110_OutString("Guide");
}

// Configurando instrucoes de jogo
void instrucao_jogo(){
    Nokia5110_SetCursor(16, 0);
    Nokia5110_OutString("COMANDOS");
    Nokia5110_SetCursor(13,2);
    Nokia5110_OutString("SW1 - Move");
    Nokia5110_SetCursor(13,3);
    Nokia5110_OutString("SW2 - Turn");
    Nokia5110_SetCursor(13,5);
    Nokia5110_OutString("press SW1");
}

void end_game(){
    Nokia5110_DrawFullImage(end);
}


// Função para ajustar o plot da onda selecionada
void display(){
    if(!in_game){
        if(ins_1){
            Nokia5110_Clear();
            instrucao_inicial();
        }
        if(menu_Tipo){
            Nokia5110_Clear();
            menu();
            while(menu_Tipo){
               load_Cursor();
            }
        }
        if(ins_jogo){
            Nokia5110_Clear();
            instrucao_jogo();

        }

    }else{
        Nokia5110_Clear();
        set_mapa_buffer(maze);
        //Nokia5110_PrintBMP2(0,0,maze,84,48);

        if(direita){
            Nokia5110_PrintBMP2(x,y,seta_direita,3,5);
            Nokia5110_DisplayBuffer();
        }
        if(esquerda){
            Nokia5110_PrintBMP2(x,y,seta_esquerda,3,5);
            Nokia5110_DisplayBuffer();
        }
        if(cima){
            Nokia5110_PrintBMP2(x,y+1,seta_cima,5,3);
            Nokia5110_DisplayBuffer();
        }
        if(baixo){
            Nokia5110_PrintBMP2(x,y+1,seta_baixo,5,3);
            Nokia5110_DisplayBuffer();
        }

        if(x>75 && y>40){
            Nokia5110_Clear();
            end_game();
        }
    }

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


   while(1){
       display();
       SysCtlDelay(SysCtlClockGet() / 12);
       Nokia5110_Clear();

   }
}
