#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include <driverlib/timer.h>
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"  // habilita macros da interrupÃ§Ã£o
#include "driverlib/timer.c"
#include "Nokia5110.h"
#include "bitmap.h"
#include <time.h>
#include <driverlib/timer.h>
#include <math.h>

#define GPIO_O_LOCK                 0x520
#define GPIO_O_CR                   0x524
#define GPIO_LOCK_KEY               0x4C4F434B

volatile int perdeu=0,select=0,pontuacao=0,x=2,y=2;
volatile static int ElefPos=0,ini_tela=0;
volatile bool ingame=0;

void start(void){
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    Nokia5110_Init();
    //habilita as portas do tipo F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //destrava o PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//timer

    //configura PF0 E PF4 como entrada
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_DIR_MODE_IN);//configura o pin 4 e 0 como entrada
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_INT_PIN_0|GPIO_INT_PIN_4,GPIO_FALLING_EDGE);//configura as interrupções para borda de descida
    IntEnable(INT_GPIOF);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0|GPIO_INT_PIN_4);
    IntMasterEnable();


}

/*void inimigo1(void){//apenas o inimigo do chao, q usa a funcao de pulo para desviar
    if (ingame==1) {
        enemypos=enemypos-5;
        Nokia5110_PrintBMP2(enemypos,35,ratinho,10,9);
        Nokia5110_DisplayBuffer();
        while(enemypos<=31 && ElefPos!=1){
                    enemypos=79;
                    Nokia5110_Clear();
                    Nokia5110_ClearBuffer();
        }
    }
}*/


void print_pontuacao(void){
        Nokia5110_SetCursor(0,0);
        Nokia5110_OutString("Score: ");
        Nokia5110_OutChar(0x30+pontuacao/100); // hundred’s digit
        Nokia5110_OutChar(0x30+(pontuacao%100)/10); // ten’s digit
        Nokia5110_OutChar(0x30+(pontuacao%100)%10); // one’s digit
}
void timer_configure(void){
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet() /1)/ 120);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

}
void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    SysCtlDelay(10000000);
    ElefPos=0;
}

void print_gameover(void){
    Nokia5110_PrintBMP2(0,0,gameoverbitmap,84,48);
}

void escolhe_inimigo(void){
    static int i=0;
    i=rand()%2;
    ini_tela=i;
}
void print_inimigo1(void){
    volatile static int enemypos=80;
    ini_tela=0;
    if(enemypos>30){
        ini_tela=0;
        SysCtlDelay(1000);
        enemypos=enemypos-5;
        Nokia5110_PrintBMP2(enemypos,35,ratinho,10,9);
        Nokia5110_DisplayBuffer();
    }
    else if(enemypos<=34 && ElefPos!=1){
        ini_tela=0;
        select=3;
        ingame=0;
        ElefPos=0;
        enemypos=80;
    }else if(enemypos<=30 && ElefPos==1){
        pontuacao++;
        escolhe_inimigo();
        enemypos=80;
    }
}


void print_inimigo2(void){
    ini_tela=1;
    volatile static int enemypos=80;
    if(enemypos>30){
        ini_tela=1;
        SysCtlDelay(1000);
        enemypos=enemypos-5;
        Nokia5110_PrintBMP2(enemypos,26,flecha,10,9);
        Nokia5110_DisplayBuffer();
    }
    else if(enemypos<=34 && ElefPos!=2){
        ini_tela=1;
        select=3;
        ingame=0;
        ElefPos=0;
        enemypos=80;
    }else if(enemypos<=30 && ElefPos==2){
        pontuacao++;
        escolhe_inimigo();
        enemypos=80;
    }
}
void inimigo(void){
    if(ini_tela==0){
        print_inimigo1();
    }else{
        print_inimigo2();
    }
}


void Timer1IntHandler(void){
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    inimigo();
}
void instruct(void){//menus=2//intrucoes. segundo menu
    Nokia5110_PrintBMP2(0,0,instrucoes,84,48);
}
void print_andando(void){
    Nokia5110_PrintBMP2(0,45,chao,84,3);
    Nokia5110_PrintBMP2(0,26,elefante,31,19);
    print_pontuacao();
    Nokia5110_DisplayBuffer();
    inimigo();
}
void print_abaixado(void){
    Nokia5110_PrintBMP2(0,45,chao,84,3);
    Nokia5110_PrintBMP2(0,30,elefante_rebaixado,31,19);
    Nokia5110_DisplayBuffer();
    inimigo();
    print_pontuacao();
}
void print_pulando(void){
    Nokia5110_PrintBMP2(0,45,chao,84,3);
    Nokia5110_PrintBMP2(0,12,elefpulando,31,18);
    Nokia5110_DisplayBuffer();
    inimigo();
    print_pontuacao();
}

void setaDireita(void){
    Nokia5110_PrintBMP2(0,0,maze,84,48);
    Nokia5110_PrintBMP2(x,y,seta_direita,3,5);
}
void print_menu_inicial2(void){//menus=0 menu principal
    Nokia5110_PrintBMP2(0,0,maze,84,48);
}

void IntPortalF(void){

    if(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) & GPIO_PIN_0 )){//direita seleciona//sw2
        if(ingame==0){//se não estiver no jogo
            switch(select){
            case 0:
                ingame=1;
                break;
            case 1:
                select=2;
                break;
            default:
                ingame=0;
                select=0;
                break;
            }
        }
        else{ // se estiver no jogo -  mudar setas
            if(ElefPos==0){
                ElefPos=1;
            }
        }
    }
    if(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) & GPIO_PIN_4 )){//esquerda troca seleção//sw1
        if(ingame==0){//se não estiver no jogo
            switch (select){
            case 0:
                select=1;
                break;
            case 1:
                select=0;
                break;
            case 2:
                select=0;
                ingame=1;
                break;
            case 3:
                select=0;
                ingame=0;
                break;
            default:
                select=0;
                break;
            }
        }
        else{//se estiver no jogo - andar
            if(ElefPos==0){
                ElefPos=2;
            }
        }
    }
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    SysCtlDelay(3000000);
}


int main(void){
    start();//configuracoes iniciais
    //Nokia5110_DrawFullImage(maze);
    while(1){
        Nokia5110_Clear();
        Nokia5110_ClearBuffer();
        //Nokia5110_DrawFullImage(maze);

        if(ingame==0){//detecta se deve estar no menu ou no jogo.
            switch(select){
            case 0:
                setaDireita();
                break;
            case 1:
                print_menu_inicial2();
                break;
            case 2:
                instruct();
                break;
            case 3:
                print_gameover();
                break;
            default:
                setaDireita();
                break;
            }
        }else{

            switch(ElefPos){
            case 0:
                print_andando();
                break;
            case 1:
                print_pulando(); //mudar direcao
                Timer0IntHandler();
                break;
            case 2:
                print_abaixado(); //andar
                Timer0IntHandler();
                break;
            default:
                print_andando();
                inimigo();
                break;
            }
            print_pontuacao();

        }

        Nokia5110_DisplayBuffer();
        SysCtlDelay(3800000);//delay para nao ficar bugado a impressao na tela
    }
}
