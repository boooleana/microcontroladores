
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/hibernate.h"
#include "driverlib/interrupt.h"
#include "Nokia5110.h"
#include "bitmap.h"

int auxiliar = 0, aux2 = 0, tempmax = 0, tempmin = 1000, first_t, temperature2;
int option = 0;
float aux;
int dia_semana=0,dia_mes=0,mes=0,hora=0,minutos=0,i=0;
bool first = false;



void temperatura(int temp){

    Nokia5110_Clear();
    Nokia5110_ClearBuffer();
    //------------ TERMOMETRO --------------------------|
    if(first == false){
        first_t = temp;
        first = true;
    }
    if(temp >= first_t - 10 && temp <= first_t + 10){
          // termometro medium
        Nokia5110_PrintBMP2(5, 12, medium, 10, 30);
    }
    if(temp >= first_t - 30 && temp < first_t - 10){
          // termometro low
        Nokia5110_PrintBMP2(5, 12, low, 10, 30);
    }
    if(temp < first_t - 30){
          // termometro very low
        Nokia5110_PrintBMP2(5, 12, very_low, 10, 30);
    }
    if(temp <= first_t + 30 && temp > first_t + 10){
          // termometro high
        Nokia5110_PrintBMP2(5, 12, high, 10, 30);
    }
    if(temp > first_t + 30){
          // termometro very high
        Nokia5110_PrintBMP2(5, 12, very_high, 10, 30);
    }
    //-------------------------------------------------------||
    Nokia5110_PrintBMP2(22, 12, num_big[temp/100], 15, 30);
    Nokia5110_PrintBMP2(41, 12, num_big[(temp%100)/10], 15, 30);
    Nokia5110_PrintBMP2(58, 12, virgula, 4, 30);
    Nokia5110_PrintBMP2(63, 12, num_big[temp%10], 15, 30);
    Nokia5110_DisplayBuffer();
    temperature2 = temp;
}

void media_temperatura(int temp){
    Nokia5110_Clear();
    Nokia5110_ClearBuffer();

    //------------ TERMOMETRO --------------------------|

        if(first == false){
            first_t = temp;
            first = true;
        }

        if(temp < tempmin){
            tempmin = temp;
        }
        if(temp > tempmax){
            tempmax = temp;
        }

//------------------- temp 1 -----------------------|
    Nokia5110_PrintBMP2(5, 12, tempe, 15, 5);


    Nokia5110_PrintBMP2(21, 12, one_small, 3, 5);
    Nokia5110_PrintBMP2(24, 12, two_point, 3, 5);

    Nokia5110_PrintBMP2(30, 12, num_small[tempmin/100], 3, 5);
    Nokia5110_PrintBMP2(34, 12, num_small[(tempmin%100)/10], 3, 5);
    Nokia5110_PrintBMP2(37, 12, one_point, 3, 5);
    Nokia5110_PrintBMP2(40, 12, num_small[tempmin%10], 3, 5);
    Nokia5110_PrintBMP2(44, 12, celsius, 11, 5);

//------------------- temp 2 -----------------------|
    Nokia5110_PrintBMP2(5, 22, tempe, 15, 5);
    Nokia5110_PrintBMP2(21, 22, two_small, 3, 5);
    Nokia5110_PrintBMP2(24, 22, two_point, 3, 5);

    Nokia5110_PrintBMP2(30, 22, num_small[tempmax/100], 3, 5);
    Nokia5110_PrintBMP2(34, 22, num_small[(tempmax%100)/10], 3, 5);
    Nokia5110_PrintBMP2(37, 22, one_point, 3, 5);
    Nokia5110_PrintBMP2(40, 22, num_small[tempmax%10], 3, 5);
    Nokia5110_PrintBMP2(44, 22, celsius, 11, 5);


//------------------ temp 3 ------------------------|
    Nokia5110_PrintBMP2(5, 32, tempe, 15, 5);
    Nokia5110_PrintBMP2(21, 32, three_small, 3, 5);
    Nokia5110_PrintBMP2(24, 32, two_point, 3, 5);

    Nokia5110_PrintBMP2(30, 32, num_small[temp/100], 3, 5);
    Nokia5110_PrintBMP2(34, 32, num_small[(temp%100)/10], 3, 5);
    Nokia5110_PrintBMP2(37, 32, one_point, 3, 5);
    Nokia5110_PrintBMP2(40, 32, num_small[temp%10], 3, 5);
    Nokia5110_PrintBMP2(44, 32, celsius, 11, 5);
    Nokia5110_DisplayBuffer();

}

int main(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //habilitando interrupcoes no portal A
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    //----------tela
    Nokia5110_Init();
    Nokia5110_Clear();
    //------------------------------- TEMPO ---------------------------------------------|
    time_t t = time(NULL);
    //t+=10800;
    struct tm* timePtr = localtime(&t);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    HibernateEnableExpClk(SysCtlClockGet());
    HibernateRTCSet(t);
    HibernateRTCEnable();
    dia_semana = timePtr->tm_wday;
    dia_mes =  timePtr->tm_mday;
    mes =  timePtr->tm_mon + 1;
    hora = timePtr->tm_hour + 3;
    minutos = timePtr->tm_min;

    //------------------------ TIMER PRA TROCAR A TELA ----------------------------------|
    IntMasterEnable();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()*10);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    //------------------------ TERMOMETRO DO TIVA ---------------------------------------|

    uint32_t temp_vetor[1];

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 1);
    //-----------------------------------------------------------------------------------|
    TimerEnable(TIMER1_BASE, TIMER_A);
    //----------------- CHAMADA DA PRIMEIRA TROCA DE TELA ---------------------------|
    //data_hora(dia_mes, dia_semana, mes, hora, minutos);
    Nokia5110_DrawFullImage(tempo1);
    while(1)
    {

        time_t t = time(NULL);
        timePtr = localtime(&t);
        HibernateEnableExpClk(SysCtlClockGet());
        HibernateRTCSet(t);
        HibernateRTCEnable();
        dia_semana = timePtr->tm_wday;
        dia_mes =  timePtr->tm_mday;
        mes =  timePtr->tm_mon + 1;
        hora = timePtr->tm_hour + 3;  //foi preciso adicionar 3 pois o código pega um horário diferenet da bios, se estiver 3h atrasado, retirar o +3.
        minutos = timePtr->tm_min;
        ADCIntClear(ADC0_BASE, 1);
        ADCProcessorTrigger(ADC0_BASE, 1);
        while(!ADCIntStatus(ADC0_BASE, 1, false)){}

        ADCSequenceDataGet(ADC0_BASE, 1, temp_vetor);
        aux = (1475 - ((2475 * (float)temp_vetor[0])) / 4096.0)/10.0;
        aux2 = aux*10;
        auxiliar = (int)aux2;

    }
}

void Timer1IntHandler(void){
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT); // reinicia o valor do timer


    if(option==0){

        media_temperatura(auxiliar);

    }
    if(option==1){

        media_temperatura(auxiliar);
        //temperatura();
    }


    option = (option+1)%2;

    TimerEnable(TIMER1_BASE, TIMER_A); //reinicia o timer
}
