uint8_t ui8IntSource;
   uint32_t ui32Period;

   ui8IntSource = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0);
    if ((ui8IntSource & 0x01) ==  0x00) //SW2 TIVA
    {

        if ((ui8IntSource & 0x02) == 0x02)
                       {
                           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
                       }
                   else
                       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);

        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);


    }
    if ((ui8IntSource & 0x10) ==  0x00) //SW1 TIVA

        {
            if ((ui8IntSource & 0x04) == 0x04)
                           {
                               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
                           }
                       else
                           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
        }